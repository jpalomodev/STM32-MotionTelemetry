#include "UART_Driver.hpp"
#include <cerrno>
#include <chrono>
#include <ctime>
#include <fcntl.h>
#include <future>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <poll.h>
#include <string>
#include <thread>
#include <unistd.h>

UART_Driver::UART_Driver() : tx_ready(0), rx_ack(0), uart_fd(-1) {}

void UART_Driver::init() {
    UART_Driver::open_connection();
    UART_Driver::configure_terminal();
    // UART_Driver::handshake();
    UART_Driver::consume();
    UART_Driver::close_connection();
}

void UART_Driver::open_connection() {
    constexpr char kDevicePath[] = "/dev/ttyACM0";
    uart_fd = open(kDevicePath, O_RDWR);

    if (uart_fd == -1) {
        if (errno == ENOENT) {
            throw std::system_error(errno, std::generic_category(),
                                    "Failed to find STM32H533RE.");
        }

        if (errno == EACCES) {
            throw std::system_error(
                errno, std::generic_category(),
                "Do not have permission to access STM32H533RE.");
        }

        throw std::system_error(
            errno, std::generic_category(),
            "Unexpected connection failure to STM32H533RE.");
    }

    UART_Driver::log("Connected to STM32H533RE ✓");
};

void UART_Driver::configure_terminal() {
    if (tcgetattr(uart_fd, &termios_default) == -1) {
        throw std::system_error(errno, std::generic_category(),
                                "Failed to get default terminal parameters.");
    }

    termios_custom = termios_default;
    cfmakeraw(&termios_custom);
    cfsetspeed(&termios_custom, B115200);

    if (tcsetattr(uart_fd, TCSANOW, &termios_custom) == -1) {
        throw std::system_error(errno, std::generic_category(),
                                "Failed to set new terminal parameters.");
    }

    if (tcflush(uart_fd, TCIOFLUSH) == -1) {
        throw std::system_error(errno, std::generic_category(),
                                "Failed to flush IO data.");
    }

    UART_Driver::log("Configured terminal for raw IO communication ✓");
}

void UART_Driver::handshake() {
    UART_Driver::log("Started handshake ✓");

    int ready = 1;
    if (write(uart_fd, &ready, sizeof(ready)) == -1) {
        throw std::system_error(errno, std::generic_category(),
                                "Failed to send RDY message to STM32H533RE.");
    }

    UART_Driver::log("Sent RDY message to STM32H533RE (Awaiting response) ✓");

    pollfd pfd{uart_fd, POLLIN, 0};
    constexpr uint32_t kPollTimeout = 5000;
    int poll_result = poll(&pfd, 1, kPollTimeout);

    if (poll_result == -1) {
        throw std::system_error(errno, std::generic_category(),
                                "Failed to poll for input event.");
    }

    if (poll_result == 0) {
        throw std::system_error(errno, std::generic_category(),
                                "Failed to receive ACK message from "
                                "STM32H533RE before timeout.");
    }

    if (read(uart_fd, &rx_ack, 1) == -1) {
        throw std::system_error(errno, std::generic_category(),
                                "Failed to read ACK message from STM32H533RE.");
    };

    if (rx_ack == 0) {
        throw std::system_error(
            errno, std::generic_category(),
            "Failed to complete handshake between Host and STM32H533RE.");
    }

    UART_Driver::log("Received ACK message from STM32H533RE ✓");
}

void UART_Driver::consume() {
    UART_Driver::log("Started ADXL367 consumption ✓");

    uint8_t packet[11] = {};
    uint8_t packet_progress = 0;

    while (true) {
        size_t max_bytes = sizeof(rx_buff) / sizeof(rx_buff[0]);
        ssize_t bytes_read = read(uart_fd, rx_buff, max_bytes);

        if (bytes_read == -1) {
            throw std::system_error(errno, std::generic_category(),
                                    "Failed to read data from STM32H533RE.");
        }

        for (size_t i = 0; i < (size_t)bytes_read; i++) {
            if (packet_progress != 0 && packet_progress != 11) {
                packet[packet_progress] = rx_buff[i];
                packet_progress++;

                if (packet_progress == 11) {
                    std::cout << std::format("[PACKET {}] ", (int)packet[2]);
                    std::cout << (int)packet[3] << " |";
                    std::cout << (int)packet[5] << " |";
                    std::cout << (int)packet[7] << "\n";

                    for (int j = 0; j < 11; j++) {
                        packet[j] = 0;
                    }

                    packet_progress = 0;
                }
            }

            else if (i != 0 && rx_buff[i] == 0x55 && rx_buff[i - 1] == 0xA2) {
                packet[0] = 0xA2;
                packet[1] = 0x55;
                packet_progress += 2;
            }
        }
    }
}

void UART_Driver::close_connection() {
    if (tcsetattr(uart_fd, TCSANOW, &termios_default) == -1) {
        throw std::system_error(
            errno, std::generic_category(),
            "Failed to restore default terminal parameters.");
    }

    if (close(uart_fd) == -1) {
        throw std::system_error(errno, std::generic_category(),
                                "Failed to disconnect from STM32H533RE.");
    }

    UART_Driver::log("Disconnected from STM32H533RE ✓");
};

void UART_Driver::log(std::string message) {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);

    std::tm local_time{};
    localtime_r(&time, &local_time);

    std::cout << "[" << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S") << "] "
              << message + "\n";
}

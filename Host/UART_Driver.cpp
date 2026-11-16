#include "UART_Driver.hpp"
#include <cerrno>
#include <chrono>
#include <ctime>
#include <fcntl.h>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>

UART_Driver::UART_Driver() : uart_fd(-1) {}

void UART_Driver::init() {
    UART_Driver::open_connection();
    UART_Driver::configure_terminal();
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

    cfmakeraw(&termios_custom);
    cfsetspeed(&termios_custom, B115200);

    if (tcsetattr(uart_fd, TCSANOW, &termios_custom) == -1) {
        throw std::system_error(errno, std::generic_category(),
                                "Failed to set new terminal parameters.");
    }

    if (tcflush(uart_fd, TCIOFLUSH)) {
        throw std::system_error(errno, std::generic_category(),
                                "Failed to flush IO data.");
    }

    UART_Driver::log("Configured terminal for raw IO communication ✓");
}

void UART_Driver::consume() {
    UART_Driver::log("Starting ADXL367 data consumption ✓");
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    while (true) {
        size_t max_bytes = sizeof(rx_buff) / sizeof(rx_buff[0]);
        ssize_t bytes_read = read(uart_fd, rx_buff, max_bytes);

        if (bytes_read == -1) {
            throw std::system_error(errno, std::generic_category(),
                                    "Failed to read data from STM32H533RE.");
        }

        std::cout << std::string(rx_buff, bytes_read);
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

#pragma once
#include <cstdint>
#include <string>
#include <termios.h>
#include <unistd.h>
#define RX_BUFF_SIZE 256

class UART_Driver {
public:
    UART_Driver();
    void init();

private:
    termios termios_default;
    termios termios_custom;

    int tx_ready;
    int rx_ack;

    int uart_fd;
    uint8_t rx_buff[RX_BUFF_SIZE] = {};

    void open_connection();
    void configure_terminal();
    void handshake();
    void consume();
    void close_connection();
    void log(std::string message);
};

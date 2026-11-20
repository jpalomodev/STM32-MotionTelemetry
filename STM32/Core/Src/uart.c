#include "uart.h"

void UART_Transmit(UART_HandleTypeDef* huart, uint8_t* buff, uint8_t size) {
    const uint16_t UART_TIMEOUT = 5000;
    HAL_UART_Transmit(huart, buff, size, UART_TIMEOUT);
}

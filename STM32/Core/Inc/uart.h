#ifndef UART_H
#define UART_H
#include "stm32h5xx_hal.h"

void UART_Transmit(UART_HandleTypeDef* huart, uint8_t* buff, uint8_t size);

#endif

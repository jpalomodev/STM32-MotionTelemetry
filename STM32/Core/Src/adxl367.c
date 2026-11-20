#include "adxl367.h"
#include "stdio.h"

static GPIO_TypeDef* const ADXL_GPIOX = GPIOA;
static const uint16_t ADXL_CS_PIN = GPIO_PIN_9;

enum ADXL_STATUS ADXL_Op(SPI_HandleTypeDef* hspi,
                         enum ADXL_COMMAND adxl_command,
                         enum ADXL_REGISTER adxl_register, uint8_t* buff) {
    const uint16_t ADXL_TIMEOUT = 5000;

    HAL_GPIO_WritePin(ADXL_GPIOX, ADXL_CS_PIN, GPIO_PIN_RESET);

    uint8_t adxl_command_byte = adxl_command;
    HAL_SPI_Transmit(hspi, &adxl_command_byte, 1, ADXL_TIMEOUT);

    uint8_t adxl_register_byte = adxl_register;
    HAL_SPI_Transmit(hspi, &adxl_register_byte, 1, ADXL_TIMEOUT);

    if (adxl_command == ADXL_WRITE) {
        if (HAL_SPI_Transmit(hspi, buff, 1, ADXL_TIMEOUT) == HAL_ERROR) {
            HAL_GPIO_WritePin(ADXL_GPIOX, ADXL_CS_PIN, GPIO_PIN_SET);
            return ADXL_WRITE_ERROR;
        }
    }

    if (adxl_command == ADXL_READ) {
        if (HAL_SPI_Receive(hspi, buff, 1, ADXL_TIMEOUT) == HAL_ERROR) {
            HAL_GPIO_WritePin(ADXL_GPIOX, ADXL_CS_PIN, GPIO_PIN_SET);
            return ADXL_READ_ERROR;
        }
    }

    HAL_GPIO_WritePin(ADXL_GPIOX, ADXL_CS_PIN, GPIO_PIN_SET);

    return ADXL_OK;
}

enum ADXL_STATUS ADXL_Init_Measure_Mode(SPI_HandleTypeDef* hspi) {
    uint8_t adxl_write_byte = ADXL_POWER_CTL_MODE_MEASURE;
    if (ADXL_Op(hspi, ADXL_WRITE, ADXL_POWER_CTL, &adxl_write_byte) !=
        ADXL_OK) {
        return ADXL_INIT_MEASURE_MODE_ERROR;
    }

    HAL_Delay(100);
    return ADXL_OK;
}

enum ADXL_STATUS ADXL_Capture_Data(SPI_HandleTypeDef* hspi, uint8_t* buff) {
    if (ADXL_Op(hspi, ADXL_READ, ADXL_XDATA_H, &buff[0]) != ADXL_OK) {
        // Handle error
    }

    if (ADXL_Op(hspi, ADXL_READ, ADXL_XDATA_L, &buff[1]) != ADXL_OK) {
        // Handle error
    }

    if (ADXL_Op(hspi, ADXL_READ, ADXL_YDATA_H, &buff[2]) != ADXL_OK) {
        // Handle error
    }

    if (ADXL_Op(hspi, ADXL_READ, ADXL_YDATA_L, &buff[3]) != ADXL_OK) {
        // Handle error
    }

    if (ADXL_Op(hspi, ADXL_READ, ADXL_ZDATA_H, &buff[4]) != ADXL_OK) {
        // Handle error
    }

    if (ADXL_Op(hspi, ADXL_READ, ADXL_ZDATA_L, &buff[5]) != ADXL_OK) {
        // Handle error
    }

    return ADXL_OK;
}

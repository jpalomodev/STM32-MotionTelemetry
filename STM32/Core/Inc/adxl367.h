#ifndef ADXL367_H
#define ADXL367_H
#include "stm32h5xx_hal.h"

enum ADXL_STATUS {
    ADXL_OK,
    ADXL_WRITE_ERROR,
    ADXL_READ_ERROR,
    ADXL_INIT_MEASURE_MODE_ERROR
};

enum ADXL_COMMAND { ADXL_WRITE = 0x0A, ADXL_READ = 0x0B };

enum ADXL_REGISTER {
    ADXL_DEVID_AD = 0x00,
    ADXL_POWER_CTL = 0x2D,
    ADXL_XDATA_H = 0x0E,
    ADXL_XDATA_L = 0x0F,
    ADXL_YDATA_H = 0x10,
    ADXL_YDATA_L = 0x11,
    ADXL_ZDATA_H = 0x12,
    ADXL_ZDATA_L = 0x13
};

enum ADXL_POWER_CTL_MODE { ADXL_POWER_CTL_MODE_MEASURE = 0x02 };

enum ADXL_STATUS ADXL_Op(SPI_HandleTypeDef* hspi,
                         enum ADXL_COMMAND adxl_command,
                         enum ADXL_REGISTER adxl_register, uint8_t* buff);

enum ADXL_STATUS ADXL_Init_Measure_Mode(SPI_HandleTypeDef* hspi);

enum ADXL_STATUS ADXL_Capture_Data(SPI_HandleTypeDef* hspi, uint8_t* buff);

#endif

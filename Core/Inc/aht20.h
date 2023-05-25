//
// Created by dong on 2023/5/25.
//
#include "stm32f1xx_hal.h"
#ifndef HELLO_NEW_TEMP_AHT20_H
#define HELLO_NEW_TEMP_AHT20_H
void delay(uint32_t ms_delay);
void I2C_Init(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_WriteByte(uint8_t data);
uint8_t I2C_ReadByte(void);
void AHT20_WriteCommand(uint8_t cmd);
void AHT20_ReadData(uint16_t *temp, uint16_t *humi);
#endif //HELLO_NEW_TEMP_AHT20_H

//
// Created by dong on 2023/5/25.
//

#ifndef HELLO_NEW_TEMP_OLED_H
#define HELLO_NEW_TEMP_OLED_H
#include "stm32f1xx_hal.h"

//常用ASCII表
//偏移量32
//ASCII字符集
//偏移量32
//大小:12*6
void OLED_GPIO(uint8_t add, uint8_t date);

void IIC_Start();

void IIC_Stop();

void IIC_Wait_Ack();

void Write_IIC_Byte(uint8_t IIC_Byte);

void Write_IIC_Command(uint8_t IIC_Command);

void Write_IIC_Data(uint8_t IIC_Data);

void OLED_WR_Byte(uint8_t dat, uint8_t cmd);

void OLED_Set_Pos(uint8_t x, uint8_t y);

void OLED_Display_On(void);

void OLED_Clear(void);

void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size);

uint16_t oled_pow(uint8_t m, uint8_t n);

void OLED_ShowNum(uint8_t x, uint8_t y, uint16_t num, uint8_t wei, uint8_t size2);

void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr, uint8_t Char_Size);

void OLED_DrawBMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t BMP[]);

void OLED_Init(void);
#endif //HELLO_NEW_TEMP_OLED_H

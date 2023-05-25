#ifndef __OLED_H_
#define __OLED_H_

#include "stm32f1xx_hal.h"
#include "oled.h"
#include "ziku.h"
/**************************************************/
/*                宏定义                          */
/**************************************************/
#define OLED_CMD  0    //写命令
#define OLED_DATA 1    //写数据
#define OLED_MODE 0
#define OLED_SDA  5
#define OLED_CLK  6

//OLED模式设置
//0:4线串行模式
//1:并行8080模式

#define SIZE 16
#define XLevelL        0x02
#define XLevelH        0x10
#define Max_Column    128
#define Max_Row        64
#define    Brightness    0xFF
#define X_WIDTH    128
#define Y_WIDTH    64


void OLED_GPIO(uint8_t add, uint8_t date) {
    if (add == 0) {
        if (date == 1) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
        else if (date == 0)HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
    } else if (add == 1) {
        if (date == 1) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
        else if (date == 0)HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
    }
}
/**************************************************/
/*               IIC Start                        */
/**************************************************/
void IIC_Start() {

    OLED_GPIO(OLED_SDA, 1);
    OLED_GPIO(OLED_CLK, 1);
    OLED_GPIO(OLED_SDA, 0);
    OLED_GPIO(OLED_CLK, 0);
}

/**************************************************/
/*               IIC Stop                         */
/**************************************************/
void IIC_Stop() {
    OLED_GPIO(OLED_CLK, 1);
    OLED_GPIO(OLED_SDA, 0);
    OLED_GPIO(OLED_CLK, 1);

    OLED_GPIO(OLED_SDA, 1);
    OLED_GPIO(OLED_CLK, 1);

}

void IIC_Wait_Ack()  //检测应答
{
    OLED_GPIO(OLED_SDA, 0);
    OLED_GPIO(OLED_CLK, 1);
    OLED_GPIO(OLED_CLK, 0);

}
/**************************************************/
/*             IIC Write byte                     */
/**************************************************/
void Write_IIC_Byte(uint8_t IIC_Byte) {
    uint8_t i;
    uint8_t m, da;
    da = IIC_Byte;
    OLED_GPIO(OLED_CLK, 0);

    for (i = 0; i < 8; i++) {
        m = da;
        //	OLED_SCLK_Clr();
        m = m & 0x80;
        if (m == 0x80) {
            OLED_GPIO(OLED_SDA, 1);
        } else
            OLED_GPIO(OLED_SDA, 0);
        da = da << 1;
        OLED_GPIO(OLED_CLK, 1);
        OLED_GPIO(OLED_CLK, 0);

    }


}
/*********************************************/
/*      	IIC Write Command		   	     */
/*********************************************/
void Write_IIC_Command(uint8_t IIC_Command) //写命令
{
    IIC_Start();
    Write_IIC_Byte(0x78);    // OLED的I2C地址（禁止修改）
    IIC_Wait_Ack();
    Write_IIC_Byte(0x00);    // OLED 指令（禁止修改）
    IIC_Wait_Ack();
    Write_IIC_Byte(IIC_Command);
    IIC_Wait_Ack();
    IIC_Stop();
}
/*********************************************/
/*       	IIC Write Data	  	   	         */
/*********************************************/
void Write_IIC_Data(uint8_t IIC_Data)   //写数据
{
    IIC_Start();
    Write_IIC_Byte(0x78);        // OLED的I2C地址（禁止修改）
    IIC_Wait_Ack();
    Write_IIC_Byte(0x40);        // OLED 数据（禁止修改）
    IIC_Wait_Ack();
    Write_IIC_Byte(IIC_Data);
    IIC_Wait_Ack();
    IIC_Stop();
}

void OLED_WR_Byte(uint8_t dat, uint8_t cmd) {
    if (cmd) {
        Write_IIC_Data(dat);
    } else {
        Write_IIC_Command(dat);
    }
}
/**************************************************/
/*              坐标设置                          */
/**************************************************/

void OLED_Set_Pos(uint8_t x, uint8_t y) {
    OLED_WR_Byte(0xb0 + y, OLED_CMD);
    OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);
    OLED_WR_Byte((x & 0x0f), OLED_CMD);
}

/**************************************************/
/*              开启OLED显示                      */
/**************************************************/

void OLED_Display_On(void) {
    OLED_WR_Byte(0X8D, OLED_CMD);  //SET DCDC命令
    OLED_WR_Byte(0X14, OLED_CMD);  //DCDC ON
    OLED_WR_Byte(0XAF, OLED_CMD);  //DISPLAY ON
}

/**************************************************/
/*              关闭OLED显示                      */
/**************************************************/

//void OLED_Display_Off(void)
//{
//	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
//	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
//	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
//}		   			 

/**************************************************/
/*               OLED清屏                         */
/**************************************************/

//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void) {
    uint8_t i, n;
    for (i = 0; i < 8; i++) {
        OLED_WR_Byte(0xb0 + i, OLED_CMD);    //设置页地址（0~7）
        OLED_WR_Byte(0x00, OLED_CMD);      //设置显示位置—列低地址
        OLED_WR_Byte(0x10, OLED_CMD);      //设置显示位置—列高地址
        for (n = 0; n < 128; n++)OLED_WR_Byte(0, OLED_DATA);
    } //更新显示
}

/**************************************************/
/*               OLED开显示                       */
/**************************************************/

//void OLED_On(void)  
//{  
//	ur i,n;		    
//	for(i=0;i<8;i++)  
//	{  
//		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
//		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
//		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
//		for(n=0;n<128;n++)OLED_WR_Byte(1,OLED_DATA); 
//	} //更新显示
//}

/**************************************************/
/*           OLED写一个字符                       */
/*           在指定位置显示一个字符,包括部分字符  */
/*           x:0~127                              */
/*           y:0~63                               */
/*           mode:0,反白显示;1,正常显示           */
/*           size:选择字体 16/12                  */
/**************************************************/
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size) {
    uint8_t c = 0, i = 0;
    c = chr - ' ';                    //得到偏移后的值
    if (x > Max_Column - 1) {
        x = 0;
        y = y + 2;
    }
    if (Char_Size == 16) {
        OLED_Set_Pos(x, y);
        for (i = 0; i < 8; i++)
            OLED_WR_Byte(F8X16[c * 16 + i], OLED_DATA);
        OLED_Set_Pos(x, y + 1);
        for (i = 0; i < 8; i++)
            OLED_WR_Byte(F8X16[c * 16 + i + 8], OLED_DATA);
    } else {
        OLED_Set_Pos(x, y);
        for (i = 0; i < 6; i++)
            OLED_WR_Byte(F6x8[c][i], OLED_DATA);

    }
}

/**************************************************/
/*        函数名：m^n函数                         */
/**************************************************/
uint16_t oled_pow(uint8_t m, uint8_t n) {
    uint16_t result = 1;
    while (n--) {
        result *= m;
    }
    return result;
}

/**************************************************/
/*         显示2个数字                            */
/*         x,y :起点坐标                          */
/*         len :数字的位数                        */
/*         size:字体大小                          */
/*         mode:模式	0,填充模式;1,叠加模式     */
/*         num:数值(0~4294967295);                */
/**************************************************/
void OLED_ShowNum(uint8_t x, uint8_t y, uint16_t num, uint8_t wei, uint8_t size2) {
    uint8_t t, temp;
    uint8_t enshow = 0;
    for (t = 0; t < wei; t++) {
        temp = (num / oled_pow(10, wei - t - 1)) % 10;
        if (enshow == 0 && t < (wei - 1)) {
            if (temp == 0) {
                OLED_ShowChar(x + (size2 / 2) * t, y, ' ', size2);
                continue;
            } else
                enshow = 1;
        }
        OLED_ShowChar(x + (size2 / 2) * t, y, temp + '0', size2);
    }
}

/**************************************************/
/*        函数名：显示一个字符号串                */
/**************************************************/
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr, uint8_t Char_Size) {
    uint8_t j = 0;
    while (chr[j] != '\0') {
        OLED_ShowChar(x, y, chr[j], Char_Size);
        x += 8;
        if (x > 120) {
            x = 0;
            y += 2;
        }
        j++;
    }
}

/**************************************************/
/*          函数名：显示汉字                      */
/**************************************************/
//void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no)
//{
//    uint8_t t,adder=0;
//    OLED_Set_Pos(x,y);    //显示坐标位置
//    for(t=0;t<16;t++)
//    {
//        OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
//        adder+=1;
//    }
//    OLED_Set_Pos(x,y+1);   //坐标
//    for(t=0;t<16;t++)
//    {
//        OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
//        adder+=1;
//    }
//}
/***********显示图片*****************/
void OLED_DrawBMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t BMP[]) {
    unsigned int j = 0;
    uint8_t x, y;

    if (y1 % 8 == 0) y = y1 / 8;
    else y = y1 / 8 + 1;
    for (y = y0; y < y1; y++) {
        OLED_Set_Pos(x0, y);
        for (x = x0; x < x1; x++) {
            OLED_WR_Byte(BMP[j++], OLED_DATA);
        }
    }
}
/**************************************************/
/*           函数名：初始化OLED                   */
/**************************************************/
void OLED_Init(void) {
    OLED_WR_Byte(0xAE, OLED_CMD);//关显示
    OLED_WR_Byte(0x00, OLED_CMD);//---set low column address
    OLED_WR_Byte(0x10, OLED_CMD);//---set high column address
    OLED_WR_Byte(0x40, OLED_CMD);//--set start line address
    OLED_WR_Byte(0xB0, OLED_CMD);//--set page address
    OLED_WR_Byte(0x81, OLED_CMD); // contract control
    OLED_WR_Byte(0xFF, OLED_CMD);//--128
    OLED_WR_Byte(0xA1, OLED_CMD);//set segment remap
    OLED_WR_Byte(0xA6, OLED_CMD);//--normal / reverse
    OLED_WR_Byte(0xA8, OLED_CMD);//--set multiplex ratio(1 to 64)
    OLED_WR_Byte(0x3F, OLED_CMD);//--1/32 duty
    OLED_WR_Byte(0xC8, OLED_CMD);//Com scan direction
    OLED_WR_Byte(0xD3, OLED_CMD);//-set display offset
    OLED_WR_Byte(0x00, OLED_CMD);//
    OLED_WR_Byte(0xD5, OLED_CMD);//set osc division
    OLED_WR_Byte(0x80, OLED_CMD);//

    OLED_WR_Byte(0xD8, OLED_CMD);//set area color mode off
    OLED_WR_Byte(0x05, OLED_CMD);//

    OLED_WR_Byte(0xD9, OLED_CMD);//Set Pre-Charge Period
    OLED_WR_Byte(0xF1, OLED_CMD);//

    OLED_WR_Byte(0xDA, OLED_CMD);//set com pin configuartion
    OLED_WR_Byte(0x12, OLED_CMD);//

    OLED_WR_Byte(0xDB, OLED_CMD);//set Vcomh
    OLED_WR_Byte(0x30, OLED_CMD);//

    OLED_WR_Byte(0x8D, OLED_CMD);//set charge pump enable
    OLED_WR_Byte(0x14, OLED_CMD);//

    OLED_WR_Byte(0xAF, OLED_CMD);//--turn on oled panel

    OLED_Display_On();
    OLED_Clear();
}


#endif
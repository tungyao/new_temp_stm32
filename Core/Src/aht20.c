#include "main.h"
#include "gpio.h"
// 定义模拟I2C的GPIO口和定时器
#define SDA_PIN 7
#define SDA_GPIO GPIOB
#define SDA_RCC RCC_APB2Periph_GPIOB

#define SCL_PIN 8
#define SCL_GPIO GPIOB
#define SCL_RCC RCC_APB2Periph_GPIOB

#define I2C_DELAY_VAL 100 // 模拟I2C传输时的延迟时间

void delay(uint32_t ms_delay) {
    uint32_t i, j;
    for (i = 0; i < ms_delay; i++) {
        for (j = 0; j < 720000; j++) { // 循环720000次大约需要1ms左右的时间
            __NOP(); // 空指令，用于消耗一定的CPU周期
        }
    }
}

// 初始化模拟I2C引脚和时钟
void I2C_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

//    (SDA_RCC | SCL_RCC, ENABLE);

    GPIO_InitStructure.Pin = SDA_PIN;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
    HAL_GPIO_Init(SDA_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = SCL_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
    HAL_GPIO_Init(SCL_GPIO, &GPIO_InitStructure);
}

// 模拟I2C的起始信号
void I2C_Start(void) {
    HAL_GPIO_WritePin(temp_sda_GPIO_Port, temp_sda_Pin, 1);
    HAL_GPIO_WritePin(temp_scl_GPIO_Port, temp_scl_Pin, 1);
    delay(I2C_DELAY_VAL);

    HAL_GPIO_WritePin(temp_sda_GPIO_Port, temp_sda_Pin, 0);
    delay(I2C_DELAY_VAL);
    HAL_GPIO_WritePin(temp_scl_GPIO_Port, temp_scl_Pin, 0);
}

// 模拟I2C的停止信号
void I2C_Stop(void) {
    HAL_GPIO_WritePin(temp_sda_GPIO_Port, temp_sda_Pin, 0);
    delay(I2C_DELAY_VAL);
    HAL_GPIO_WritePin(temp_scl_GPIO_Port, temp_scl_Pin, 1);
    delay(I2C_DELAY_VAL);
    HAL_GPIO_WritePin(temp_sda_GPIO_Port, temp_sda_Pin, 1);
}

// 模拟I2C的发送数据
void I2C_WriteByte(uint8_t data) {
    uint8_t i;
    for (i = 0; i < 8; i++) {
        if (data & 0x80) {
            HAL_GPIO_WritePin(temp_sda_GPIO_Port, temp_sda_Pin, 1);
        } else {
            HAL_GPIO_WritePin(temp_sda_GPIO_Port, temp_sda_Pin, 0);
        }
        delay(I2C_DELAY_VAL);
        HAL_GPIO_WritePin(temp_scl_GPIO_Port, temp_scl_Pin, 1);
        delay(I2C_DELAY_VAL);
        HAL_GPIO_WritePin(temp_scl_GPIO_Port, temp_scl_Pin, 0);
        data <<= 1;
    }
}

// 模拟I2C的接收数据
uint8_t I2C_ReadByte(void) {
    uint8_t i, data = 0;
    HAL_GPIO_WritePin(temp_sda_GPIO_Port, temp_sda_Pin, 1);
    for (i = 0; i < 8; i++) {
        data <<= 1;
        HAL_GPIO_WritePin(temp_scl_GPIO_Port, temp_scl_Pin, 1);
        delay(I2C_DELAY_VAL);
        if (HAL_GPIO_ReadPin(temp_sda_GPIO_Port, temp_sda_Pin)) {
            data |= 0x01;
        }
        HAL_GPIO_WritePin(temp_scl_GPIO_Port, temp_scl_Pin, 0);
        delay(I2C_DELAY_VAL);
    }
    return data;
}

// 向AHT20写入命令
void AHT20_WriteCommand(uint8_t cmd) {
    I2C_Start();
    I2C_WriteByte(0x38); // AHT20的I2C地址为0x38
    I2C_WriteByte(cmd);
    I2C_Stop();
}

// 读取AHT20的数据
void AHT20_ReadData(uint16_t *temp, uint16_t *humi) {
    uint16_t data[6];
    uint8_t i;

    I2C_Start();
    I2C_WriteByte(0x39); // AHT20的I2C地址为0x38（写命令）和0x39（读命令）
    for (i = 0; i < 6; i++) {
        data[i] = I2C_ReadByte();
        if (i < 5) {
            I2C_WriteByte(0x00);
        }
    }
    I2C_Stop();

    uint16_t _humi = data[1];
    _humi <<= 8;
    _humi += data[2];
    _humi <<= 4;
    _humi += data[3] >> 4;

    *humi = _humi;

    uint16_t _temp = data[3] & 0x0f;
    _temp <<= 8;
    _temp += data[4];
    _temp <<= 8;
    _temp += data[5];

    *temp = _temp;
}
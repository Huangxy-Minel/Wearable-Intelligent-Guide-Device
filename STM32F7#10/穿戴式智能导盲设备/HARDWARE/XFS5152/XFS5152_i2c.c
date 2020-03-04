#include "XFS5152_i2c.h"
#include "delay.h"

#define T_HALF_CYCLE  35  // 半个SCLK周期: 35us
#define T_HOLD_TIME   15  // Data Hold Time
#define T_START_HOLD  18  // Start Condition Hold Time: 18us
#define T_STOP_HOLD   20 // Stop Condition Setup Time: 18us
#define T_IDLE        35  // Idle Time before a New Transmission can Start: 35us



void I2C_Init(Pin_Struct *scl, Pin_Struct *sda)
{
    // Init scl pin
    HAL_GPIO_WritePin(scl->Group, scl->Num, GPIO_PIN_SET);
    GPIO_InitTypeDef GPIO_InitStruct = {
        scl->Num, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_LOW
    };
		
    HAL_GPIO_Init(scl->Group, &GPIO_InitStruct);  
  
    // Init sda pin
    HAL_GPIO_WritePin(sda->Group, sda->Num, GPIO_PIN_SET);
    GPIO_InitStruct.Pin = sda->Num;
    HAL_GPIO_Init(sda->Group, &GPIO_InitStruct);    
    
    delay_us(T_HALF_CYCLE);
}

void I2C_Start(Pin_Struct *scl, Pin_Struct *sda)
{
    HAL_GPIO_WritePin(sda->Group, sda->Num, GPIO_PIN_RESET); // sda low
    delay_us(T_START_HOLD); // t_HSTART
    
    HAL_GPIO_WritePin(scl->Group, scl->Num, GPIO_PIN_RESET); // scl low  
    delay_us(T_START_HOLD); // delay
}

void I2C_Stop(Pin_Struct *scl, Pin_Struct *sda)
{ 
    HAL_GPIO_WritePin(sda->Group, sda->Num, GPIO_PIN_RESET);
    delay_us(T_HOLD_TIME);
    
    HAL_GPIO_WritePin(scl->Group, scl->Num, GPIO_PIN_SET);
    delay_us(T_STOP_HOLD);
    
    HAL_GPIO_WritePin(sda->Group, sda->Num, GPIO_PIN_SET);
    delay_us(T_IDLE);
}

// 写一个字节，函数内部已包括ACK检测, 写入成功返回0，失败返回1
GPIO_PinState WriteByte(Pin_Struct *scl, Pin_Struct *sda, uint8_t data)
{
    for (uint8_t i = 0x80; i > 0; i >>= 1) {
        HAL_GPIO_WritePin(sda->Group, sda->Num, (data & i ? GPIO_PIN_SET : GPIO_PIN_RESET));
        delay_us(T_HALF_CYCLE-T_HOLD_TIME); //delay

        HAL_GPIO_WritePin(scl->Group, scl->Num, GPIO_PIN_SET);
        delay_us(T_HALF_CYCLE); //delay

        HAL_GPIO_WritePin(scl->Group, scl->Num, GPIO_PIN_RESET);
        delay_us(T_HOLD_TIME);
    }
    
    // wait ACK
    
    // Set SDA Input Mode
    GPIO_InitTypeDef GPIO_InitStruct = {
        sda->Num, GPIO_MODE_INPUT, GPIO_PULLUP, GPIO_SPEED_LOW
    };
    HAL_GPIO_Init(sda->Group, &GPIO_InitStruct); 
    delay_us(T_HALF_CYCLE); //delay
    
    HAL_GPIO_WritePin(scl->Group, scl->Num, GPIO_PIN_SET);  // scl high 
    delay_us(T_HOLD_TIME); //delay
    
    GPIO_PinState sda_state = HAL_GPIO_ReadPin(sda->Group, sda->Num);  // Read SDA
    delay_us(T_HALF_CYCLE - T_HOLD_TIME);
       
    HAL_GPIO_WritePin(scl->Group, scl->Num, GPIO_PIN_RESET);  // scl low
    delay_us(T_HOLD_TIME);
    
    // Reset sda to output mode
    HAL_GPIO_WritePin(sda->Group, sda->Num, GPIO_PIN_SET);  // sda high
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(sda->Group, &GPIO_InitStruct);

    return sda_state;
}


uint8_t I2C_SendData(Pin_Struct *scl, Pin_Struct *sda, uint8_t address, uint8_t data)
{
    I2C_Start(scl, sda);          // 开始
    if(WriteByte(scl, sda, address)) return 0; // 写地址
    
    delay_us(T_HALF_CYCLE*2);
    
    if(WriteByte(scl, sda, data)) return 0;    // 写数据
    I2C_Stop(scl, sda);           // 结束
    return 1;
}


uint16_t I2C_SendDatas(Pin_Struct *scl, Pin_Struct *sda, uint8_t address, uint8_t *data, uint16_t n)
{
    I2C_Start(scl, sda);           // 开始
    if(WriteByte(scl, sda, address)) return 0;  // 写地址
    
    // 写数据
    for (uint16_t i=0; i<n; i++) {
        if(WriteByte(scl, sda, data[i])) return i;
        delay_us(T_HALF_CYCLE*2);
    }
    
    I2C_Stop(scl, sda);             // 结束
    return n;
}

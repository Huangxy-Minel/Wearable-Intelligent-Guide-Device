#include "XFS5152.h"
#include "XFS5152_i2c.h"
#include "delay.h"
#include <stdarg.h>

// XFS5152 引脚分配
Pin_Struct scl_pin = {GPIOF, GPIO_PIN_1};  // SCL PC0
Pin_Struct sda_pin = {GPIOF, GPIO_PIN_0};  // SDA PC1
Pin_Struct rdy_pin = {GPIOF, GPIO_PIN_2};  // RDY PC1
Pin_Struct rst_pin = {GPIOF, GPIO_PIN_11}; // rst 


void XFS_Init(void)
{
    __HAL_RCC_GPIOF_CLK_ENABLE();	//开启GPIOF时钟
    I2C_Init(&scl_pin, &sda_pin); // I2C初始化
    
    // RDY 引脚初始化
    GPIO_InitTypeDef GPIO_InitStruct = {
        rdy_pin.Num, GPIO_MODE_INPUT, GPIO_PULLUP, GPIO_SPEED_LOW
    };
    HAL_GPIO_Init(rdy_pin.Group, &GPIO_InitStruct);
    
    // RST 引脚初始化
    GPIO_InitStruct.Pin = rst_pin.Num;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(rst_pin.Group, &GPIO_InitStruct);
    
	// 复位 语音合成模块
    while(!XFS_Reset());
}

uint8_t XFS_Speech(const char *format, ...)
{
		while(XFS_isBusy()){};
    va_list ap;
    va_start(ap, format);
    
    static char frame[256];
    int16_t length = vsprintf(frame+5, format, ap);
    static uint8_t state = 0;
    if(length>0) {
        length = (uint8_t)length;
        
        frame[0] = 0xFD; 			//帧头FD
        frame[1] = (length+2)>>8;   //数据区长度的高字节
        frame[2] = (length+2)&0xff; //数据区长度的低字节
        frame[3] = 0x01; 			//命令字：合成播放命令		 		 
        frame[4] = 0x00;            //文本编码格式：GB2312
        
        state =  I2C_SendDatas(&scl_pin, &sda_pin, 0x80, (uint8_t*)frame, 5+length) == 5+length ? 1:0;
    }
    return state;
}

inline uint8_t XFS_Stop(void)
{
    uint8_t StopCommand[] = {0xFD,0X00,0X01,0X02};
    return I2C_SendDatas(&scl_pin, &sda_pin, 0x80, StopCommand, 4);
}

inline uint8_t XFS_Suspend(void)
{
    uint8_t SuspendCommand[]={0XFD,0X00,0X01,0X03};
    return I2C_SendDatas(&scl_pin, &sda_pin, 0x80, SuspendCommand, 4);
}

inline uint8_t XFS_Recovery(void)
{
    uint8_t RecoverCommand[]={0XFD,0X00,0X01,0X04};
    return I2C_SendDatas(&scl_pin, &sda_pin, 0x80, RecoverCommand, 4);
}

inline uint8_t XFS_PowerDown(void)
{
    uint8_t PDCommand[]={0XFD,0X00,0X01,0X88}; 
    return I2C_SendDatas(&scl_pin, &sda_pin, 0x80, PDCommand, 4);
}

inline uint8_t XFS_WakeUp(void)
{
    uint8_t WakeUpCommand[]={0XFD,0X00,0X01,0Xff}; 
    return I2C_SendDatas(&scl_pin, &sda_pin, 0x80, WakeUpCommand, 4);
}

inline uint8_t XFS_isBusy(void)
{
    return HAL_GPIO_ReadPin(rdy_pin.Group, rdy_pin.Num);
}

uint8_t XFS_Reset(void)
{
    HAL_GPIO_WritePin(rst_pin.Group, rst_pin.Num, GPIO_PIN_RESET);
    delay_ms(500);
    HAL_GPIO_WritePin(rst_pin.Group, rst_pin.Num, GPIO_PIN_SET);
    delay_ms(500);
    return !XFS_isBusy();
}

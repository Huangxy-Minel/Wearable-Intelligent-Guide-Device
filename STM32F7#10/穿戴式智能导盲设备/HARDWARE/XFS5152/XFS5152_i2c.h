#ifndef _IIC_H_
#define _IIC_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f7xx_hal.h"

/**
 * @brief 封装引脚的组和编号的结构体； 
 */ 
typedef struct
{
    GPIO_TypeDef *Group;
    uint16_t Num;
} Pin_Struct;


/**
 * @brief IIC通信初始化，必需在其他I2C操作函数之前调用
 * 
 * @param scl      SCL引脚结构体指针
 * @param sda      SDA引脚结构体指针
 */
void I2C_Init(Pin_Struct *scl, Pin_Struct *sda);


/**
 * @brief 向IIC设备写入一个字节的数据
 * 
 * @param scl      SCL引脚结构体指针
 * @param sda      SDA引脚结构体指针
 * @param address  从机地址
 * @param data     要写入的数据
 *
 * @return 成功返回1 失败返回0
 */
uint8_t I2C_SendData(Pin_Struct *scl, Pin_Struct *sda, uint8_t address, uint8_t data);


/**
 * @brief 向IIC设备写入n个字节的数据
 * 
 * @param scl      SCL引脚结构体指针
 * @param sda      SDA引脚结构体指针
 * @param address  从机地址
 * @param data     要写入的数据数组的指针
 * @param n        要写入的数据个数
 *
 * @return 成功发送的字节数
 */
uint16_t I2C_SendDatas(Pin_Struct *scl, Pin_Struct *sda, uint8_t address, uint8_t *data, uint16_t n);


#ifdef __cplusplus
}
#endif
#endif

#ifndef _IIC_H_
#define _IIC_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f7xx_hal.h"

/**
 * @brief ��װ���ŵ���ͱ�ŵĽṹ�壻 
 */ 
typedef struct
{
    GPIO_TypeDef *Group;
    uint16_t Num;
} Pin_Struct;


/**
 * @brief IICͨ�ų�ʼ��������������I2C��������֮ǰ����
 * 
 * @param scl      SCL���Žṹ��ָ��
 * @param sda      SDA���Žṹ��ָ��
 */
void I2C_Init(Pin_Struct *scl, Pin_Struct *sda);


/**
 * @brief ��IIC�豸д��һ���ֽڵ�����
 * 
 * @param scl      SCL���Žṹ��ָ��
 * @param sda      SDA���Žṹ��ָ��
 * @param address  �ӻ���ַ
 * @param data     Ҫд�������
 *
 * @return �ɹ�����1 ʧ�ܷ���0
 */
uint8_t I2C_SendData(Pin_Struct *scl, Pin_Struct *sda, uint8_t address, uint8_t data);


/**
 * @brief ��IIC�豸д��n���ֽڵ�����
 * 
 * @param scl      SCL���Žṹ��ָ��
 * @param sda      SDA���Žṹ��ָ��
 * @param address  �ӻ���ַ
 * @param data     Ҫд������������ָ��
 * @param n        Ҫд������ݸ���
 *
 * @return �ɹ����͵��ֽ���
 */
uint16_t I2C_SendDatas(Pin_Struct *scl, Pin_Struct *sda, uint8_t address, uint8_t *data, uint16_t n);


#ifdef __cplusplus
}
#endif
#endif

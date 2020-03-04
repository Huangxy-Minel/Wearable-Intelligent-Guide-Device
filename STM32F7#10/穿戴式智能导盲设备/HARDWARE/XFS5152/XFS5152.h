#ifndef _XFS5152_H_
#define _XFS5152_H_

#include "stm32f7xx_hal.h"

#ifdef __cplusplus
 extern "C" {
#endif


/**
 * @brief 语音合成模块初始化函数，该函数必须在其他语音相关函数之前被调用；
 */
void XFS_Init(void);


/**
 * @brief 语音合成函数
 * 
 * @param data: 待合成的文字，编码为GB2312，最大长度为4092（一个中文字符大小为2）
 *
 * @return 失败返回0，成功返回1
 */
uint8_t XFS_Speech(const char *format, ...) __attribute__((format(printf, 1, 2)));


/**
 * @brief 状态查询函数
 *
 * @return 空闲返回0，忙碌返回1
 */
uint8_t XFS_isBusy(void);


/**
 * @brief 停止当前语音合成
 *
 * @return 失败返回0，成功返回1
 */
uint8_t XFS_Stop(void);


/**
 * @brief 暂停当前语音合成
 *
 * @return 失败返回0，成功返回1
 */
uint8_t XFS_Suspend(void);


/**
 * @brief 恢复之前暂停的语音合成
 *
 * @return 失败返回0，成功返回1
 */
uint8_t XFS_Recovery(void);


/**
 * @brief 进入省电模式
 *
 * @return 失败返回0，成功返回1
 */
uint8_t XFS_PowerDown(void);


/**
 * @brief 将设备从省电模式唤醒
 *
 * @return 失败返回0，成功返回1
 */
uint8_t XFS_WakeUp(void);


/**
* @brief 复位语音合成
 *
 * @return 失败返回0，成功返回1
 */
uint8_t XFS_Reset(void);


#ifdef __cplusplus
}
#endif
#endif

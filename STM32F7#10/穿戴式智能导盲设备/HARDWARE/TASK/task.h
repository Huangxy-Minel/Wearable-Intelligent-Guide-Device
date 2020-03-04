#ifndef _TASK_H
#define _TASK_H
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "lcd.h"
#include "led.h"
#include "power.h"
#include "wt931.h"
#include "exti.h"
#include "adc.h"
#include "spi.h"
#include "XFS5152.h"

#define MAX_ROOM 3
#define MAX_OBJECT 2
#define MAX_FACE 3
#define OPENMVCODE1 111  //用于串口1访问地址
#define OPENMVCODE2 222  //用于串口1访问地址
#define STM32F1CODE 333 //用于串口1访问地址
void InitTask(void);
u8 Get_ImageMsg(u8 flag, u8* buf, u8 openmvaddr);
void VoiceTask(u16 voice_msg_code,u8 message);
void GpsTask(void);
float fix_g(void);
void SoundTask(void);
u8 GetVoiceMsg(void);
void ChargeTask(void);

#endif

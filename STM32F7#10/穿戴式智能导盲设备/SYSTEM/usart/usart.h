#ifndef _USART_H
#define _USART_H
#include "sys.h"
#include "stdio.h"	
#include "stdarg.h"	 	 	 	 
#include "string.h"	
#include "timer.h"
//////////////////////////////////////////////////////////////////////////////////	 
//±¾³ÌĞòÖ»¹©Ñ§Ï°Ê¹ÓÃ£¬Î´¾­×÷ÕßĞí¿É£¬²»µÃÓÃÓÚÆäËüÈÎºÎÓÃÍ¾
//ALIENTEK STM32F7¿ª·¢°å
//´®¿Ú1³õÊ¼»¯		   
//ÕıµãÔ­×Ó@ALIENTEK
//¼¼ÊõÂÛÌ³:www.openedv.csom
//ĞŞ¸ÄÈÕÆÚ:2015/6/23
//°æ±¾£ºV1.0
//°æÈ¨ËùÓĞ£¬µÁ°æ±Ø¾¿¡£
//Copyright(C) ÕıµãÔ­×Ó 2009-2019
//All rights reserved
//********************************************************************************
//V1.0ĞŞ¸ÄËµÃ÷ 
////////////////////////////////////////////////////////////////////////////////// 	
#define USART_REC_LEN  			200  	//¶¨Òå×î´ó½ÓÊÕ×Ö½ÚÊı 200
#define EN_USART_RX 			1		//Ê¹ÄÜ£¨1£©/½ûÖ¹£¨0£©´®¿Ú1½ÓÊÕ
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //½ÓÊÕ»º³å,×î´óUSART_REC_LEN¸ö×Ö½Ú.Ä©×Ö½ÚÎª»»ĞĞ·û 
extern u16 USART_RX_STA;         		//½ÓÊÕ×´Ì¬±ê¼Ç	
extern u16 UART5_RX_STA;         		//½ÓÊÕ×´Ì¬±ê¼Ç	
extern u16 UART4_RX_STA;         		//½ÓÊÕ×´Ì¬±ê¼Ç

extern UART_HandleTypeDef UART1_Handler; //UART¾ä±ú
extern UART_HandleTypeDef UART4_Handler; //UART¾ä±ú
extern UART_HandleTypeDef UART5_Handler; //UART¾ä±ú




#define RXBUFFERSIZE   1 //»º´æ´óĞ¡
extern u8 aRxBuffer[RXBUFFERSIZE];//HAL¿âUSART½ÓÊÕBuffer

//Èç¹ûÏë´®¿ÚÖĞ¶Ï½ÓÊÕ£¬Çë²»Òª×¢ÊÍÒÔÏÂºê¶¨Òå
void uart_init(u32 bound);


//´®¿Ú3Ïà¹Øºê¶¨Òå
#define USART3_MAX_RECV_LEN		800					//×î´ó½ÓÊÕ»º´æ×Ö½ÚÊı
#define USART3_MAX_SEND_LEN		800					//×î´ó·¢ËÍ»º´æ×Ö½ÚÊı
#define USART3_RX_EN 			1					//0,²»½ÓÊÕ;1,½ÓÊÕ.
#define EN_USART1_RX 1
#define EN_UART5_RX 			1		//Ê¹ÄÜ£¨1£©/½ûÖ¹£¨0£©´®¿Ú1½ÓÊÕ
#define EN_UART4_RX 			1		//Ê¹ÄÜ£¨1£©/½ûÖ¹£¨0£©´®¿Ú1½ÓÊÕ

extern u8  USART3_RX_BUF[USART3_MAX_RECV_LEN]; 		//½ÓÊÕ»º³å,×î´óUSART3_MAX_RECV_LEN×Ö½Ú
extern u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 		//·¢ËÍ»º³å,×î´óUSART3_MAX_SEND_LEN×Ö½Ú
extern u16 USART3_RX_STA;   						//½ÓÊÕÊı¾İ×´Ì
extern u8 UART4_RX_BUF[USART_REC_LEN];
extern u8 UART5_RX_BUF[USART_REC_LEN];

void uart4_init(u32 bound);
void usart3_init(u32 bound);				//´®¿Ú3³õÊ¼»¯ 
void TIM7_Int_Init(u16 arr,u16 psc);
void u3_printf(char* fmt, ...);

void uart5_init(u32 bound);
#endif

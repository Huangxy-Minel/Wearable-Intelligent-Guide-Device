#ifndef _USART_H
#define _USART_H
#include "sys.h"
#include "stdio.h"	
#include "stdarg.h"	 	 	 	 
#include "string.h"	
#include "timer.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F7������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.csom
//�޸�����:2015/6/23
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
//********************************************************************************
//V1.0�޸�˵�� 
////////////////////////////////////////////////////////////////////////////////// 	
#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	
extern u16 UART5_RX_STA;         		//����״̬���	
extern u16 UART4_RX_STA;         		//����״̬���

extern UART_HandleTypeDef UART1_Handler; //UART���
extern UART_HandleTypeDef UART4_Handler; //UART���
extern UART_HandleTypeDef UART5_Handler; //UART���




#define RXBUFFERSIZE   1 //�����С
extern u8 aRxBuffer[RXBUFFERSIZE];//HAL��USART����Buffer

//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 bound);


//����3��غ궨��
#define USART3_MAX_RECV_LEN		800					//�����ջ����ֽ���
#define USART3_MAX_SEND_LEN		800					//����ͻ����ֽ���
#define USART3_RX_EN 			1					//0,������;1,����.
#define EN_USART1_RX 1
#define EN_UART5_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
#define EN_UART4_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����

extern u8  USART3_RX_BUF[USART3_MAX_RECV_LEN]; 		//���ջ���,���USART3_MAX_RECV_LEN�ֽ�
extern u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 		//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
extern u16 USART3_RX_STA;   						//��������״�
extern u8 UART4_RX_BUF[USART_REC_LEN];
extern u8 UART5_RX_BUF[USART_REC_LEN];

void uart4_init(u32 bound);
void usart3_init(u32 bound);				//����3��ʼ�� 
void TIM7_Int_Init(u16 arr,u16 psc);
void u3_printf(char* fmt, ...);

void uart5_init(u32 bound);
#endif

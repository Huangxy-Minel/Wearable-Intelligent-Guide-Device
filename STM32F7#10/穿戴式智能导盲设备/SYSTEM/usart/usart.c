#include "usart.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��os,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//os ʹ��	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F7������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2015/6/23
//�汾��V1.5
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.0�޸�˵�� 
////////////////////////////////////////////////////////////////////////////////// 	  
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
//#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)	
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((UART4->ISR&0X40)==0);//ѭ������,ֱ���������   
	UART4->TDR=(u8)ch;      
	return ch;
}
#endif 

#if EN_USART_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u8 UART5_RX_BUF[USART_REC_LEN];
u8 UART4_RX_BUF[USART_REC_LEN];
//���ڷ��ͻ����� 	
__align(8) u8 USART3_TX_BUF[USART3_MAX_SEND_LEN]; 	//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�  	  
//���ڽ��ջ����� 	
u8 USART3_RX_BUF[USART3_MAX_RECV_LEN]; 				//���ջ���,���USART3_MAX_RECV_LEN���ֽ�.

//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	
u16 USART3_RX_STA=0; 
u16 UART5_RX_STA=0;       //����״̬���
u16 UART4_RX_STA = 0;

u8 UART5_COUNT = 100;
u8 UART5_START = 0;

u8 aRxBuffer[RXBUFFERSIZE];//HAL��ʹ�õĴ��ڽ��ջ���
UART_HandleTypeDef UART1_Handler; //UART���
UART_HandleTypeDef UART3_Handler; //UART���
UART_HandleTypeDef UART4_Handler; //UART���
UART_HandleTypeDef UART5_Handler; //UART���

//��ʼ��IO ����1 
//bound:������
void uart_init(u32 bound)
{	
	//UART ��ʼ������
	UART1_Handler.Instance=USART1;					    //USART1
	UART1_Handler.Init.BaudRate=bound;				    //������
	UART1_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	UART1_Handler.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
	UART1_Handler.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	UART1_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //��Ӳ������
	UART1_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	HAL_UART_Init(&UART1_Handler);					    //HAL_UART_Init()��ʹ��UART1
	
	//HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE);//�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������
  
}
 //��ʼ��IO ����3 
//bound:������
void usart3_init(u32 bound)
{	
	//UART ��ʼ������
	UART3_Handler.Instance=USART3;					    //USART3
	UART3_Handler.Init.BaudRate=bound;				    //������
	UART3_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	UART3_Handler.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
	UART3_Handler.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	UART3_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //��Ӳ������
	UART3_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	HAL_UART_Init(&UART3_Handler);					    //HAL_UART_Init()��ʹ��UART1
}
//UART�ײ��ʼ����ʱ��ʹ�ܣ��������ã��ж�����
//�˺����ᱻHAL_UART_Init()����
//huart:���ھ��

//��ʼ��IO ����4
//bound:������
void uart4_init(u32 bound)
{	
	//UART ��ʼ������
	UART4_Handler.Instance=UART4;					    //UART5
	UART4_Handler.Init.BaudRate=bound;				    //������
	UART4_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	UART4_Handler.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
	UART4_Handler.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	UART4_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //��Ӳ������
	UART4_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	HAL_UART_Init(&UART4_Handler);					    //HAL_UART_Init()��ʹ��UART5
	
	HAL_UART_Receive_IT(&UART4_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE);//�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������
  
}

//��ʼ��IO ����5 
//bound:������
void uart5_init(u32 bound)
{	
	//UART ��ʼ������
	UART5_Handler.Instance=UART5;					    //UART5
	UART5_Handler.Init.BaudRate=bound;				    //������
	UART5_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	UART5_Handler.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
	UART5_Handler.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	UART5_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //��Ӳ������
	UART5_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	HAL_UART_Init(&UART5_Handler);					    //HAL_UART_Init()��ʹ��UART5
	
	HAL_UART_Receive_IT(&UART5_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE);//�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������
  
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIO�˿�����
	GPIO_InitTypeDef GPIO_Initure;
	
	if(huart->Instance==USART1)//����Ǵ���1�����д���1 MSP��ʼ��
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();			//ʹ��GPIOAʱ��
		__HAL_RCC_USART1_CLK_ENABLE();			//ʹ��USART1ʱ��
	
		GPIO_Initure.Pin=GPIO_PIN_9;			//PA9
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�����������
		GPIO_Initure.Pull=GPIO_PULLUP;			//����
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		//����
		GPIO_Initure.Alternate=GPIO_AF7_USART1;	//����ΪUSART1
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��PA9

		GPIO_Initure.Pin=GPIO_PIN_10;			//PA10
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��PA10
		
		__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);		//���������ж�
		HAL_NVIC_EnableIRQ(USART1_IRQn);				//ʹ��USART1�ж�ͨ��
		HAL_NVIC_SetPriority(USART1_IRQn,2,2);			//��ռ���ȼ�3�������ȼ�3	
	}
	if(huart==(&UART3_Handler))
	{
		  //GPIO�˿�����
		GPIO_InitTypeDef GPIO_Initure;
	
		__HAL_RCC_GPIOB_CLK_ENABLE();			//ʹ��GPIOBʱ��
		__HAL_RCC_USART3_CLK_ENABLE();			//ʹ��USART3ʱ��
	
		GPIO_Initure.Pin=GPIO_PIN_10;			//PB10
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�����������
		GPIO_Initure.Pull=GPIO_PULLUP;			//����
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		//����
		GPIO_Initure.Alternate=GPIO_AF7_USART3;	//����ΪUSART3
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	//��ʼ��PB10

		GPIO_Initure.Pin=GPIO_PIN_11;			//PB11
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	//��ʼ��PB11
	
//		__HAL_UART_DISABLE_IT(huart,UART_IT_TC);
		__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);		//���������ж�
		HAL_NVIC_EnableIRQ(USART3_IRQn);				//ʹ��USART3�ж�
		HAL_NVIC_SetPriority(USART3_IRQn,2,3);			//��ռ���ȼ�2�������ȼ�3	
		TIM7_Int_Init(1000-1,9000-1);		//100ms�ж�
		USART3_RX_STA=0;		//����
		TIM7->CR1&=~(1<<0);        //�رն�ʱ��7
	}
	
	if(huart->Instance==UART4)//����Ǵ���5�����д���5 MSP��ʼ��
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();			//ʹ��GPIOAʱ��
		__HAL_RCC_UART4_CLK_ENABLE();			//ʹ��UART4ʱ��
	
		GPIO_Initure.Pin=GPIO_PIN_11;			//PB12
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�����������
		GPIO_Initure.Pull=GPIO_PULLUP;			//����
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_LOW;		//����
		GPIO_Initure.Alternate=GPIO_AF6_UART4;	//����ΪUART5
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��PB12

		GPIO_Initure.Pin=GPIO_PIN_12;			//PB13
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��PB13
		
#if EN_UART4_RX
		HAL_NVIC_EnableIRQ(UART4_IRQn);				//ʹ��UART5�ж�ͨ��
		HAL_NVIC_SetPriority(UART4_IRQn,3,3);			//��ռ���ȼ�3�������ȼ�3
#endif	
	}
	
	if(huart->Instance==UART5)//����Ǵ���5�����д���5 MSP��ʼ��
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();			//ʹ��GPIOBʱ��
		__HAL_RCC_UART5_CLK_ENABLE();			//ʹ��UART5ʱ��
	
		GPIO_Initure.Pin=GPIO_PIN_12;			//PB12
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�����������
		GPIO_Initure.Pull=GPIO_PULLUP;			//����
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		//����
		GPIO_Initure.Alternate=GPIO_AF8_UART5;	//����ΪUART5
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	//��ʼ��PB12

		GPIO_Initure.Pin=GPIO_PIN_13;			//PB13
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	//��ʼ��PB13
		
#if EN_UART5_RX
		HAL_NVIC_EnableIRQ(UART5_IRQn);				//ʹ��UART5�ж�ͨ��
		HAL_NVIC_SetPriority(UART5_IRQn,3,3);			//��ռ���ȼ�3�������ȼ�3
#endif	
	}
	

}

//����3,printf ����
//ȷ��һ�η������ݲ�����USART3_MAX_SEND_LEN�ֽ�
void u3_printf(char* fmt,...)  
{  
	u16 i,j; 
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char*)USART3_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART3_TX_BUF);		//�˴η������ݵĳ���
	for(j=0;j<i;j++)							//ѭ����������
	{
		while((USART3->ISR&0X40)==0);			//ѭ������,ֱ���������   
		USART3->TDR=USART3_TX_BUF[j];  
	} 
}


void USART3_IRQHandler(void)
{
	u8 res;	      
	if(__HAL_UART_GET_FLAG(&UART3_Handler,UART_FLAG_RXNE)!=RESET)//���յ�����
	{	 
//		HAL_UART_Receive(&UART3_Handler,&res,1,1000);
		res=USART3->RDR; 			 
		if((USART3_RX_STA&(1<<15))==0)//�������һ������,��û�б�����,���ٽ�����������
		{ 
			if(USART3_RX_STA<USART3_MAX_RECV_LEN)	//�����Խ�������
			{
//				__HAL_TIM_SetCounter(&TIM7_Handler,0);	
				TIM7->CNT=0;         				//���������	
				if(USART3_RX_STA==0) 				//ʹ�ܶ�ʱ��7���ж� 
				{
//					__HAL_RCC_TIM7_CLK_ENABLE();            //ʹ��TIM7ʱ��
					TIM7->CR1|=1<<0;     			//ʹ�ܶ�ʱ��7
				}
				USART3_RX_BUF[USART3_RX_STA++]=res;	//��¼���յ���ֵ	 
			}else 
			{
				USART3_RX_STA|=1<<15;				//ǿ�Ʊ�ǽ������
			} 
		}
	}  				 											 
}   
 



//����1�жϷ������
void USART1_IRQHandler(void)                	
{ 
	u8 Res;
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntEnter();    
#endif
	if((__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_RXNE)!=RESET))  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
        HAL_UART_Receive(&UART1_Handler,&Res,1,1000); 
		if((USART_RX_STA&0x8000)==0)//����δ���
		{
			if(USART_RX_STA&0x4000)//���յ���0x0d
			{
				if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else USART_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}   		 
	}
	HAL_UART_IRQHandler(&UART1_Handler);	
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntExit();  											 
#endif
} 


//����4�жϷ������
void UART4_IRQHandler(void)                	
{ 
	u32 timeout=0;
	u32 maxDelay=0x1FFFF;


	HAL_UART_IRQHandler(&UART4_Handler);	//??HAL?????????

	timeout=0;
	while (HAL_UART_GetState(&UART4_Handler)!=HAL_UART_STATE_READY)//????
	{
		timeout++;////????
		if(timeout>maxDelay) break;		
	}

	timeout=0;
	while(HAL_UART_Receive_IT(&UART4_Handler,(u8 *)aRxBuffer, RXBUFFERSIZE)!=HAL_OK)//????????,?????????RxXferCount?1
	{
		timeout++; //????
		if(timeout>maxDelay) break;	
	}
} 


//�����жϷ������
void UART5_IRQHandler(void)                	
{ 
	u32 timeout=0;
    u32 maxDelay=0x1FFFF;
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntEnter();    
#endif
	
	HAL_UART_IRQHandler(&UART5_Handler);	//����HAL���жϴ����ú���
	
	timeout=0;
    while (HAL_UART_GetState(&UART5_Handler)!=HAL_UART_STATE_READY)//�ȴ�����
	{
        timeout++;////��ʱ����
        if(timeout>maxDelay) break;		
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&UART5_Handler,(u8 *)aRxBuffer, RXBUFFERSIZE)!=HAL_OK)//һ�δ������֮�����¿����жϲ�����RxXferCountΪ1
	{
        timeout++; //��ʱ����
        if(timeout>maxDelay) break;	
	}
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntExit();  											 
#endif
} 

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==UART5)//����Ǵ���5
	{
		if((UART5_RX_STA&0x8000)==0)//����δ���
		{
			if(UART5_COUNT == 100 && aRxBuffer[0] == 0x55)//��δ��ʼ����֡��ʱ����֡ͷ
			{
				UART5_COUNT = 55;//���ֵ�һ��֡ͷ��־λ
				UART5_RX_STA = 0;
				UART5_RX_BUF[UART5_RX_STA&0X3FFF]=aRxBuffer[0];
				UART5_RX_STA++;
			}
			else if(UART5_COUNT == 55 && aRxBuffer[0] == 0x51)//�̵�һ��֡ͷ���ֵ�һ������֡ͷ
			{
				UART5_RX_BUF[UART5_RX_STA&0X3FFF]=aRxBuffer[0];
				UART5_RX_STA++;
				UART5_COUNT = 0;
				UART5_START = 1;
			}
			else if(UART5_START == 1)//��������
			{
				UART5_RX_BUF[UART5_RX_STA&0X3FFF]=aRxBuffer[0];
				UART5_RX_STA++;
				UART5_COUNT+=1;
				if(UART5_COUNT == 42)//�Ѿ���������ʮ��������
				{
					//��Ǳ�־λ���ָ������ж�ʹ�õ�ֵ
					UART5_RX_STA|=0x8000;
					UART5_COUNT = 100;
					UART5_START = 0;
				}
			}
			
		}

	}
	
	if(huart->Instance==UART4)//?????4
	{
		if((UART4_RX_STA&0x8000)==0)//?????
		{
			if(UART4_RX_STA&0x4000)//????0x0d
			{
				if(aRxBuffer[0]!=0x0a)UART4_RX_STA=0;//????,????
				else UART4_RX_STA|=0x8000;	//????? 
			}
			else //????0X0D
			{	
				if(aRxBuffer[0]==0x0d)UART4_RX_STA|=0x4000;
				else
				{
					UART4_RX_BUF[UART4_RX_STA&0X4FFF]=aRxBuffer[0] ;
					UART4_RX_STA++;
					if(UART4_RX_STA>(USART_REC_LEN-1))UART4_RX_STA=0;//??????,??????	  
				}		 
			}
		}

	}
	
}

#endif	



 





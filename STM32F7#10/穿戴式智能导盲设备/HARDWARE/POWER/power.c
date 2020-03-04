#include "power.h"
void PowerInit(void)
{
		GPIO_InitTypeDef GPIO_Initure;
	  __HAL_RCC_GPIOH_CLK_ENABLE();			//����GPIOHʱ��
		__HAL_RCC_GPIOB_CLK_ENABLE();			//����GPIOBʱ��
	                                                                                      
    GPIO_Initure.Pin=GPIO_PIN_13|GPIO_PIN_14; 
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOH,&GPIO_Initure);
	
		GPIO_Initure.Pin=GPIO_PIN_14;
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);
		
    HAL_GPIO_WritePin(GPIOH,GPIO_PIN_13,GPIO_PIN_SET);	//PH13��1
		HAL_GPIO_WritePin(GPIOH,GPIO_PIN_14,GPIO_PIN_SET);	//PH14��1 
	  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_SET);	//PB14��1
}


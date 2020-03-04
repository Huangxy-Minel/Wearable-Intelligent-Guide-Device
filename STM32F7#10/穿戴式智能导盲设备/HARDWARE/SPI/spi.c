#include "task.h"

void my_spi_init(void)
{
	  GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOA_CLK_ENABLE();			//����GPIOAʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_4;   //PA5-ʱ��SCLK��PA4-NSS
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);	//PA4��1
		
}

u8 spi_read_onebit(void)
{
	u8 buf;
	SCLK(1);
	delay_us(2);
	SCLK(0);
	delay_us(2);
	SCLK(1);
	buf = DATA0;
	delay_us(2);
	return buf;
}

u32 ad_read(void)
{
	u8 i;
	u32 buf = 0;
	while(DRDY);
	for(i=0;i<24;i++)
		{
			buf<<=1;
			buf+=spi_read_onebit();
		}
	return buf;
}

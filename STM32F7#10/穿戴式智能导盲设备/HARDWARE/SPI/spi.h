#ifndef _SPI_H
#define _SPI_H
#include "sys.h"
#define SCLK(n)		(n?HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET))
#define SCLK_Toggle (HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5))
#define NSS(n)		(n?HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET))
#define NSS_Toggle (HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_4))

#define DRDY        HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6) 
#define DATA0        HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7) 
void my_spi_init(void);
u32 ad_read(void);
#endif

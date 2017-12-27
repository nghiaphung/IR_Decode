/*
 * @filename: led.c
 * @Author	: nghiaphung
 */
/******************************************************************************/
/**!                               INCLUDE                                    */
/******************************************************************************/
#include "led.h"
#include "stm32f0xx.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"
/******************************************************************************/
/**!                            LOCAL TYPEDEF                                 */
/******************************************************************************/
;
/******************************************************************************/
/**!                            LOCAL SYMBOLS                                 */
/******************************************************************************/
/* Symbol to convert active value */
#define GPIO_ACTIVE_STAGE       1
/******************************************************************************/
/**!                         EXPORTED VARIABLES                               */
/******************************************************************************/

/******************************************************************************/
/**!                          LOCAL VARIABLES                                 */
/******************************************************************************/

/******************************************************************************/
/**!                    LOCAL FUNCTIONS                                       */
/******************************************************************************/

/******************************************************************************/
/**!                        EXPORTED FUNCTIONS                                */
/******************************************************************************/
void Led_Init (void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	/* User data configuration */
	GPIO_InitTypeDef    GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd   = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed  = GPIO_Speed_50MHz;

		/* Initialize led A */
	GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	/* Initialize led B */
	GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_6;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	/* Initialize led C */
	GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_5;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void Led_SetLevel (led_channel_t pLed, led_level_t pLevel)
{
    GPIO_TypeDef* GPIOx = ((void*)0);
    uint16_t   GPIO_Pin = 0;
    uint8_t xOutput = 0;
	
	switch (pLed)
    {
        case LED_A:
            GPIOx   = GPIOA;
            GPIO_Pin = GPIO_Pin_7;
            break;
        case LED_B:
            GPIOx   = GPIOA;
            GPIO_Pin = GPIO_Pin_6;
            break;
        case LED_C:
            GPIOx   = GPIOA;
            GPIO_Pin = GPIO_Pin_5;
            break;
		default:
			break;
    }
    /* Indicate output level */
    if (LED_LEVEL_ENABLE == pLevel)
        xOutput = GPIO_ACTIVE_STAGE;
    else
        xOutput = (GPIO_ACTIVE_STAGE ^ 0x01);
    /* Set to hardware */
    GPIO_WriteBit(GPIOx, GPIO_Pin, (BitAction)xOutput);
}

void Led_Toggle (led_channel_t pLed)
{
    GPIO_TypeDef* GPIOx = ((void*)0);
    uint16_t   GPIO_Pin = 0;
    uint8_t xOutput = 0;
	
	switch (pLed)
    {
        case LED_A:
            GPIOx    = GPIOA;
            GPIO_Pin = GPIO_Pin_7;
            break;
        case LED_B:
            GPIOx    = GPIOA;
            GPIO_Pin = GPIO_Pin_6;
            break;
        case LED_C:
            GPIOx    = GPIOA;
            GPIO_Pin = GPIO_Pin_5;
            break;
		default:
			break;
    }
		
	xOutput = GPIO_ReadOutputDataBit(GPIOx, GPIO_Pin);
	/* Toggle */
	xOutput = xOutput ^ 0x01;
	/* Set to hardware */
    GPIO_WriteBit(GPIOx, GPIO_Pin, (BitAction)xOutput);
}
/******************************************************************************/
/**!                          LOCAL FUNCTIONS                                 */
/******************************************************************************/


/**
 * @filename: button.c
 * @Author	: nghiaphung	
 */
/******************************************************************************/
/**!                               INCLUDE                                    */
/******************************************************************************/
#include "button.h"
#include "stm32f0xx.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_exti.h"
#include "stm32f0xx_tim.h"
#include "stm32f0xx_misc.h"
#include "stm32f0xx_syscfg.h"
#include <stdio.h>
/******************************************************************************/
/**!                            LOCAL TYPEDEF                                 */
/******************************************************************************/

/******************************************************************************/
/**!                            LOCAL SYMBOLS                                 */
/******************************************************************************/
#define BUTTON_PORT              GPIOA
#define BUTTON_PIN               GPIO_Pin_3
#define BUTTON_EXTI_PORT_SRC     EXTI_PortSourceGPIOA
#define BUTTON_EXTI_PIN_SRC      EXTI_PinSource5
#define BUTTON_EXTI_LINE         EXTI_Line3
/******************************************************************************/
/**!                         EXPORTED VARIABLES                               */
/******************************************************************************/

/******************************************************************************/
/**!                          LOCAL VARIABLES                                 */
/******************************************************************************/

/******************************************************************************/
/**!                    LOCAL FUNCTIONS PROTOTYPES                            */
/******************************************************************************/

/******************************************************************************/
/**!                        EXPORTED FUNCTIONS                                */
/******************************************************************************/
void Button_Init (void)
{
	/* User data configuration */
    GPIO_InitTypeDef GPIO_InitStruct;
    EXTI_InitTypeDef EXTI_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
	
	/* Data initialization for GPIO */
	GPIO_InitStruct.GPIO_Pin   = BUTTON_PIN;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_1;
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(BUTTON_PORT, &GPIO_InitStruct);
    /* Configure external interrupt for GPIO */
    EXTI_InitStruct.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Line    = BUTTON_EXTI_LINE;
	EXTI_Init(&EXTI_InitStruct);
	SYSCFG_EXTILineConfig(BUTTON_EXTI_PORT_SRC, BUTTON_EXTI_PIN_SRC);
    /* Configure NVIC for indicated vectors */
    NVIC_InitStruct.NVIC_IRQChannelPriority     = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd     = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannel = EXTI2_3_IRQn;
	NVIC_Init(&NVIC_InitStruct);	
}

button_stage_t Button_Get (void)
{
	return (button_stage_t)GPIO_ReadInputDataBit(BUTTON_PORT, BUTTON_PIN);
}
/******************************************************************************/
/**!                          LOCAL FUNCTIONS                                 */
/******************************************************************************/
static void EXTI2_3_IRQHandler (void)
{
	if (SET == EXTI_GetITStatus(BUTTON_EXTI_LINE))
	{
		
	}
	EXTI_ClearITPendingBit(BUTTON_EXTI_LINE);
}


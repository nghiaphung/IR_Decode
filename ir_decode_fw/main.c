/**
 * @filename: main.c
 * @Author	: nghiaphung	
 */
/******************************************************************************/
/**!                               INCLUDE                                    */
/******************************************************************************/
#include "driver/ir/ir.h"
#include "driver/led/led.h"
#include "stm32f0xx.h"
int main (void)
{
	SystemInit();
	Led_Init();
	IR_Init();
	while (1);
}

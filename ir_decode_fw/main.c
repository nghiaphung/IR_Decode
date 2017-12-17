/**
 * @filename: main.c
 * @Author	: nghiaphung	
 */
/******************************************************************************/
/**!                               INCLUDE                                    */
/******************************************************************************/
#include "driver/ir/ir.h"
#include "stm32f0xx.h"
int main (void)
{
	SystemInit();
	IR_Init();
	while (1);
}

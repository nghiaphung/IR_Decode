/**
 * @filename: main.c
 * @Author	: nghiaphung	
 */
/******************************************************************************/
/**!                               INCLUDE                                    */
/******************************************************************************/
#include <stdbool.h>
#include "driver/ir/ir.h"
#include "driver/led/led.h"
#include "driver/serial/serial.h"
#include "stm32f0xx.h"
/******************************************************************************/
/**!                            LOCAL TYPEDEF                                 */
/******************************************************************************/

/******************************************************************************/
/**!                            LOCAL SYMBOLS                                 */
/******************************************************************************/

/******************************************************************************/
/**!                         EXPORTED VARIABLES                               */
/******************************************************************************/

/******************************************************************************/
/**!                          LOCAL VARIABLES                                 */
/******************************************************************************/
/* Set default cmd for LED is 0, 1, 2 */
uint8_t IR_cmd_LED_A = 0;
uint8_t IR_cmd_LED_B = 1;
uint8_t IR_cmd_LED_C = 2;

state_t gState = STATE_CMD;
/******************************************************************************/
/**!                    LOCAL FUNCTIONS PROTOTYPES                            */
/******************************************************************************/
void SetupHardware (void);
/******************************************************************************/
/**!                        EXPORTED FUNCTIONS                                */
/******************************************************************************/
int main (void)
{
	uint8_t cmd;
	SetupHardware();
	while (1)
	{
		switch (gState)
		{
			case STATE_CMD:
				if (IR_GetCmd(&cmd) == true)
				{
					IR_ClearRecvFlag();
					Serial_SendByte(cmd);
					if (cmd == IR_cmd_LED_A)
						Led_Toggle(LED_A);
					if (cmd == IR_cmd_LED_B)
						Led_Toggle(LED_B);
					if (cmd == IR_cmd_LED_C)
						Led_Toggle(LED_C);					
				}
				break;
			case STATE_LEARNING:
				Led_SetLevel(LED_A, LED_LEVEL_ENABLE);
				Led_SetLevel(LED_B, LED_LEVEL_ENABLE);
				Led_SetLevel(LED_C, LED_LEVEL_ENABLE);
			
				while(IR_GetCmd(&cmd) == false);
				IR_ClearRecvFlag();
				IR_cmd_LED_A = cmd;
				Led_SetLevel(LED_A, LED_LEVEL_DISABLE);
			
				while(IR_GetCmd(&cmd) == false);
				IR_ClearRecvFlag();
				IR_cmd_LED_B = cmd;
				Led_SetLevel(LED_B, LED_LEVEL_DISABLE);
			
				while(IR_GetCmd(&cmd) == false);
				IR_ClearRecvFlag();
				IR_cmd_LED_C = cmd;
				Led_SetLevel(LED_C, LED_LEVEL_DISABLE);
			
				gState = STATE_CMD;
				Serial_SendByte('f');
				break;	
			default:
				break;
		}
	}
}

/******************************************************************************/
/**!                          LOCAL FUNCTIONS                                 */
/******************************************************************************/
void SetupHardware (void)
{
	SystemInit();
	Led_Init();
	IR_Init();
	Serial_Init();
}

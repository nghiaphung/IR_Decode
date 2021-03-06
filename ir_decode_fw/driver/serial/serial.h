/*******************************************************************************
 * @filename: serial.h
 * @Author	: nghiaphung

 ******************************************************************************/
#ifndef SERIAL_H
#define SERIAL_H
/******************************************************************************/
/*                               INCLUDE                                      */
/******************************************************************************/

/******************************************************************************/
/*                             PUBLIC TYPEDEF                                 */
/******************************************************************************/

/******************************************************************************/
/**!                            PUBLIC SYMBOLS                                */
/******************************************************************************/
typedef enum {
	STATE_CMD = 0,
	STATE_LEARNING = 1,
}state_t;
/******************************************************************************/
/**!                          PUBLIC VARIABLES                                */
/******************************************************************************/

/******************************************************************************/
/**!                    PUBLIC FUNCTIONS PROTOTYPES                           */
/******************************************************************************/
void Serial_SendByte(uint8_t byte);
void Serial_Init (void);
#endif /* SERIAL_H */
/******************************************************************************/
/**!                           END OF FILE                                    */
/******************************************************************************/

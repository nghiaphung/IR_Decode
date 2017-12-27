/*******************************************************************************
 * @filename: ir.h
 * @Author  : nghiaphung
 ******************************************************************************/
#ifndef IR_H_
#define IR_H_
/******************************************************************************/
/*                               INCLUDE                                      */
/******************************************************************************/
#include <stdbool.h>
#include <stdint.h>
/******************************************************************************/
/*                             PUBLIC TYPEDEF                                 */
/******************************************************************************/
/**
 * @brief IR protocols
 */
typedef enum
{
    IR_PROTOCOL_RC5             = 0,//!< IR_PROTOCOL_RC5
    IR_PROTOCOL_RC6             = 1,//!< IR_PROTOCOL_RC6
    IR_PROTOCOL_RC_MM           = 2,//!< IR_PROTOCOL_RC_MM
    IR_PROTOCOL_SONY_SIRC       = 3,//!< IR_PROTOCOL_SONY_SIRC
    IR_PROTOCOL_JVC             = 4,//!< IR_PROTOCOL_JVC
    IR_PROTOCOL_NEC             = 5,//!< IR_PROTOCOL_NEC
    IR_PROTOCOL_SHARP           = 6,//!< IR_PROTOCOL_SHARP
    IR_PROTOCOL_MITSUBISHI      = 7,//!< IR_PROTOCOL_MITSUBISHI
    IR_PROTOCOL_PANASONIC       = 8,//!< IR_PROTOCOL_PANASONIC
    IR_PROTOCOL_RCA             = 9,//!< IR_PROTOCOL_RCA
    IR_PROTOCOL_NOKIA_NRC17     = 10,//!< IR_PROTOCOL_NOKIA_NRC17
    IR_PROTOCOL_NUM
}ir_protocol_t;

/**
 * @brief IR events
 */
typedef enum
{
    IR_EVENT_INPUT_SIGNAL                    = 0,//!< IR_EVENT_INPUT_SIGNAL
    IR_EVENT_TIMER_OVERFLOW                  = 1,//!< IR_EVENT_TIMER_OVERFLOW
    IR_EVENT_ERROR_RECEIVED_UNKNOWN_PROTOCOL = 2,//!< IR_EVENT_ERROR_UNKNOWN_PROTOCOL
}ir_event_t;

/**
 * @brief IR callback handler
 * @param xEvent        : event
 * @param xInterval     : microseconds
 * @param xEdgesCount   : edges count
 * @param xIrProtocol   : protocol
 */
typedef \
void (*ir_callback_t)(ir_event_t xEvent, uint16_t xInterval, \
                      uint8_t xEdgesCount, ir_protocol_t xIrProtocol);

/******************************************************************************/
/**!                            PUBLIC SYMBOLS                                */
/******************************************************************************/

/******************************************************************************/
/**!                          PUBLIC VARIABLES                                */
/******************************************************************************/

/******************************************************************************/
/**!                    PUBLIC FUNCTIONS PROTOTYPES                           */
/******************************************************************************/
void IR_Init (void);
bool IR_GetCmd(uint8_t* cmd);
void IR_ClearRecvFlag (void);
#endif /* IR_H_ */
/******************************************************************************/
/**!                           END OF FILE                                    */
/******************************************************************************/


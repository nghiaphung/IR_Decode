/*******************************************************************************
 * @filename: ir.h
 * @Author	: nghiaphung
 * @Date	: 7/11/2017
 * @Email	: ducnghia318@gmail.com
 ******************************************************************************/
#ifndef IR_H_
#define IR_H_

/******************************************************************************/
/*                               INCLUDE                                      */
/******************************************************************************/
#include "stm32f10x.h"
#include "stm32f10x_tim.h"
/******************************************************************************/
/*                             PUBLIC TYPEDEF                                 */
/******************************************************************************/
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
 * @brief Supported IR protocols
 */
typedef enum
{
    IR_PROTOCOL_RC5             = 0,//!< IR_PROTOCOL_RC5
    IR_PROTOCOL_RC6             = 1,//!< IR_PROTOCOL_RC6
    IR_PROTOCOL_SONY_SIRC       = 2,//!< IR_PROTOCOL_SONY_SIRC
    IR_PROTOCOL_JVC             = 3,//!< IR_PROTOCOL_JVC
    IR_PROTOCOL_NEC             = 4,//!< IR_PROTOCOL_NEC
    IR_PROTOCOL_SHARP           = 5,//!< IR_PROTOCOL_SHARP
    IR_PROTOCOL_PANASONIC       = 6,//!< IR_PROTOCOL_PANASONIC
    IR_PROTOCOL_NUM
}ir_protocol_t;

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

typedef struct
{
    ir_callback_t callback;
}IrObj_t;
/******************************************************************************/
/**!                            PUBLIC SYMBOLS                                */
/******************************************************************************/
#define IR_START_BIT_ON_TIME_US_RC5                     ( 889UL  )
#define IR_START_BIT_ON_TIME_US_RC6                     ( 2666UL )
#define IR_START_BIT_ON_TIME_US_SONY_SIRC_PROTOCOL      ( 2400UL )
#define IR_START_BIT_ON_TIME_US_JVC_PROTOCOL            ( 9500UL )
#define IR_START_BIT_ON_TIME_US_NEC_PROTOCOL            ( 9000UL )
#define IR_START_BIT_ON_TIME_US_SHARP_PROTOCOL          ( 320UL  )
#define IR_START_BIT_ON_TIME_US_MITSUBISHI_PROTOCOL     ( 8000UL )
#define IR_START_BIT_ON_TIME_US_PANASONIC_PROTOCOL      ( 1728UL )
#define IR_START_BIT_ON_TIME_US_NOKIA_NRC17_PROTOCOL    ( 500UL  )

/** @brief Edges limit according to specific protocol */
#define IR_FRAME_EDGES_NUMER_RC5                     ( 0 )
#define IR_FRAME_EDGES_NUMER_RC6                     ( 0 )
#define IR_FRAME_EDGES_NUMER_SONY_SIRC_PROTOCOL      ( 26 )
#define IR_FRAME_EDGES_NUMER_JVC_PROTOCOL            ( 0 )
#define IR_FRAME_EDGES_NUMER_NEC_PROTOCOL            ( 0 )
#define IR_FRAME_EDGES_NUMER_SHARP_PROTOCOL          ( 0 )
#define IR_FRAME_EDGES_NUMER_MITSUBISHI_PROTOCOL     ( 0 )
#define IR_FRAME_EDGES_NUMER_PANASONIC_PROTOCOL      ( 0 )
#define IR_FRAME_EDGES_NUMER_NOKIA_NRC17_PROTOCOL    ( 0 )

#define IR_SONY_START_TO_START_INTERVAL_US           ( 45000UL)
#define IR_RC5_START_TO_START_INTERVAL_US            ( 45000UL)
#define IR_RC6_START_TO_START_INTERVAL_US            ( 45000UL)
#define IR_JVC_START_TO_START_INTERVAL_US            ( 45000UL)
#define IR_NEC_START_TO_START_INTERVAL_US            ( 45000UL)
#define IR_SHARP_START_TO_START_INTERVAL_US          ( 45000UL)
#define IR_MITSUBISHI_START_TO_START_INTERVAL_US     ( 45000UL)
#define IR_PANASONIC_START_TO_START_INTERVAL_US      ( 45000UL)
#define IR_NRC17_START_TO_START_INTERVAL_US          ( 45000UL)

#define IR_SIGNAL_FREQ              ( 48000UL ) /* @ref datasheet of TSOP1838 */
#define IR_SIGNAL_TOLERANCE_US      ( 100UL )
/******************************************************************************/
/**!                          PUBLIC VARIABLES                                */
/******************************************************************************/

/******************************************************************************/
/**!                          INLINE FUNCTIONS                                */
/******************************************************************************/

/******************************************************************************/
/**!                    PUBLIC FUNCTIONS PROTOTYPES                           */
/******************************************************************************/

#endif 
/******************************************************************************/
/**!                           END OF FILE                                    */
/******************************************************************************/

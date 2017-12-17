/*******************************************************************************
 * @filename: ir_decoder.h
 * @Author  : fitz-nguyen
 * @Date    : Jun 26, 2017
 * @Email   : powerelectronics.vn@gmail.com
 ******************************************************************************/
#ifndef FRAMEWORK_IR_DECODER_H_
#define FRAMEWORK_IR_DECODER_H_
/*******************************************************************************
 *                               HISTORY
 *  July 7, 2017        1.0.1   fitz-nguyen : Initial revision
 ******************************************************************************/

/******************************************************************************/
/*                               INCLUDE                                      */
/******************************************************************************/
#include "platform.h"
#include "ir.h"
#include "switch.h"
/******************************************************************************/
/*                             PUBLIC TYPEDEF                                 */
/******************************************************************************/
typedef struct {
    uint16_t interval;
    uint8_t  edges;
    uint8_t  protocol;
}ir_decode_intput_t;

typedef struct {
    uint8_t  xCount;
    uint32_t xPreviousBit;
    uint8_t  vTemp;
}ir_decode_var_t;

typedef struct {
    uint8_t  cmd;
    uint8_t  addr;
    uint16_t value;
}ir_decode_output_t;
/******************************************************************************/
/**!                            PUBLIC SYMBOLS                                */
/******************************************************************************/

/******************************************************************************/
/**!                          PUBLIC VARIABLES                                */
/******************************************************************************/

/******************************************************************************/
/**!                          INLINE FUNCTIONS                                */
/******************************************************************************/

/******************************************************************************/
/**!                    PUBLIC FUNCTIONS PROTOTYPES                           */
/******************************************************************************/
/**
 * @brief to decode IR signal base on input information
 * @param pcIn
 * @param pxOut
 * @return
 *          EOK: finish a frame, decode OK, output is available.
 *          Others: error. Output is not valid.
 */
//IMPORT int xIR_Decoder(ir_decode_intput_t pcIn, uint32_t* pxOut);
IMPORT int xIR_Decoder(ir_decode_intput_t pcIn, ir_decode_output_t *xOut);

/**
 * @brief Set IR button command for each switch
 * @param[in]   pSwitch_t: switch want to set code, pCmd: command code
 * @param[out]  none
 * @return      none
 * @details
 */

IMPORT void xIR_SetButtonCmdCode(switch_channel_t pSwitch, uint8_t pValue);
/**
 * @brief Get IR button command for each switch
 * @param[in]   pSwitch_t: switch want to get code
 * @param[out]  none
 * @return      command that controlling pSwitch_t
 * @details
 */
IMPORT uint8_t xIR_GetButtonCmdCode(switch_channel_t pSwitch);
#endif /* FRAMEWORK_IR_DECODER_H_ */
/******************************************************************************/
/**!                           END OF FILE                                    */
/******************************************************************************/
/*                   All rights reserved © 2017 PE JSC                        */
/******************************************************************************/

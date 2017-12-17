/*******************************************************************************
 * @filename: ir_decoder.c
 * @Author  : fitz-nguyen
 * @Date    : Jun 26, 2017
 * @Email   : powerelectronics.vn@gmail.com
 */
/******************************************************************************/
/**!                           REVISIONS HISTORY                              */
/******************************************************************************/
/**!
 *      July 7, 2017    1.0.1   fitz-nguyen : Initial revision
 */
/******************************************************************************/
/**!                               INCLUDE                                    */
/******************************************************************************/
#include "ir_decoder.h"
/******************************************************************************/
/**!                            LOCAL TYPEDEF                                 */
/******************************************************************************/

/******************************************************************************/
/**!                            LOCAL SYMBOLS                                 */
/******************************************************************************/
#define IR_SIGNAL_TOLERANCE_US      ( 100UL )
/* @ref https://en.wikipedia.org/wiki/RC-5 */
#define IR_START_BIT_ON_TIME_US_RC5                     ( 889UL  )
#define IR_START_BIT_ON_TIME_US_RC6                     ( 2666UL )
#define IR_START_BIT_ON_TIME_US_RC_MM                   ( 417UL  )
#define IR_START_BIT_ON_TIME_US_SONY_SIRC_PROTOCOL      ( 2400UL )
#define IR_START_BIT_ON_TIME_US_JVC_PROTOCOL            ( 9500UL )
#define IR_START_BIT_ON_TIME_US_NEC_PROTOCOL            ( 9000UL )
#define IR_START_BIT_ON_TIME_US_SHARP_PROTOCOL          ( 320UL  )
#define IR_START_BIT_ON_TIME_US_MITSUBISHI_PROTOCOL     ( 8000UL )
#define IR_START_BIT_ON_TIME_US_PANASONIC_PROTOCOL      ( 1728UL )
#define IR_START_BIT_ON_TIME_US_RCA_PROTOCOL            ( 4000UL )
#define IR_START_BIT_ON_TIME_US_NOKIA_NRC17_PROTOCOL    ( 500UL  )

#define IR_SONY_PROTOCOL_LONG_INTERVAL                  ( 1200UL )
#define IR_SONY_PROTOCOL_SHORT_INTERVAL                 ( 600UL  )
#define IR_DECODE_ON_BIT                                1
#define IR_DECODE_FINISH_BIT                            0
/******************************************************************************/
/**!                         EXPORTED VARIABLES                               */
/******************************************************************************/

/******************************************************************************/
/**!                    LOCAL FUNCTIONS PROTOTYPES                            */
/******************************************************************************/
uint32_t xIR_RC5_Decoder (ir_decode_intput_t pcIn, ir_decode_var_t *pVar, ir_decode_output_t* xOut);
uint32_t xIR_SONY_Decoder (ir_decode_intput_t pcIn, ir_decode_var_t *pVar, ir_decode_output_t* xOut);
/******************************************************************************/
/**!                          LOCAL VARIABLES                                 */
/******************************************************************************/
LOCAL uint8_t xBUTTON1_IR_CMD = 0x00;
LOCAL uint8_t xBUTTON2_IR_CMD = 0x01;
LOCAL uint8_t xBUTTON3_IR_CMD = 0x02;
/******************************************************************************/
/**!                        EXPORTED FUNCTIONS                                */
/******************************************************************************/
/**
 * @brief to decode IR signal base on input information
 * @param pcIn
 * @param pxOut
 * @return
 *          EOK: finish a frame, decode OK, output is available.
 *          Others: error. Output is not valid.
 */
int xIR_Decoder(ir_decode_intput_t pcIn, ir_decode_output_t *xOut)
{
    static ir_decode_var_t vIR_var;
    switch (pcIn.protocol)
    {
        case IR_PROTOCOL_RC5:
        {
            return xIR_RC5_Decoder(pcIn, &vIR_var, xOut);
        }
        case IR_PROTOCOL_SONY_SIRC:
        {
            return xIR_SONY_Decoder(pcIn, &vIR_var, xOut);
        }
        default:
            break;
    }
    return 1;
}

/**
 * @brief Set IR button command for each switch
 * @param[in]   pSwitch_t: switch want to set code, pCmd: command code
 * @param[out]  none
 * @return      none
 * @details
 */
PUBLIC void xIR_SetButtonCmdCode(switch_channel_t pSwitch, uint8_t pCmd)
{
    switch (pSwitch)
    {
        case SWITCH_A:
            xBUTTON1_IR_CMD = pCmd;
            break;
        case SWITCH_B:
            xBUTTON2_IR_CMD = pCmd;
            break;
        case SWITCH_C:
            xBUTTON3_IR_CMD = pCmd;
            break;
        default: break;
    }
}

/**
 * @brief Get IR button command for each switch
 * @param[in]   pSwitch_t: switch want to get code
 * @param[out]  none
 * @return      command that controlling pSwitch_t
 * @details
 */
PUBLIC uint8_t xIR_GetButtonCmdCode(switch_channel_t pSwitch)
{
    uint8_t vTemp;
    switch (pSwitch)
    {
        case SWITCH_A:
            vTemp = xBUTTON1_IR_CMD;
            break;
        case SWITCH_B:
            vTemp = xBUTTON2_IR_CMD;
            break;
        case SWITCH_C:
            vTemp = xBUTTON3_IR_CMD;
            break;
    }
    return vTemp;
}

/******************************************************************************/
/**!                          LOCAL FUNCTIONS                                 */
/******************************************************************************/
uint32_t xIR_RC5_Decoder (ir_decode_intput_t pcIn, ir_decode_var_t *pVar, ir_decode_output_t* xOut)
{
    /* RC5 transmit MSB first */
    uint16_t vLongMax, vLongMin;
    uint16_t vShortMax, vShortMin;

    vLongMax = (IR_START_BIT_ON_TIME_US_RC5 * 2) + IR_SIGNAL_TOLERANCE_US;
    vLongMin = (IR_START_BIT_ON_TIME_US_RC5 * 2) - IR_SIGNAL_TOLERANCE_US;
    vShortMax = IR_START_BIT_ON_TIME_US_RC5 + IR_SIGNAL_TOLERANCE_US;
    vShortMax = IR_START_BIT_ON_TIME_US_RC5 - IR_SIGNAL_TOLERANCE_US;

    if (pcIn.edges == 2)
    {
        pVar->vTemp = 1;
        pVar->xCount = 1;
        pVar->xPreviousBit = 0x0001;
        xOut->value = 0x0001;
    }
    if (pcIn.edges > 2)
    {
        if ((pcIn.interval > vLongMin) && (pcIn.interval < vLongMax))
        {
            pVar->xPreviousBit ^= 0x0001;
            xOut->value = (xOut->value << 1) | pVar->xPreviousBit;
            pVar->vTemp = 1;
            pVar->xCount++;
        }
        if ((pcIn.interval > vShortMin) && (pcIn.interval < vShortMax))
            {
                if (pVar->vTemp == 1)
                    pVar->vTemp = 2;
                else
                {
                    //pVar->xPreviousBit = pVar->xPreviousBit;
                    xOut->value = (xOut->value << 1) | pVar->xPreviousBit;
                    pVar->vTemp = 1;
                    pVar->xCount++;
                }
            }
    }
    //if ((pVar->xCount == 14) && (pVar->vTemp == 2))
    if (pVar->xCount == 14)
    {
        return 0;
    }
    return 1;
}

uint32_t xIR_SONY_Decoder(ir_decode_intput_t pcIn, ir_decode_var_t *pVar, ir_decode_output_t* xOut)
{
    /* SONY transmit LSB first */
    uint16_t vLongMax, vLongMin;

    vLongMax  = IR_SONY_PROTOCOL_LONG_INTERVAL + IR_SIGNAL_TOLERANCE_US;
    vLongMin  = IR_SONY_PROTOCOL_LONG_INTERVAL - IR_SIGNAL_TOLERANCE_US;

    if (pcIn.edges == 3){
        pVar->xCount = 0;
        pVar->xPreviousBit = 0;
        xOut->value  = 0;
    }
    else
    {
        if ((pcIn.edges & 0x01) == 0) //!< Data is available at even edge
        {
            /* Increase bits count */
            pVar->xCount++;
            /* Shift out to convert from LSB to MSB and the end of frame */
            pVar->xPreviousBit = pVar->xPreviousBit >> 1;
            /* Check 1 bit */
            if ((pcIn.interval > vLongMin) && (pcIn.interval < vLongMax)){
                pVar->xPreviousBit += ((uint16_t)1<<11);
            }
            /* Check whether output is available or not */
            if (pVar->xCount == 12) {
                xOut->value = pVar->xPreviousBit;
                /* parse IR value to command and address */
                xOut->cmd = xOut->value & 0x7F;
                xOut->addr = (xOut->value >> 7) & 0x1F;
                return 0;
            }
        }
    }
    return 1;
}
/******************************************************************************/
/*                   All rights reserved © 2017 PE JSC                        */
/******************************************************************************/

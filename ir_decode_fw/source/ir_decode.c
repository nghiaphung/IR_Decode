/*
 * @filename: ir_decode.c
 * @Author	: nghiaphung
 */

/******************************************************************************/
/**!                               INCLUDE                                    */
/******************************************************************************/
#include "ir_decode.h"
/******************************************************************************/
/**!                            LOCAL TYPEDEF                                 */
/******************************************************************************/

/******************************************************************************/
/**!                            LOCAL SYMBOLS                                 */
/******************************************************************************/
#define IR_SONY_PROTOCOL_LONG_INTERVAL                  1200UL
#define IR_SONY_PROTOCOL_SHORT_INTERVAL                 600UL
#define IR_SIGNAL_TOLERANCE_US                          100
/******************************************************************************/
/**!                         EXPORTED VARIABLES                               */
/******************************************************************************/

/******************************************************************************/
/**!                          LOCAL VARIABLES                                 */
/******************************************************************************/

/******************************************************************************/
/**!                    LOCAL FUNCTIONS PROTOTYPES                            */
/******************************************************************************/
uint32_t IR_SonyDecode(ir_decode_intput_t input, ir_decode_output_t* output, ir_decode_var_t* var);
/******************************************************************************/
/**!                        EXPORTED FUNCTIONS                                */
/******************************************************************************/
int IR_Decode (ir_decode_intput_t *input, ir_decode_output_t *output)
{
    static ir_decode_var_t var;
    switch (input->protocol)
    {
        case IR_PROTOCOL_SONY_SIRC:
            return IR_SonyDecode(*input, output, &var);
        default:
            break;
    }
	return 1;
}
/******************************************************************************/
/**!                          LOCAL FUNCTIONS                                 */
/******************************************************************************/
uint32_t IR_SonyDecode(ir_decode_intput_t input, ir_decode_output_t* output, ir_decode_var_t* var)
{
    /* SONY transmit LSB first */
    uint16_t vLongMax, vLongMin;

    vLongMax  = IR_SONY_PROTOCOL_LONG_INTERVAL + IR_SIGNAL_TOLERANCE_US;
    vLongMin  = IR_SONY_PROTOCOL_LONG_INTERVAL - IR_SIGNAL_TOLERANCE_US;

    if (input.edges == 3){
        var->xCount = 0;
        var->xPreviousBit = 0;
        output->value  = 0;
    }
    else
    {
        if ((input.edges & 0x01) == 0) //!< Data is available at even edge
        {
            /* Increase bits count */
            var->xCount++;
            /* Shift out to convert from LSB to MSB and the end of frame */
            var->xPreviousBit = var->xPreviousBit >> 1;
            /* Check 1 bit */
            if ((input.interval > vLongMin) && (input.interval < vLongMax)){
                var->xPreviousBit += ((uint16_t)1<<11);
            }
            /* Check whether output is available or not */
            if (var->xCount == 12) {
                output->value = var->xPreviousBit;
                /* parse IR value to command and address */
                output->cmd = output->value & 0x7F;
                output->addr = (output->value >> 7) & 0x1F;
                return 0;
            }
        }
    }
    return 1;
}


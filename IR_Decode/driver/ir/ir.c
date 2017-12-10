/*****************************************************************************
 * @filename: 
 * @Author	: nghiaphung
 * @Date	: 
 * @Email	: ducnghia318@gmail.com
 */
 
/******************************************************************************/
/**!                               INCLUDE                                    */
/******************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "ir.h"
/******************************************************************************/
/**!                            LOCAL TYPEDEF                                 */
/******************************************************************************/
typedef struct
{
    uint16_t start_width;
    uint32_t start_to_start_interval;
    ir_protocol_t protocol;
}ir_protocols_map_t;
/******************************************************************************/
/**!                            LOCAL SYMBOLS                                 */
/******************************************************************************/
#define IR_PORT            GPIOA
#define IR_PIN             GPIO_Pin_1
#define IR_TIM_CAP_UNIT    TIM2
#define IR_TIM_CAP_CH      TIM_Channel_2

#define IR_START_PRESS              ( 1 )
#define IR_CONTINUE_PRESS           ( 0 )


#define IR_FIRST_EDGE  1
#define IR_SECOND_EDGE 2


/******************************************************************************/
/**!                         EXPORTED VARIABLES                               */
/******************************************************************************/

/******************************************************************************/
/**!                          LOCAL VARIABLES                                 */
/******************************************************************************/
uint32_t IR_CarryFreq = IR_SIGNAL_FREQ;
ir_callback_t IrCallback  = ((void*)0);
const ir_protocols_map_t IR_Protocol_map[IR_PROTOCOL_NUM] = 
{
    {IR_START_BIT_ON_TIME_US_RC5                 ,IR_RC5_START_TO_START_INTERVAL_US,       IR_PROTOCOL_RC5         },
    {IR_START_BIT_ON_TIME_US_RC6                 ,IR_RC6_START_TO_START_INTERVAL_US,       IR_PROTOCOL_RC6         },
    {IR_START_BIT_ON_TIME_US_SONY_SIRC_PROTOCOL  ,IR_SONY_START_TO_START_INTERVAL_US,      IR_PROTOCOL_SONY_SIRC   },
    {IR_START_BIT_ON_TIME_US_JVC_PROTOCOL        ,IR_JVC_START_TO_START_INTERVAL_US,       IR_PROTOCOL_JVC         },
    {IR_START_BIT_ON_TIME_US_NEC_PROTOCOL        ,IR_NEC_START_TO_START_INTERVAL_US,       IR_PROTOCOL_NEC         },
    {IR_START_BIT_ON_TIME_US_SHARP_PROTOCOL      ,IR_SHARP_START_TO_START_INTERVAL_US,     IR_PROTOCOL_SHARP       },
    {IR_START_BIT_ON_TIME_US_PANASONIC_PROTOCOL  ,IR_PANASONIC_START_TO_START_INTERVAL_US, IR_PROTOCOL_PANASONIC   },
};
/******************************************************************************/
/**!                    LOCAL FUNCTIONS PROTOTYPES                            */
/******************************************************************************/

/******************************************************************************/
/**!                        EXPORTED FUNCTIONS                                */
/******************************************************************************/
void IR_Init (IrObj_t* IrObj)
{
    GPIO_TypeDef*           GPIOx;
    TIM_TypeDef*            TIMx;
    GPIO_InitTypeDef        GPIO_InitStruct;
    TIM_TimeBaseInitTypeDef TIM_InitStruct;
    TIM_ICInitTypeDef       TIM_ICInitStruct;
    NVIC_InitTypeDef        NVIC_InitStruct;
    RCC_ClocksTypeDef       RCC_ClockStatus;
    uint16_t prescaler;
    /* get clock status */
    RCC_GetClocksFreq(&RCC_ClockStatus);
    /* calculate prescaler */
    prescaler = (uint16_t)(RCC_ClockStatus.PCLK1_Frequency/IR_CarryFreq) - 1;
    /* init GPIO */
    GPIOx = IR_PORT;
    GPIO_InitStruct.GPIO_Pin    = IR_PIN;
    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed  = GPIO_Speed_50MHz;
    GPIO_Init(GPIOx, &GPIO_InitStruct);
    TIMx                        = IR_TIM_CAP_UNIT;
    TIM_InitStruct.TIM_ClockDivision     = TIM_CKD_DIV1;
    TIM_InitStruct.TIM_CounterMode       = TIM_CounterMode_Up;
    TIM_InitStruct.TIM_Period            = 0xFFFF;
    TIM_InitStruct.TIM_Prescaler         = prescaler;
    TIM_InitStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIMx, &TIM_InitStruct);
    TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIMx, ENABLE);
    /* set up IR event */
    TIM_ICInitStruct.TIM_Channel         = IR_TIM_CAP_CH;
    TIM_ICInitStruct.TIM_ICFilter        = 0;
    TIM_ICInitStruct.TIM_ICPolarity      = TIM_ICPolarity_BothEdge;
    TIM_ICInitStruct.TIM_ICPrescaler     = TIM_ICPSC_DIV1;
    TIM_ICInitStruct.TIM_ICSelection     = TIM_ICSelection_DirectTI;
    TIM_ICInit(TIMx, &TIM_ICInitStruct);
    TIM_CCxCmd(TIMx, IR_TIM_CAP_CH, TIM_CCx_Enable);
    TIM_ITConfig(TIMx, TIM_IT_CC2, ENABLE);
    /* Enable NVIC */
    NVIC_InitStruct.NVIC_IRQChannel                   = TIM2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_Init(&NVIC_InitStruct);
    /* Setting callback function */
    if (IrObj->callback)
        IrCallback = IrObj->callback;
    
}
/******************************************************************************/
/**!                          LOCAL FUNCTIONS                                 */
/******************************************************************************/
void TIM2_IRQHandler (void)
{
    static ir_protocol_t IR_Protocol = IR_PROTOCOL_SONY_SIRC; //set sony as default protocol
    static uint8_t  EdgesCount = 0;
    static uint32_t IR_TimelineUS = 0;
    static bool IR_RepeatedFrame = false;
    uint32_t PulseLength = 0;
    ir_event_t Event = IR_EVENT_ERROR_RECEIVED_UNKNOWN_PROTOCOL;
    
    /* time overflow event */
    if (SET == TIM_GetITStatus(IR_TIM_CAP_UNIT, TIM_IT_Update))
    {
        /* update timeline */
        IR_TimelineUS += 0xFFFF;
        if (IR_TimelineUS >= 500000)
        {
            EdgesCount = 0;
            IR_TimelineUS = 0;
        }
        /* Clear interrupt flag of timer overflow */
        TIM_ClearITPendingBit(IR_TIM_CAP_UNIT, TIM_IT_Update);
    }
    
    /* Input capture event */
    if (SET == TIM_GetITStatus(IR_TIM_CAP_UNIT, TIM_IT_CC2))
    {
        /* Increase edges counter */
        EdgesCount++;
        /* Get timer counter */
        PulseLength = (uint32_t)(TIM_GetCapture2(IR_TIM_CAP_UNIT));
        /* indicate event */
        Event = IR_EVENT_INPUT_SIGNAL;
        /* increase timeline */
        IR_TimelineUS += PulseLength;
        /* check EdgesCount */
        if (EdgesCount == IR_FIRST_EDGE)
        {
            /* First edge. indicate start of a frame */
            /* Checking repeated frame condition */
            if (IR_TimelineUS < IR_Protocol_map[IR_Protocol].start_to_start_interval)
            {
                /* Repeated frame */
                IR_RepeatedFrame = true;
            } else
            {
                /* stop and start pressing */
                IR_RepeatedFrame = false;
            }
            /* Reset time-line counter */
            IR_TimelineUS = 0;
        } 
        else if (EdgesCount == IR_SECOND_EDGE)
        {
            uint16_t max = 0;
            uint16_t min = 0;
            uint8_t i = 0;
            /* parsing protocol */
            for (i = 0; i < (uint8_t)IR_PROTOCOL_NUM; i++)
            {
                /* Assign max and MIN at the middle point */
                max = IR_Protocol_map[i].start_width;
                min = IR_Protocol_map[i].start_width;
                /* Update max/min with indicated tolerance */
                max += IR_SIGNAL_TOLERANCE_US;
                min -= IR_SIGNAL_TOLERANCE_US;
                /* Check length is in range or not */
                if ((PulseLength < max) && (PulseLength > min))
                {
                    /* Indicate protocol of received signal */
                    IR_Protocol = IR_Protocol_map[i].protocol;
                    break;
                }
            }
        }
        else 
        {
            /* not first or second edge */
            if (IR_RepeatedFrame == false)
            {
                if (IrCallback != NULL)
                    /* call callback handler  */
                    IrCallback(Event, PulseLength, EdgesCount, IR_Protocol);
            }
        }
        /* Clear interrupt flag */
        TIM_ClearFlag(IR_TIM_CAP_UNIT, TIM_FLAG_CC2);
        /* Set the counter value to zero */
        TIM_SetCounter(IR_TIM_CAP_UNIT, 0);
    }
}


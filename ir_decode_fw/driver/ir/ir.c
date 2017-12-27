/*
 * @filename: ir.c
 * @Author  : nghiaphung
 */
/******************************************************************************/
/**!                               INCLUDE                                    */
/******************************************************************************/
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_tim.h"
#include "stm32f0xx_syscfg.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_misc.h"
#include <stdbool.h>
#include "ir.h"
#include "../../source/ir_decode.h"
#include "../led/led.h"
/******************************************************************************/
/**!                            LOCAL TYPEDEF                                 */
/******************************************************************************/
typedef struct
{
    uint16_t start_width;
    ir_protocol_t protocol;
}ir_protocols_map_t;
/******************************************************************************/
/**!                            LOCAL SYMBOLS                                 */
/******************************************************************************/
#define IR_SIGNAL_FREQ              1000000UL
#define IR_SIGNAL_TOLERANCE_US      100UL

#define IR_FRAME_EDGES_NUMER_SONY_SIRC_PROTOCOL      26
#define IR_SONY_START_TO_START_INTERVAL_US           45000UL

#define IR_FIRST_EDGE   1
#define IR_SECOND_EDGE  2

#define IR_START_BIT_ON_TIME_US_RC5                     889UL
#define IR_START_BIT_ON_TIME_US_RC6                     2666UL
#define IR_START_BIT_ON_TIME_US_RC_MM                   417UL
#define IR_START_BIT_ON_TIME_US_SONY_SIRC_PROTOCOL      2400UL
#define IR_START_BIT_ON_TIME_US_JVC_PROTOCOL            9500UL
#define IR_START_BIT_ON_TIME_US_NEC_PROTOCOL            9000UL
#define IR_START_BIT_ON_TIME_US_SHARP_PROTOCOL          320UL 
#define IR_START_BIT_ON_TIME_US_MITSUBISHI_PROTOCOL     8000UL
#define IR_START_BIT_ON_TIME_US_PANASONIC_PROTOCOL      1728UL
#define IR_START_BIT_ON_TIME_US_RCA_PROTOCOL            4000UL
#define IR_START_BIT_ON_TIME_US_NOKIA_NRC17_PROTOCOL    500UL
/******************************************************************************/
/**!                         EXPORTED VARIABLES                               */
/******************************************************************************/

/******************************************************************************/
/**!                          LOCAL VARIABLES                                 */
/******************************************************************************/
uint32_t IR_CarryFreq = IR_SIGNAL_FREQ;
const ir_protocols_map_t IR_Protocol_map[IR_PROTOCOL_NUM] = 
{
	{IR_START_BIT_ON_TIME_US_RC5                 , IR_PROTOCOL_RC5         },
    {IR_START_BIT_ON_TIME_US_RC6                 , IR_PROTOCOL_RC6         },
    {IR_START_BIT_ON_TIME_US_RC_MM               , IR_PROTOCOL_RC_MM       },
    {IR_START_BIT_ON_TIME_US_SONY_SIRC_PROTOCOL  , IR_PROTOCOL_SONY_SIRC   },
    {IR_START_BIT_ON_TIME_US_JVC_PROTOCOL        , IR_PROTOCOL_JVC         },
    {IR_START_BIT_ON_TIME_US_NEC_PROTOCOL        , IR_PROTOCOL_NEC         },
    {IR_START_BIT_ON_TIME_US_SHARP_PROTOCOL      , IR_PROTOCOL_SHARP       },
    {IR_START_BIT_ON_TIME_US_MITSUBISHI_PROTOCOL , IR_PROTOCOL_MITSUBISHI  },
    {IR_START_BIT_ON_TIME_US_PANASONIC_PROTOCOL  , IR_PROTOCOL_PANASONIC   },
    {IR_START_BIT_ON_TIME_US_RCA_PROTOCOL        , IR_PROTOCOL_RCA         },
    {IR_START_BIT_ON_TIME_US_NOKIA_NRC17_PROTOCOL, IR_PROTOCOL_NOKIA_NRC17 }
};

ir_callback_t ir_callback;
static ir_protocol_t IR_Protocol  = IR_PROTOCOL_RC5;
static ir_decode_output_t IR_Output;
bool isGetNewSignal = false;
/******************************************************************************/
/**!                    LOCAL FUNCTIONS PROTOTYPES                            */
/******************************************************************************/
void IR_EventHandle (ir_event_t xEvent, uint16_t xInterval, \
                      uint8_t xEdgesCount, ir_protocol_t xIrProtocol);
/******************************************************************************/
/**!                        EXPORTED FUNCTIONS                                */
/******************************************************************************/
void IR_Init (void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_ICInitTypeDef TIM_ICInitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    RCC_ClocksTypeDef RCC_ClocksStatus;
	uint16_t TIM_Prescaler;
	/* Get clock status */
    RCC_GetClocksFreq(&RCC_ClocksStatus);
	/* Calculate pre-scaler */
	TIM_Prescaler =(uint16_t)(RCC_ClocksStatus.PCLK_Frequency/IR_CarryFreq) - 1;
	/* Initializing GPIO */
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_2;
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_1);
	/* Initializing INPUT CAPTURE for indicated timer */
    TIM_TimeBaseInitStruct.TIM_CounterMode       = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period            = 0xFFFF;
    TIM_TimeBaseInitStruct.TIM_Prescaler         = TIM_Prescaler;
    TIM_TimeBaseInitStruct.TIM_ClockDivision     = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM3, ENABLE);
    /* Setting up IR event */
    TIM_ICInitStruct.TIM_Channel     = TIM_Channel_4;
    TIM_ICInitStruct.TIM_ICFilter    = 0x00;
    TIM_ICInitStruct.TIM_ICPolarity  = TIM_ICPolarity_BothEdge;
    TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInit(TIM3, &TIM_ICInitStruct);
    TIM_CCxCmd(TIM3, TIM_Channel_4, TIM_CCx_Enable);
    TIM_ITConfig(TIM3, TIM_IT_CC4, ENABLE);
    /* Enable NVIC */
    NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
	ir_callback = IR_EventHandle;
}

bool IR_GetCmd(uint8_t* cmd)
{
	if (isGetNewSignal == true)
		*cmd = IR_Output.cmd;
	return isGetNewSignal;
}

void IR_ClearRecvFlag (void)
{
	isGetNewSignal = false;
}
/******************************************************************************/
/**!                          LOCAL FUNCTIONS                                 */
/******************************************************************************/
void TIM3_IRQHandler (void)
{
    static uint8_t  EdgesCount = 0;
    static uint32_t IR_TimelineUS = 0;
    static bool IR_RepeatedFrame = false;
	static int temp = 0;
    uint32_t PulseLength = 0;
    ir_event_t Event = IR_EVENT_ERROR_RECEIVED_UNKNOWN_PROTOCOL;
	/* time overflow event */
    if (SET == TIM_GetITStatus(TIM3, TIM_IT_Update))
    {
        /* update timeline */
        IR_TimelineUS += 65535;
        if (IR_TimelineUS >= 500000)
        {
            EdgesCount = 0;
            IR_TimelineUS = 0;
        }
        /* Clear interrupt flag of timer overflow */
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }
    /* Input capture event */
    if (SET == TIM_GetITStatus(TIM3, TIM_IT_CC4))
    {
        /* Increase edges counter */
        EdgesCount++;
        /* Get timer counter */
        PulseLength = (uint32_t)(TIM_GetCapture4(TIM3));
        /* indicate event */
        Event = IR_EVENT_INPUT_SIGNAL;
        /* increase timeline */
        IR_TimelineUS += PulseLength;
        /* check EdgesCount */
        if (EdgesCount == IR_FIRST_EDGE)
        {
			temp++;
			if (temp == 2)
			{
				temp = temp;
			}
            /* First edge. indicate start of a frame */
            /* Checking repeated frame condition */
            if (IR_TimelineUS < IR_SONY_START_TO_START_INTERVAL_US)
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
			if ((i == (uint8_t)IR_PROTOCOL_NUM) && ir_callback){
                /* Indicate unknown protocol */
                IR_Protocol = IR_PROTOCOL_NUM;
                /* Indicate error event */
                Event = IR_EVENT_ERROR_RECEIVED_UNKNOWN_PROTOCOL;
            }
        }
        else 
        {
            /* not first or second edge */
            if (IR_RepeatedFrame == false)
            {
                if (ir_callback != ((void*)0))
                    /* call callback handler  */
                    ir_callback(Event, PulseLength, EdgesCount, IR_Protocol);
            }
			if ((IR_Protocol == IR_PROTOCOL_SONY_SIRC)
             && (EdgesCount == IR_FRAME_EDGES_NUMER_SONY_SIRC_PROTOCOL)){
                /* Self reset edges counter */
                EdgesCount = 0;
            }
        }
        /* Clear interrupt flag */
        TIM_ClearFlag(TIM3, TIM_FLAG_CC4);
        /* Set the counter value to zero */
        TIM_SetCounter(TIM3, 0);
    }
}

static void IR_EventHandle (ir_event_t xEvent, uint16_t xInterval, \
                      uint8_t xEdgesCount, ir_protocol_t xIrProtocol)
{
	ir_decode_intput_t pcIn;
	pcIn.edges = xEdgesCount;
	pcIn.interval = xInterval;
	pcIn.protocol = xIrProtocol;
	if (IR_Decode(&pcIn, &IR_Output) == 0)
	{
		isGetNewSignal = true;
	}
}


/*******************************************************************************
 * @filename: lcd.c
 * @Author	: nghiaphung
 * @Date	: 7/11/2017
 * @Email	: ducnghia318@gmail.com
 */
 
/******************************************************************************/
/**!                               INCLUDE                                    */
/******************************************************************************/
#include "lcd.h"
/******************************************************************************/
/**!                            LOCAL TYPEDEF                                 */
/******************************************************************************/
#define LCD_EN_PIN              GPIO_Pin_12
#define LCD_RS_PIN              GPIO_Pin_13
#define LCD_RW_PIN              GPIO_Pin_14
#define LCD_DB4_PIN             GPIO_Pin_15
#define LCD_DB5_PIN             GPIO_Pin_8
#define LCD_DB6_PIN             GPIO_Pin_9
#define LCD_DB7_PIN             GPIO_Pin_10

#define LCD_EN_PORT             GPIOB
#define LCD_RS_PORT             GPIOB
#define LCD_RW_PORT             GPIOB
#define LCD_DB4_PORT            GPIOB
#define LCD_DB5_PORT            GPIOA
#define LCD_DB6_PORT            GPIOA
#define LCD_DB7_PORT            GPIOA

#define DON          0x0F  /* Display on      */
#define DOFF         0x0B  /* Display off     */
#define CURSOR_ON    0x0F  /* Cursor on       */
#define CURSOR_OFF   0x0D  /* Cursor off      */
#define BLINK_ON     0x0F  /* Cursor Blink    */
#define BLINK_OFF    0x0E  /* Cursor No Blink */

/* Cursor or Display Shift definitions */
#define SHIFT_CUR_LEFT     0x04  /* Cursor shifts to the left   */
#define SHIFT_CUR_RIGHT    0x05  /* Cursor shifts to the right  */
#define SHIFT_DISP_LEFT    0x06  /* Display shifts to the left  */
#define SHIFT_DISP_RIGHT   0x07  /* Display shifts to the right */

/* Function Set definitions */
#define FOUR_BIT    0x2C  /* 4-bit Interface               */
#define EIGHT_BIT   0x3C  /* 8-bit Interface               */
#define LINE_5X7    0x30  /* 5x7 characters, single line   */
#define LINE_5X10   0x34  /* 5x10 characters               */
#define LINES_5X7   0x38  /* 5x7 characters, multiple line */
/******************************************************************************/
/**!                            LOCAL SYMBOLS                                 */
/******************************************************************************/

/******************************************************************************/
/**!                         EXPORTED VARIABLES                               */
/******************************************************************************/

/******************************************************************************/
/**!                          LOCAL VARIABLES                                 */
/******************************************************************************/

/******************************************************************************/
/**!                    LOCAL FUNCTIONS PROTOTYPES                            */
/******************************************************************************/
void delay_us(uint16_t us);
void delay_ms(uint16_t ms);
void LCD_ConfigPin (void);
void LCD_Cmd (uint8_t cmd);
void LCD_Blink_EN_PIN (void);
void LCD_Data (uint8_t data);
void LCD_Send4bit (uint8_t data);
/******************************************************************************/
/**!                        EXPORTED FUNCTIONS                                */
/******************************************************************************/
void LCD_Init (void)
{
    LCD_ConfigPin();
    delay_ms(100); //delay for power on
    /* Try to set 4bit mode */
    LCD_Send4bit(0x30);
    delay_ms(50);
    LCD_Send4bit(0x30);
    delay_ms(50);    
    LCD_Send4bit(0x32);
    delay_ms(100); //delay for lcdreset
    
    LCD_Cmd(FOUR_BIT & LINES_5X7);
    delay_ms(2); //wait for lcd
    LCD_Cmd(DOFF & CURSOR_OFF & BLINK_OFF); //display off
    delay_ms(2); //wait for lcd 
    LCD_Cmd(DON & CURSOR_OFF & BLINK_OFF); //display on
    delay_ms(2); //wait for lcd 

    LCD_Cmd(0x01); //clear lcd
    delay_ms(2);
    LCD_Cmd(SHIFT_CUR_LEFT); //cursor shift mode
    delay_ms(2);
    LCD_Cmd(0x01); //clear lcd
    delay_ms(2);
    
}

void LCD_Clear (void)
{
    LCD_Cmd(0x01); //clear lcd
    delay_ms(2);
}

void LCD_GotoXY(uint8_t x, uint8_t y)
{
    uint8_t addr;
    if (y != 0)
        addr = 0x40;
    else
        addr = 0x00;
    addr += x;
    LCD_Cmd(0x80 | addr);
    delay_ms(2);
}

void LCD_PutChar (char c)
{
    switch (c)
    {
        case '\f':
            LCD_Clear();
            break;
        case '\n':
            LCD_GotoXY(0,1);
            break;
        default:
            LCD_Data(c);
            delay_ms(2);
            break;            
    }
}

void LCD_PutStr (char* s)
{
    while (*s)
        LCD_PutChar(*s++);
}
/******************************************************************************/
/**!                          LOCAL FUNCTIONS                                 */
/******************************************************************************/
void LCD_ConfigPin (void)
{
    GPIO_InitTypeDef     GPIO_InitStruct;
    /* set clock for gpio: 50Mhz */
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    /* set pin as output push pull */
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;
    /* init enable pin */
    GPIO_InitStruct.GPIO_Pin   = LCD_EN_PIN;
    GPIO_Init(LCD_EN_PORT, &GPIO_InitStruct);
    /* init rs pin */
    GPIO_InitStruct.GPIO_Pin   = LCD_RS_PIN;
    GPIO_Init(LCD_RS_PORT, &GPIO_InitStruct);
    /* init rw pin */
    GPIO_InitStruct.GPIO_Pin   = LCD_RW_PIN;
    GPIO_Init(LCD_RS_PORT, &GPIO_InitStruct);
    /* init DB4 pin */
    GPIO_InitStruct.GPIO_Pin   = LCD_DB4_PIN;
    GPIO_Init(LCD_DB4_PORT, &GPIO_InitStruct);
    /* init DB5 pin */
    GPIO_InitStruct.GPIO_Pin   = LCD_DB5_PIN;
    GPIO_Init(LCD_DB5_PORT, &GPIO_InitStruct);
    /* init DB6 pin */
    GPIO_InitStruct.GPIO_Pin   = LCD_DB6_PIN;
    GPIO_Init(LCD_DB6_PORT, &GPIO_InitStruct);
    /* init DB7 pin */
    GPIO_InitStruct.GPIO_Pin   = LCD_DB7_PIN;
    GPIO_Init(LCD_DB7_PORT, &GPIO_InitStruct);
    /* set all pin low */
    GPIO_ResetBits(LCD_EN_PORT, LCD_EN_PIN);
    GPIO_ResetBits(LCD_RW_PORT, LCD_RW_PIN);
    GPIO_ResetBits(LCD_RS_PORT, LCD_RS_PIN);
    GPIO_ResetBits(LCD_DB4_PORT, LCD_DB4_PIN);
    GPIO_ResetBits(LCD_DB5_PORT, LCD_DB5_PIN);
    GPIO_ResetBits(LCD_DB6_PORT, LCD_DB6_PIN);
    GPIO_ResetBits(LCD_DB7_PORT, LCD_DB7_PIN);
}

void delay_us (uint16_t us)
{
    uint16_t prescaler = 0;
    /* use timer 4 */ 
    RCC_ClocksTypeDef          RCC_ClockStatus = {0};
    /* get clock status */
    RCC_GetClocksFreq(&RCC_ClockStatus);
    /* calcute prescaler value */
    prescaler = (uint16_t)(RCC_ClockStatus.PCLK1_Frequency / 1000000); // tim4 - PCLK1 
    /* init tim4, mode count up, clock division 1 */
    TIM4->ARR = us - 1; //period
    TIM4->CNT = 0;      //start at 0;
    TIM4->EGR = 1;      //event genetator reg
    TIM4->PSC = prescaler;
    TIM4->SR  = 0;      // clear flag status
    TIM4->CR1 = 1;      //enable tim4
    
    while (!TIM4->SR);
    TIM4->CR1 = 0;      //disable tim4
}

void delay_ms (uint16_t ms)
{
    /* ms max: 650ms */
    uint16_t prescaler = 0;
    /* use timer 4 */ 
    RCC_ClocksTypeDef          RCC_ClockStatus = {0};
    /* get clock status */
    RCC_GetClocksFreq(&RCC_ClockStatus);
    /* calcute prescaler value */
    prescaler = (uint16_t)(RCC_ClockStatus.PCLK1_Frequency / 100000); // tim4 - PCLK1 
    /* init tim4, mode count up, clock division 1 */
    TIM4->ARR = 100 * ms - 1; //period
    TIM4->CNT = 0;      //start at 0;
    TIM4->EGR = 1;      //event genetator reg
    TIM4->PSC = prescaler;
    TIM4->SR  = 0;      // clear flag status
    TIM4->CR1 = 1;      //enable tim4
    
    while (!TIM4->SR);
    TIM4->CR1 = 0;      //disable tim4
}

void LCD_Send4bit (uint8_t data)
{
    GPIO_WriteBit(LCD_DB4_PORT, LCD_DB4_PIN, (BitAction)(data & 0x01));
    GPIO_WriteBit(LCD_DB5_PORT, LCD_DB5_PIN, (BitAction)(data & 0x02));
    GPIO_WriteBit(LCD_DB6_PORT, LCD_DB6_PIN, (BitAction)(data & 0x04));
    GPIO_WriteBit(LCD_DB7_PORT, LCD_DB7_PIN, (BitAction)(data & 0x08));
    LCD_Blink_EN_PIN();
}

void LCD_Blink_EN_PIN (void)
{
    GPIO_SetBits(LCD_EN_PORT, LCD_EN_PIN);
    delay_us(50);
    GPIO_ResetBits(LCD_EN_PORT, LCD_EN_PIN);
    delay_us(50);
}

void LCD_Cmd (uint8_t cmd)
{
    /* RS_PIN = 0: command mode */
    GPIO_ResetBits(LCD_RS_PORT, LCD_RS_PIN);
    /* high nibble */
    LCD_Send4bit(cmd >> 4);
    /* low nibble */
    LCD_Send4bit(cmd & 0x0F);
}

void LCD_Data (uint8_t data)
{
    /* RS_PIN = 1: data mode */
    GPIO_SetBits(LCD_RS_PORT, LCD_RS_PIN);
    /* high nibble */
    LCD_Send4bit(data >> 4);
    /* low nibble */
    LCD_Send4bit(data & 0x0F);
}

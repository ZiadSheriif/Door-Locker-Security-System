/**
 * @file Timer1.c
 * @author Ziad Sherif
 * @brief
 * @date 2022-11-02
 *
 */

#include "timer1.h"
#include "common_macros.h"
#include <avr/io.h>
#include <avr/interrupt.h>

static volatile void (*g_callBackPtr)(void) = NULL_PTR;

/**
 * @brief ISR for timer 0 overflow
 *
 */
ISR(TIMER1_OVF_vect)
{
    if (g_callBackPtr != NULL_PTR)
    {
        (*g_callBackPtr)();
    }
}

ISR(TIMER1_COMPA_vect)
{
    if (g_callBackPtr != NULL_PTR)
        (*g_callBackPtr)();
}

/**
 * @brief function that initialize Timer 1
 *
 * @param Config_Ptr
 */
void Timer1_init(const Timer1_ConfigType *Config_Ptr)
{
    TCNT1 = 0; // set 0 to be initial value
    TCCR1A = (1 << FOC1A) | (1 << FOC1B);
    TCCR1B = (1 << CS12) | (1 << CS10) | (1 << WGM12);

    /* SET IF TIMER IS COMPARE MODE*/
    if (Config_Ptr->mode == COMPARE_MODE)
    {
        OCR0 = config_ptr->compareValue; // Set Compare Value
        TCCR1 |= (1 << WGM01)
    }

    /* SET IF TIMER IS OVERFLOW MODE*/
    else if (Config_Ptr->mode == OVERFLOW_MODE)
    {
        TIMSK |= (1 << TOIE0); // Enable Timer0 Overflow Interrupt
    }

    /* SET PRESCALAR*/
    TCCR1A = (TCCR1A & 0xf8) | (Config_Ptr->prescaler);
}

/**
 * @brief function to de-initialize  Timer 1
 *
 */
void Timer1_deInit(void)
{
    TCCR1B = 0;
}

void Timer1_setCallBack(void (*a_ptr)(void))
{
    /* set call back function*/
    g_callBackPtr = a_ptr;
}
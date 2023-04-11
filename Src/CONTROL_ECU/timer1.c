/******************************************************************************
 *
 * Module: Timer/Counter1
 *
 * File Name: timer1.c
 *
 * Description: Source file for the AVR Timer/Counter1
 *
 * Author: Ziad Sherif
 *
 *******************************************************************************/
#include"timer1.h"
#include "common_macros.h" /* To use the macros like SET_BIT */
#include<avr/io.h>
#include<avr/interrupt.h>
#include"gpio.h"

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr)(void) = NULL_PTR;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR(TIMER1_OVF_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER1_COMPA_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

/*******************************************************************************
 *                     Functions Definitions                                   *
 *******************************************************************************/

/* Description:
 * Function to initialize the Timer driver
 */

void Timer1_init(const Timer1_ConfigType * Config_Ptr)
{

	TCNT1 = Config_Ptr->initial_value;		/* Set timer1 initial value */

	/* Configure timer control register TCCR1A
	 * 1. Clear OC1A on compare match (non inverting mode) COM1A1=1 COM1A0=0
	 * 2. Disconnect OC1B  COM1B0=0 COM1B1=0
	 * 3. FOC1A=1 FOC1B=1 because these bits are only active in case non-pwm mode
	 */
	TCCR1A = (1<<FOC1A) |(1<<FOC1B) |(1<<COM1A1) ;

	if(Config_Ptr->mode == NORMAL)
	{
		TCCR1A = (TCCR1A & 0xFC) | (Config_Ptr->mode);
		TIMSK |=(1<<TOIE1);
	}
	else if(Config_Ptr->mode== CTC)
	{
		TCCR1B = (TCCR1B & 0xE7) | ((Config_Ptr->mode)<<1);

		GPIO_setupPinDirection(PORTD_ID, PIN5_ID, PIN_OUTPUT);

		OCR1A = Config_Ptr->compare_value; /* Set the compare value */

		TIMSK |=(1<<OCIE1A);
	}

	/* choose the prescaler */

	TCCR1B = (TCCR1B & 0xF8) | (Config_Ptr->prescaler);

	SREG |=(1<<7);

}

/* Description:
 * Function to disable the Timer1
 */

void Timer1_deInit(void)
{
	/* Clear All Timer1 Registers */
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;

	/* Disable the Input Capture interrupt */
	TIMSK &= ~(1<<TOIE1);
	TIMSK &= ~(1<<OCIE1A);
}

/* Description:
 * Function to set the Call Back function address
 */

void Timer1_setCallBack(void(*a_ptr)(void))
{

	/* Save the address of the Call back function in a global variable */
	g_callBackPtr = a_ptr;
}

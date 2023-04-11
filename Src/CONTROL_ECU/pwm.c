/******************************************************************************
 *
 * Module: PWM
 *
 * File Name: pwm.h
 *
 * Description: header file for the PWM driver
 *
 * Author: Ziad Sherif
 *
 *******************************************************************************/
#include"gpio.h"
#include"pwm.h"
#include<avr/io.h>  /* to use Timer registers */

/* Description:
 * The function responsible for trigger the Timer0 with the PWM Mode.
 * Setup the PWM mode with Non-Inverting.
 * Setup the prescaler with F_CPU/8.
 * Setup the compare value based on the required input duty cycle
 * Setup the direction for OC0 as output pin through the GPIO driver.
 * The generated PWM signal frequency will be 500Hz to control the DC
 * Motor speed.
 */
void PWM_Timer0_Start(uint8 duty_cycle)
{
	/* set initial value */

	TCNT0 = 0;

	/* set compare value */

	OCR0 = duty_cycle;

	/* set OC0/PB3 as output pin */
	GPIO_setupPinDirection(PORTB_ID, PIN3_ID, PIN_OUTPUT);

	/* Configure timer control register
	 * 1. Fast PWM mode FOC0=0
	 * 2. Fast PWM Mode WGM01=1 & WGM00=1
	 * 3. Clear OC0 when match occurs (non inverted mode) COM00=0 & COM01=1
	 * 4. clock = F_CPU/8 CS00=0 CS01=1 CS02=0
	 */

	TCCR0 = (1<<WGM01) |(1<<WGM00) |(1<<COM01) |(1<<CS01) ;

}

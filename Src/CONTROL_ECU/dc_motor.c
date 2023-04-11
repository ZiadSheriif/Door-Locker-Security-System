/******************************************************************************
 *
 * Module: DC Motor
 *
 * File Name: dc_motor.h
 *
 * Description: source file for the DC Motor driver
 *
 * Author: Ziad Sherif
 *
 *******************************************************************************/
#include "dc_motor.h"
#include "gpio.h"
#include "pwm.h"

/* Description:
 * The Function responsible for setup the direction for the two
  motor pins through the GPIO driver.
 * Stop at the DC-Motor at the beginning through the GPIO driver
 */
void DcMotor_Init(void)
{
	/* set PA0 & PA1 as output pins */
	GPIO_setupPinDirection(PORTA_ID, PIN0_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(PORTA_ID, PIN1_ID, PIN_OUTPUT);

	/* Stop at the DC-Motor at the beginning */
	GPIO_writePin(PORTA_ID, PIN0_ID, 0);
	GPIO_writePin(PORTA_ID, PIN1_ID, 0);
}

/* Description:
 * The function responsible for rotate the DC Motor CW/ or A-CW or
  stop the motor based on the state input state value.
 * Send the required duty cycle to the PWM driver based on the
  required speed value.
 */
void DcMotor_Rotate(DcMotor_State state, uint8 speed)
{
	volatile uint8 set_duty_cycle = 0;
	switch (state)
	{
	case CW:
		GPIO_writePin(PORTA_ID, PIN0_ID, LOGIC_HIGH); /* PB0 = 1 */
		GPIO_writePin(PORTA_ID, PIN1_ID, LOGIC_LOW);  /* PB1 = 0 */
		break;
	case STOP:
		GPIO_writePin(PORTA_ID, PIN0_ID, LOGIC_LOW); /* PB0 = 0 */
		GPIO_writePin(PORTA_ID, PIN1_ID, LOGIC_LOW); /* PB0 = 0 */
		break;
	case A_CW:
		GPIO_writePin(PORTA_ID, PIN0_ID, LOGIC_LOW);  /* PB0 = 0 */
		GPIO_writePin(PORTA_ID, PIN1_ID, LOGIC_HIGH); /* PB1 = 1 */
		break;
	}

	switch (speed)
	{
	case 0:
		set_duty_cycle = 0;
		break;
	case 25:
		set_duty_cycle = 64;
		break;
	case 50:
		set_duty_cycle = 128;
		break;
	case 75:
		set_duty_cycle = 192;
		break;
	case 100:
		set_duty_cycle = 255;
		break;
	}

	/* run PWM with the needed duty cycle */

	PWM_Timer0_Start(set_duty_cycle);
}

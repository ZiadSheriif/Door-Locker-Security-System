/*
*  ModuleL dc-motor
*
*
* File Name: dc-motor.c
*
* Description: DC MOTOR driver
*
* Author: Ziad-Sherif

*/

#include "dc-motor.h"
#include "pwm.h"

/*
 * Function Definations
 */
void DcMotor_Init(void)
{
    GPIO_setupPinDirection(MOTOR_PORT_ID, MOTOR_PIN0_ID, PIN_OUTPUT);
    GPIO_setupPinDirection(MOTOR_PORT_ID, MOTOR_PIN1_ID, PIN_OUTPUT);
    GPIO_writePin(MOTOR_PORT_ID, MOTOR_PIN0_ID, 0);
    GPIO_writePin(MOTOR_PORT_ID, MOTOR_PIN1_ID, 0);
}

void DcMotor_Rotate(DcMotor_State state, uint8 speed)
{
    if (stop == state)
    {
        GPIO_writePin(MOTOR_PORT_ID, MOTOR_PIN0_ID, 0);
        GPIO_writePin(MOTOR_PORT_ID, MOTOR_PIN1_ID, 0);
    }
    else if (CLOCK_WISE == state)
    {
        GPIO_writePin(MOTOR_PORT_ID, MOTOR_PIN0_ID, 1);
        GPIO_writePin(MOTOR_PORT_ID, MOTOR_PIN1_ID, 0);
        PWM_Timer0_Start(speed);
    }
}

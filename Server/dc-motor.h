/******************************************************************************
 *
 * Module: dc-motor
 *
 * File Name: dc-motor.h
 *
 * Description: DC MOTOR deriver
 *
 *Author: Ziad-Sherif
 *******************************************************************************/
#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "gpio.h"

/****************************************************************
 * configrations
 *****************************************************************/
#define MOTOR_PORT_ID PORTB_ID
#define MOTOR_PIN0_ID PIN0_ID
#define MOTOR_PIN1_ID PIN1_ID

/*
 * State enum
 */
typedef enum
{
    stop,
    CLOCK_WISE,
    ANTI_CLOCK_WISE
} DcMotor_State;

void DcMotor_Init(void);

void DcMotor_Rotate(DcMotor_State state, uint8 speed);

#endif /* DC_MOTOR_H_ */

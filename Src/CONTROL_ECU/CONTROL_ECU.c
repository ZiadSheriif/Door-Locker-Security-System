/************************************************************************************************************
 *                                                                                                          *
 * Module: CONTROL                                                                                          *
 *                                                                                                          *
 * File Name: CONTROL_ECU.c                                                                                 *
 *                                                                                                          *
 * Description: responsible for all the processing and decisions in the system like password                *
 *              checking, open the door and activate the system alarm                                       *
 * Author: Ziad Sherif                                                                                     *
 *                                                                                                          *
 ************************************************************************************************************/
#include "buzzer.h"
#include "dc_motor.h"
#include "external_eeprom.h"
#include "uart.h"
#include "timer1.h"
#include <util/delay.h>
#include <avr/io.h>

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* global variable contain the ticks count of the timer */
unsigned char g_tick = 0;

/*******************************************************************************
 *                     Functions Definitions                                   *
 *******************************************************************************/

/* Description:
 * take the key from HMI_ECU
 * save it in the EEPROM
 * check whether it matches with the user's password or not
 */

void PASS_CHECK(void)
{
	uint8 receive, key, i, read1, read2, check = 0;

	for (i = 6; i < 11; i++)
	{
		key = UART_recieveByte();					 /* Receive the key from HMI_ECU */
		receive = EEPROM_writeByte((0x01) + i, key); /* Save the key in the EEPROM */
		_delay_ms(500);								 /* delay to give the EEPROM the time to save successfully */
	}
	/* check the two passwords byte by byte */
	for (i = 0; i < 5; i++)
	{
		EEPROM_readByte((0x01) + i, &read1);
		_delay_ms(10);
		EEPROM_readByte((0x07) + i, &read2);
		_delay_ms(10);
		if (read1 == read2)
		{
			check++; /* If the two bytes are matched increment the check counter */
		}
	}
	if (check == 5) /* the two passwords are matched if the check counter is equal 5 */
	{
		UART_sendByte(1); /* Send 1 to HMI_ECU to let it know that the two passwords are matched */
	}
	else
	{
		UART_sendByte(0); /* Send 0 to HMI_ECU to let it know that the two passwords are not matched */
	}
	check = 0; /* make the check counter 0 every time after the check has done */
}

/* Description:
 * take the password for the first time or when the user wants to change it from HMI_ECU
 * Save it in the EEPROM
 * Check if the two passwords entered are matched or not
 */

void EEPROM(void)
{
	uint8 receive, key, i, read1, read2, check = 0;

	for (i = 0; i < 5; i++)
	{
		key = UART_recieveByte();					 /* Receive the key from HMI_ECU */
		receive = EEPROM_writeByte((0x01) + i, key); /* Save the key in the EEPROM */
		_delay_ms(500);								 /* delay to give the EEPROM the time to save successfully */
	}

	for (i = 6; i < 11; i++)
	{
		key = UART_recieveByte();					 /* Receive the key from HMI_ECU */
		receive = EEPROM_writeByte((0x01) + i, key); /* Save the key in the EEPROM  in different address */
		_delay_ms(500);								 /* delay to give the EEPROM the time to save successfully */
	}
	/* check the two passwords byte by byte */
	for (i = 0; i < 5; i++)
	{
		EEPROM_readByte((0x01) + i, &read1);
		_delay_ms(10);
		EEPROM_readByte((0x07) + i, &read2);
		_delay_ms(10);
		if (read1 == read2)
		{
			check++; /* If the two bytes are matched increment the check counter */
		}
	}
	if (check == 5) /* the two passwords are matched if the check counter is equal 5 */
	{
		UART_sendByte(1); /* Send 1 to HMI_ECU to let it know that the two passwords are matched */
	}
	else
	{
		UART_sendByte(0); /* Send 0 to HMI_ECU to let it know that the two passwords are not matched */
	}
	check = 0; /* make the check counter 0 every time after the check has done */
}

/* Description:
 * counts the ticks of the timer
 * It is the callback function
 */

void TIMER1_ticks()
{
	g_tick++;
}

/*******************************************************************************
 *                   The MAIN of the program                                   *
 *******************************************************************************/

int main(void)
{
	uint8 byte_check; /* a variable to receive a command from HMI_ECU */
	/* choose the configuration of UART:
	 * 8-bit data
	 * parity check is disabled
	 * stop bit is one bit
	 * baud rate is 9600
	 */

	UART_ConfigType config1 = {EIGHT, DISABLED, ONE_BIT, BR7};

	/* choose the configuration of TIMER1:
	 * initial value is 0
	 * compare value is 31250-1=31249
	 * prescaler is 256
	 * the mode is compare mode
	 */

	Timer1_ConfigType config2 = {0, 31249, F_CPU_256, CTC};

	Timer1_init(&config2); /* Initialize TIMER1 with the required configurations */

	UART_init(&config1); /* Initialize UART with the required configurations */

	Timer1_setCallBack(TIMER1_ticks); /* set the TIMER1_ticks to be the callback function */

	DcMotor_Init(); /* Initialize the DC-MOTOR */

	Buzzer_init(); /* Initialize the BUZZER */

	while (1)
	{
		byte_check = UART_recieveByte(); /* Receive a command from HMI_ECU to take a specific action */
		switch (byte_check)
		{
		case '*': /* the user need to change the password or enter the password for the first time */

			EEPROM(); /* call EEPROM function to do this action */

			break;

		case '#': /* The user need to enter the password to either open the dor or change password */

			/* call PASS_CHECK function to check whether the password entered matches the user's password or not */
			PASS_CHECK();

			break;

		case '&': /* The user need to open the door */

			/* OPEN the door for 15 seconds */

			DcMotor_Rotate(CW, 100);
			g_tick = 0;
			while (g_tick <= 15)
				;

			/* The door is hold for 3 seconds */

			DcMotor_Rotate(STOP, 0);
			while (g_tick <= 18)
				;

			/* CLOSE the door for 15 seconds */

			DcMotor_Rotate(A_CW, 100);
			while (g_tick <= 33)
				;

			g_tick = 0;

			DcMotor_Rotate(STOP, 0); /* Stop the motor to not let it always run  */

			break;

		case '$': /* The user entered the password wrong 3 consecutive time so that the ALARM must be ON */

			g_tick = 0;

			/* Activate the BUZZER for 1 minute */

			Buzzer_on();
			while (g_tick <= 60)
				;

			g_tick = 0;

			/* Turn off the BUZZER after 1 minute */

			Buzzer_off();

			break;
		}
	}
}

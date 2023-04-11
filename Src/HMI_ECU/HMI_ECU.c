/************************************************************************************************************
 *                                                                                                          *
 * Module: HUMAN MACHINE INTERFACE                                                                          *
 *                                                                                                          *
 * File Name: HMI_ECU.c                                                                                     *
 *                                                                                                          *
 * Description: Interaction with the user just take inputs through keypad and display messages on the LCD   *
 *                                                                                                          *
 * Author: Ziad Sherif                                                                                        *
 *                                                                                                          *
 ************************************************************************************************************/

#include "lcd.h"
#include "keypad.h"
#include "uart.h"
#include <util/delay.h>
#include "timer1.h"
#include <avr/io.h>

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* global variable contain the ticks count of the timer */
unsigned char g_tick = 0;

/* global variable contain the received byte by UART from CONTROL_ECU */
uint8 receive = 0;

/*******************************************************************************
 *                     Functions Definitions                                   *
 *******************************************************************************/

/* Description:
 * take the password from the user
 * send it to CONTROL_ECU to check on it
 */

void PASS(void)
{
	uint8 key, i;
	for (i = 0; i < 5; i++)
	{
		/* Get the pressed key number, if any switch pressed for more than 500 ms it will considered more than one press */
		key = KEYPAD_getPressedKey();

		if ((key <= 9) && (key >= 0))
		{
			LCD_displayCharacter('*'); /* display the pressed keypad switch as '*' */
			UART_sendByte(key);		   /* send the pressed key to the CONTROL_ECU */
		}
		else
		{
			i--; /* to make the UART send five numbers only not including any characters */
		}
		_delay_ms(500); /* Press time */
	}
	key = KEYPAD_getPressedKey();
	if (key == '=') /* make '=' as an Enter button */
	{
		/* Receive a byte 1 or 0 to check that the two passwords are matched or not */
		receive = UART_recieveByte();
		_delay_ms(500); /* to give the UART time to receive the byte */
	}
}

/* Description:
 * Responsible for changing the password first time and for any time the user wants to change it
 */

void CHANGE_PASSWORD(void)
{
	uint8 key, i;
	for (i = 0; i < 5; i++)
	{
		/* Get the pressed key number, if any switch pressed for more than 500 ms it will considered more than one press */
		key = KEYPAD_getPressedKey();

		if ((key <= 9) && (key >= 0))
		{
			LCD_displayCharacter('*'); /* display the pressed keypad switch */
			UART_sendByte(key);		   /* send the pressed key to the CONTROL_ECU */
		}
		else
		{
			i--; /* to make the UART send five numbers only not including any characters */
		}
		_delay_ms(500); /* Press time */
	}
	key = KEYPAD_getPressedKey();
	if (key == '=') /* make '=' as an Enter button */
	{
		/* Asking the user to enter the same password */
		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 0, "Plz re-enter the");
		LCD_displayStringRowColumn(1, 0, "Same Pass:");
	}
	LCD_moveCursor(1, 11);
	for (i = 0; i < 5; i++)
	{
		/* Get the pressed key number, if any switch pressed for more than 500 ms it will considered more than one press */
		key = KEYPAD_getPressedKey();

		if ((key <= 9) && (key >= 0))
		{
			LCD_displayCharacter('*'); /* display the pressed keypad switch */
			UART_sendByte(key);		   /* send the pressed key to the CONTROL_ECU */
		}
		else
		{
			i--; /* to make the UART send five numbers only not including any characters */
		}
		_delay_ms(500); /* Press time */
	}
	key = KEYPAD_getPressedKey();
	if (key == '=') /* make '=' as an Enter button */
	{
		/* Receive a byte 1 or 0 to check that the two passwords are matched or not */
		receive = UART_recieveByte();
		_delay_ms(500); /* to give the UART time to receive the byte */
	}
}

/* Description:
 * counts the ticks of the timer
 * It is the callback function
 */

void TIMER1_ticks()
{
	g_tick++;
}

/* Description:
 * Display the status of the door while opening or closing
 */

void OPEN_CLOSE_DOOR()
{
	UART_sendByte('&'); /* a command to make the CONTROL_ECU ready for using the door(motor) */

	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "Door is");
	LCD_displayStringRowColumn(1, 0, "Unlocking");

	g_tick = 0; /* begin the counts again to get the needed 15 seconds */

	while (g_tick <= 15)
		; /* wait until the 15 seconds are over */

	LCD_clearScreen();
	LCD_displayString("WARNING..."); /* warning to warn the user that the door will close in 3 seconds */

	while (g_tick <= 18)
		; /* wait until the 3 seconds more are over */

	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "Door is");
	LCD_displayStringRowColumn(1, 0, "Locking");

	while (g_tick <= 33)
		; /* wait until the 15 seconds more are over */

	g_tick = 0; /* make the counter zero again as it finished */
}

/* Description:
 * responsible for the opening and closing the buzzer
 * it is used when the password does not match the user's password for three consecutive trials
 */

void BUZZER()
{
	UART_sendByte('$'); /* a command to make the CONTROL_ECU ready for using the buzzer */
	LCD_clearScreen();
	LCD_displayString("ERROR!!!!");
	g_tick = 0; /* begin the counts again to get the needed 60 seconds (1 minute) */
	while (g_tick <= 60)
		;		/* wait until the 60 seconds more are over */
	g_tick = 0; /* make the counter zero again as it finished */
}

/*******************************************************************************
 *                   The MAIN of the program                                   *
 *******************************************************************************/

int main(void)
{
	uint8 k; /* a variable to get the key from the keypad */
	/* choose the configuration of UART:
	 * 8-bit data
	 * parity check is disabled
	 * stop bit is one bit
	 * baud rate is 9600
	 */

	UART_ConfigType config1 = {EIGHT, DISABLED, ONE_BIT, BR7};

	/* choose the configuration of TIMER1:
	 * initial value is 0
	 * compare value is 999
	 * prescaler is 1024
	 * the mode is compare mode
	 */

	Timer1_ConfigType config2 = {0, 999, F_CPU_1024, CTC};

	Timer1_init(&config2); /* Initialize TIMER1 with the required configurations */

	UART_init(&config1); /* Initialize UART with the required configurations */

	Timer1_setCallBack(TIMER1_ticks); /* set the TIMER1_ticks to be the callback function */

	LCD_init();							  /* Initialize the LCD */
	LCD_displayString("Plz enter pass:"); /* Display the message "Plz enter pass" for the first time */
	LCD_moveCursor(1, 0);

	/* a command to make the CONTROL_ECU ready for reading the password and save it */

	UART_sendByte('*');
	/* calling the function CHANGE_PASSWORD to give the password to the system and save it */

	/* Step 1 */

	CHANGE_PASSWORD();

	if (receive == 1) /* if the two passwords are matched receive 1 */
	{
		/* Go to step 2 */
	}
	/* if the two passwords don't match repeat step 1 for second time  */
	else
	{
		LCD_clearScreen();
		LCD_displayString("Plz enter pass:");
		LCD_moveCursor(1, 0);
		UART_sendByte('*');
		CHANGE_PASSWORD();
		if (receive == 1)
		{
			/* Go to step 2 */
		}
		/* if the two passwords don't match repeat step 1 for last time  */
		else
		{
			LCD_clearScreen();
			LCD_displayString("Plz enter pass:");
			LCD_moveCursor(1, 0);
			UART_sendByte('*');
			CHANGE_PASSWORD();
			if (receive == 1)
			{
				/* Go to step 2 */
			}
			/* if the two passwords don't match DISPLAY an ERROR and close the system */
			else
			{
				LCD_clearScreen();
				LCD_displayString("SYSTEM ERROR");
				return 0;
			}
		}
	}
	/* Step 2 */
	while (1)
	{
		/* Display the main options on the screen */

		LCD_displayStringRowColumn(0, 0, "+ : Open Door");
		LCD_displayStringRowColumn(0, 13, "   ");
		LCD_displayStringRowColumn(1, 0, "- : Change Pass");
		LCD_displayStringRowColumn(1, 15, "  ");

		k = KEYPAD_getPressedKey();
		if (k == '+') /* you choose to open the door */ /* Step 3 */
		{
			/* Enter the password to be able to open the door */
			LCD_clearScreen();
			LCD_displayString("Plz enter pass:");
			LCD_moveCursor(1, 0);
			/* a command to make the CONTROL_ECU ready for reading the password and check it with the one saved before */
			UART_sendByte('#');
			/* take the password from the user */
			PASS();
			if (receive == 1) /* Receive 1 for matched passwords */
			{
				/* Open the door then close it */
				OPEN_CLOSE_DOOR();
			}
			else /* If the two passwords don't match then repeat step 1 for the first time */
			{

				LCD_clearScreen();
				LCD_displayString("Plz enter pass:");
				LCD_moveCursor(1, 0);
				/* a command to make the CONTROL_ECU ready for reading the password and check it with the one saved before */
				UART_sendByte('#');
				/* take the password from the user */
				PASS();
				if (receive == 1) /* Receive 1 for matched passwords */
				{
					/* Open the door then close it */
					OPEN_CLOSE_DOOR();
				}
				else /* If the two passwords don't match then repeat step 1 for the second time */
				{
					LCD_clearScreen();
					LCD_displayString("Plz enter pass:");
					LCD_moveCursor(1, 0);
					/* a command to make the CONTROL_ECU ready for reading the password and check it with the one saved before */
					UART_sendByte('#');
					/* take the password from the user */
					PASS();
					if (receive == 1) /* Receive 1 for matched passwords */
					{
						/* Open the door then close it */
						OPEN_CLOSE_DOOR();
					}
					else /* If the two passwords don't match for the last time then turn on the BUZZER */ /* Step 5 */
					{
						/* Turn on the BUZZER */
						BUZZER();
					}
				}
			}
		}
		else if (k == '-') /* you choose to change the password */ /* Step 4 */
		{
			/* Enter the password to be able to change the password */
			LCD_clearScreen();
			LCD_displayString("Plz enter pass:");
			LCD_moveCursor(1, 0);
			/* a command to make the CONTROL_ECU ready for reading the password and check it with the one saved before */
			UART_sendByte('#');
			/* take the password from the user */
			PASS();
			if (receive == 1) /* Receive 1 for matched passwords */
			{

				LCD_clearScreen();
				LCD_displayString("Plz enter pass:");
				LCD_moveCursor(1, 0);
				/* a command to make the CONTROL_ECU ready for reading the password and save it */
				UART_sendByte('*');
				/* calling the function CHANGE_PASSWORD to give the password to the system and save it */
				CHANGE_PASSWORD();
			}
			else /* If the two passwords don't match then repeat step 1 for the first time */
			{
				LCD_clearScreen();
				LCD_displayString("Plz enter pass:");
				LCD_moveCursor(1, 0);
				/* a command to make the CONTROL_ECU ready for reading the password and check it with the one saved before */
				UART_sendByte('#');
				/* take the password from the user */
				PASS();
				if (receive == 1) /* Receive 1 for matched passwords */
				{
					LCD_clearScreen();
					LCD_displayString("Plz enter pass:");
					LCD_moveCursor(1, 0);
					/* a command to make the CONTROL_ECU ready for reading the password and save it */
					UART_sendByte('*');
					/* calling the function CHANGE_PASSWORD to give the password to the system and save it */
					CHANGE_PASSWORD();
				}
				else /* If the two passwords don't match then repeat step 1 for the second time */
				{
					LCD_clearScreen();
					LCD_displayString("Plz enter pass:");
					LCD_moveCursor(1, 0);
					/* a command to make the CONTROL_ECU ready for reading the password and check it with the one saved before */
					UART_sendByte('#');
					/* take the password from the user */
					PASS();
					if (receive == 1) /* Receive 1 for matched passwords */
					{
						LCD_clearScreen();
						LCD_displayString("Plz enter pass:");
						LCD_moveCursor(1, 0);
						/* a command to make the CONTROL_ECU ready for reading the password and save it */
						UART_sendByte('*');
						/* calling the function CHANGE_PASSWORD to give the password to the system and save it */
						CHANGE_PASSWORD();
					}
					else /* If the two passwords don't match then repeat step 1 for the last time */ /* Step 5 */
					{
						/* Turn on the BUZZER */
						BUZZER();
					}
				}
			}
		}
	}
}

 /******************************************************************************
 *
 * Module: BUZZER
 *
 * File Name: buzzer.c
 *
 * Description: Source file for the Buzzer driver
 *
 * Author: Ziad Sherif
 *
 *******************************************************************************/
#include "gpio.h"
#include"buzzer.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/* Description:
*  Setup the direction for the buzzer pin as output pin through the
   GPIO driver.
*  Turn off the buzzer through the GPIO
*/

void Buzzer_init()
{
	/* Setup the direction for the buzzer pin as output pin through the GPIO driver */

	GPIO_setupPinDirection(PORTA_ID, PIN7_ID, PIN_OUTPUT);

	/* Turn off the buzzer through the GPIO */

	GPIO_writePin(PORTA_ID, PIN7_ID, LOGIC_LOW);

}

/* Description:
 * Function to enable the Buzzer through the GPIO
 */

void Buzzer_on(void)
{
	GPIO_writePin(PORTA_ID, PIN7_ID, LOGIC_HIGH);
}

/* Description:
 * Function to disable the Buzzer through the GPIO
 */

void Buzzer_off(void)
{
	GPIO_writePin(PORTA_ID, PIN7_ID, LOGIC_LOW);
}

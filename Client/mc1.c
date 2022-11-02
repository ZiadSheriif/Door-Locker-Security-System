/*
 * mc1.c
 *
 *  Created on: Oct 19, 2022
 *      Author: Ziad
 */

#include "keypad.h"
#include"uart.h"
#include <util/delay.h>

int main(void)
{
	uint8 key;


    while(1)
    {

		key = KEYPAD_getPressedKey();

		_delay_ms(500);
    }
}



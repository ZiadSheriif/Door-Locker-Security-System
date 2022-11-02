
#include "uart.h"
#include "keypad.h"
#include "lcd.h"
#include "std_types.h"
#include"common_macros.h"
// 1--> flash 0
// 2--> flash 1
// 3--> flash 2

int main(void)
{
	UART_init(9600);
	uint8 keyPressed;
	while (1)
	{
		keyPressed = KEYPAD_getPressedKey();
		UART_sendByte(keyPressed);
	}
}

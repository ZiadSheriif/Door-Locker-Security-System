/*
 * LCD.c
 *
 *  Created on: Sep 30, 2022
 *      Author: Ziad
 */
#include"gpio.h"
#include "LCD.h"
#include<util/delay.h>

void LCD_sendCommand(uint8 command){
	GPIO_writePin(PORTD_ID,PIN4_ID,LOGIC_LOW);
	_delay_ms(1);
	GPIO_writePin(PORTD_ID,PIN5_ID,LOGIC_HIGH);
	_delay_ms(1);
	GPIO_writePort(PORTC_ID,command);
	_delay_ms(1);
	GPIO_writePin(PORTD_ID,PIN5_ID,LOGIC_LOW);
	_delay_ms(1);
}
void LCD_displayCharacter(uint8 character){
	GPIO_writePin(PORTD_ID,PIN4_ID,LOGIC_HIGH);
	_delay_ms(1);
	GPIO_writePin(PORTD_ID,PIN5_ID,LOGIC_HIGH);
	_delay_ms(1);
	GPIO_writePort(PORTC_ID,character);
	_delay_ms(1);
	GPIO_writePin(PORTD_ID,PIN5_ID,LOGIC_LOW);
	_delay_ms(1);
}

void LCD_init(){
	GPIO_setupPinDirection(PORTD_ID,PIN4_ID,PIN_OUTPUT);
	_delay_ms(1);
	GPIO_setupPinDirection(PORTD_ID,PIN5_ID,PIN_OUTPUT);
	_delay_ms(1);
	GPIO_setupPortDirection(PORTC_ID,PORT_OUTPUT);
	_delay_ms(1);
	LCD_sendCommand(0x38);
	_delay_ms(1);
	LCD_sendCommand(0x0C);
	_delay_ms(1);
	LCD_sendCommand(0x01);
}
void LCD_displayString(const char *ch){
	uint8 i=0;
	while(ch[i]!='\0'){
		LCD_displayCharacter(ch[i]);
		i++;
	}
}

/*
 * LCD.h
 *
 *  Created on: Sep 30, 2022
 *      Author: Ziad
 */

#ifndef LCD_H_
#define LCD_H_
#include "std_types.h"

void LCD_sendCommand(uint8);
void LCD_displayCharacter(uint8);
void LCD_init();
void LCD_displayString(const char*);


#endif /* LCD_H_ */

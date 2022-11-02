/**
 * @file buzzer.c
 * @author Ziad Sherif
 * @brief 
 * 
 */

// include headers
#include "buzzer.h"
#include "gpio.h"
void BUZZER_init (void){
	GPIO_setupPinDirection(buzzer_port, buzzer_pin, PIN_OUTPUT);
	 BUZZER_off(); // buzzer off at the beginning
}
void BUZZER_on (void){
	GPIO_writePin(buzzer_port, buzzer_pin, LOGIC_HIGH);
}
void BUZZER_off (void){
	GPIO_writePin(buzzer_port, buzzer_pin, LOGIC_LOW);
}
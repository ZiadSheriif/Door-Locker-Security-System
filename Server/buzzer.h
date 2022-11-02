/**
 * @file buzzer.h
 * @author your name (you@domain.com)
 * @brief 
 *
 * 
 */
#ifndef BUZZER_H_
#define BUZZER_H_
#include "gpio.h"

#define buzzer_port   PORTA_ID
#define buzzer_pin    PIN0_ID


void BUZZER_init (void);
/* function to set the buzzer pin*/
void BUZZER_on (void);
/*function to clear the buzzer pin*/
void BUZZER_off (void);
#endif /* BUZZER_H_ */
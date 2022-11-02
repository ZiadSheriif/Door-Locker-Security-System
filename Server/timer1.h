
/**
 * @file timer1.h
 * @author Ziad Sherif
 * @brief  timer1 module which calibrate timing slots in it
 * @date 2022-11-02
 *
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "std_types.h"
/*
enum  for timer  mode
*/
typedef enum
{
    COMPARE_MODE,
    OVERFLOW_MODE
} Timer1_Mode;

/*
 enum for prescalar
*/

typedef enum
{
    F_CPU_CLOCK = 1,
    F_CPU_8 = 2,
    F_CPU_64 = 3,
    F_CPU_256 = 4,
    F_CPU_1024 = 5
} Timer1_Prescaler;

typedef struct
{

    uint16 initial_value;
    uint16 compare_value;
    Timer1_Prescaler prescaler;
    Timer1_Mode mode;
} Timer1_ConfigType;

/**
 * @brief function that initialize Timer 1
 *
 * @param Config_Ptr
 */
void Timer1_init(const Timer1_ConfigType *Config_Ptr);

/**
 * @brief function to de-initialize  Timer 1
 *
 */
void Timer1_deInit(void);

/**
 * @brief  function that sets call-back fucntion
 *
 */
void Timer1_setCallBack(void (*a_ptr)(void));

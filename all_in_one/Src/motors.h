#ifndef _MOTORS_H_
#define _MOTORS_H_

#include "stm32f1xx_hal.h"
#include <string.h>

/* 
M1-> TIMER1_CH1  M2-> TIMER2_CH1 M3-> TIMER3_CH1 M4-> TIMER4_CH1
1000-2000 (From 0 to 100)
timer configuration:
psc =          arr =
period =       pulse(0%-100%) = 
*/
#define MAX_Counter 4000  //needed to calibrate

void motors_init(TIM_HandleTypeDef *tim1,TIM_HandleTypeDef *tim2,TIM_HandleTypeDef*tim3,TIM_HandleTypeDef*tim4);
void motors_set_speed(float M1_per,float M2_per,float M3_per,float M4_per);


#endif

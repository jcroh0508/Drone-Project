#ifndef _HCSR04_H_
#define _HCSR04_H_

#include "stm32f1xx_hal.h"
#include "systick.h"

/*
configuration for the ultrasonic sensor
trig -> pb10   echo pb->11  tim6 arr = 1000-1  psc = 72-1
*/

#define HC_SR04_GPIO_PORT GPIOB
#define HC_SR04_Trig_GPIO_PIN  GPIO_PIN_11
#define HC_SR04_Echo_GPIO_PIN  GPIO_PIN_10

#define HC_SR04_Trig_H() HAL_GPIO_WritePin( HC_SR04_GPIO_PORT,  HC_SR04_Trig_GPIO_PIN,  GPIO_PIN_SET);
#define HC_SR04_Trig_L() HAL_GPIO_WritePin( HC_SR04_GPIO_PORT,  HC_SR04_Trig_GPIO_PIN,  GPIO_PIN_RESET); 

void  HCSR04_INIT(void);
void open_timer6(void);
void close_timer6(void);
int Get_HS_Echo_Value(void);
int Get_HS_Time_Value(void);
float Get_HS_Length_Value(void);


#endif


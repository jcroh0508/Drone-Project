#include "motors.h"

TIM_HandleTypeDef M1,M2,M3,M4;

void motors_init(TIM_HandleTypeDef *tim1,TIM_HandleTypeDef *tim2,TIM_HandleTypeDef*tim3,TIM_HandleTypeDef*tim4){

    memcpy(&M1, tim1,sizeof(*tim1));
	  memcpy(&M2, tim2,sizeof(*tim2));
	  memcpy(&M3, tim3,sizeof(*tim3));
	  memcpy(&M4, tim4,sizeof(*tim4));
	  HAL_TIM_PWM_Start(&M1,TIM_CHANNEL_1);
	  HAL_TIM_PWM_Start(&M2,TIM_CHANNEL_3);
	  HAL_TIM_PWM_Start(&M3,TIM_CHANNEL_1);
	  HAL_TIM_PWM_Start(&M4,TIM_CHANNEL_2);
	
}

void motors_set_speed(float M1_per,float M2_per,float M3_per,float M4_per){
   
	uint16_t value1,value2,value3,value4;
	value1 = M1_per*MAX_Counter/100;
	value2 = M2_per*MAX_Counter/100;
	value3 = M3_per*MAX_Counter/100;
	value4 = M4_per*MAX_Counter/100;
	
   __HAL_TIM_SET_COMPARE(&M1,TIM_CHANNEL_1,value1);
	 __HAL_TIM_SET_COMPARE(&M2,TIM_CHANNEL_3,value2);
   __HAL_TIM_SET_COMPARE(&M3,TIM_CHANNEL_1,value3);
   __HAL_TIM_SET_COMPARE(&M4,TIM_CHANNEL_2,value4);

}



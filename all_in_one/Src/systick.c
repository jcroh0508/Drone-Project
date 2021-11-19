#include "systick.h"

void HAL_Delay_us(uint32_t time){

  uint32_t start,NumOfTicks,delays,wait;
	
	start = SysTick->VAL;
	NumOfTicks = HAL_GetTick();
  delays = time*72;  // its for 72 MHZ main clock 
  if(delays > start){
	
	  while(HAL_GetTick()==NumOfTicks);
	  wait = 72000 + start - delays;
	  while(wait<SysTick->VAL);
	 
	}else{
	
	         wait = start - delays;
           while(wait<SysTick->VAL && HAL_GetTick()==NumOfTicks);		
	}
	
}

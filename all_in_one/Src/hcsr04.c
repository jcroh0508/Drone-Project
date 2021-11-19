#include "hcsr04.h"

int mscount;
extern TIM_HandleTypeDef htim6;


void HCSR04_INIT(void){

    //TIM_MasterConfigTypeDef sMasterConfig;
	  GPIO_InitTypeDef GPIO_InitStruct;

  
 
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_TIM6_CLK_ENABLE();

  
  HAL_GPIO_WritePin(HC_SR04_GPIO_PORT, HC_SR04_Trig_GPIO_PIN, GPIO_PIN_RESET);

  /*Configure GPIO pin : ECHO_Pin */
  GPIO_InitStruct.Pin = HC_SR04_Echo_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(HC_SR04_GPIO_PORT, &GPIO_InitStruct);

  /*Configure GPIO pin : TRIG_Pin */
  GPIO_InitStruct.Pin = HC_SR04_Trig_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(HC_SR04_GPIO_PORT, &GPIO_InitStruct);

  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 72-1;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 1000-1;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  HAL_TIM_Base_Init(&htim6);
	HAL_TIM_Base_Stop( &htim6);

    HAL_TIM_Base_Start_IT(&htim6);
    HAL_NVIC_SetPriority(TIM6_IRQn, 1, 1);
    HAL_NVIC_EnableIRQ(TIM6_IRQn);

}

void open_timer6(void){
    
	  TIM6->CNT = 0;
    //__HAL_TIM_SET_COUNTER(&htim6,0);   
		HAL_TIM_Base_Start( &htim6);
    mscount = 0;
         

}
void close_timer6(void){

	  HAL_TIM_Base_Stop( &htim6);

}


 void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	 if(htim == &htim6){
    mscount++;
	 __HAL_TIM_CLEAR_FLAG(&htim6,TIM_FLAG_UPDATE);
	 }
}
 
int Get_HS_Echo_Value(void){

  uint16_t echo_value ;
	echo_value = HAL_GPIO_ReadPin(HC_SR04_GPIO_PORT,HC_SR04_Echo_GPIO_PIN);
	return echo_value;

}

int Get_HS_Time_Value(void){

  uint32_t time = mscount*1000;
	time += __HAL_TIM_GET_COUNTER(&htim6); 
  TIM6->CNT = 0;
  HAL_Delay(50); //if not work change it to 50ms
	return time;

}

//get five datas of distance to get a more accurate value by average value
float Get_HS_Length_Value(){

  float distance, sum=0;
	int i;
	uint32_t time;
	for(i=0;i<5;i++){
	
	 HC_SR04_Trig_H()
	 HAL_Delay_us(12);
	 HC_SR04_Trig_L();
	 while(Get_HS_Echo_Value()==0);
   open_timer6();
	 while(Get_HS_Echo_Value()==1);
		close_timer6();
		time = Get_HS_Time_Value();
		distance = (float)time/58.0;
		sum += distance;
	
	}

	  distance = sum/5.0;
  	return distance;

}



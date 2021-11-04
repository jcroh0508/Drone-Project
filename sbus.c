#include "sbus.h"

UART_HandleTypeDef huart1;

extern  SBUS_CHLs_DATA Updated_datas;
uint8_t USART1_RX_BUF[100];
const uint8_t Total_Num_Channel = 8;
const uint8_t Total_Data_Byte = 14;
uint16_t USART1_RX_STA = 0;
uint8_t datas_buffer[1];


void SBUS_UART_INIT(void){

	
	//rx ->PA10
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_9B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	
	HAL_UART_Init(&huart1);

	HAL_UART_Receive_IT(&huart1,datas_buffer,1);
	
}

void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
  GPIO_InitTypeDef GPIO_InitStruct ;
  if(huart->Instance==USART1)
  {
 
    /* Peripheral clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		
		HAL_NVIC_EnableIRQ(USART1_IRQn);		 
		HAL_NVIC_SetPriority(USART1_IRQn, 3, 2);

  }

}

void HAL_UART_RxCpltcallback(UART_HandleTypeDef* huart){

	int i;
	while (huart->Instance == USART1) 
	{
		USART1_RX_BUF[USART1_RX_STA] = datas_buffer[0];
		if (USART1_RX_STA == 0 && USART1_RX_BUF[USART1_RX_STA] != 0x0F) 
			break;
		
		USART1_RX_STA++;
		
		if (USART1_RX_STA > 20) 
			USART1_RX_STA = 0; 
		
		if (USART1_RX_BUF[0] == 0x0F && USART1_RX_BUF[13] == 0x00 && USART1_RX_STA == 14)	//got all the channel datas
		{
			update_sbus(USART1_RX_BUF,&Updated_datas );
			
			for (i = 0; i<13; i++)		
				USART1_RX_BUF[i] = 0;
			
			USART1_RX_STA = 0;
			break;
			
		}
		
	}

}

void update_sbus(uint8_t *datas,SBUS_CHLs_DATA *sbus_datas){

int i;
    if (datas[12] == 0)  //ensure that the flag is zero which means datas from sbus is already set
    {
        
        sbus_datas->CH1 = ((int16_t)datas[ 1] >> 0 | ((int16_t)datas[ 2] << 8 )) & 0x07FF;
        sbus_datas->CH2 = ((int16_t)datas[ 2] >> 3 | ((int16_t)datas[ 3] << 5 )) & 0x07FF;
        sbus_datas->CH3 = ((int16_t)datas[ 3] >> 6 | ((int16_t)datas[ 4] << 2 ) | (int16_t)datas[ 5] << 10 ) & 0x07FF;
        sbus_datas->CH4 = ((int16_t)datas[ 5] >> 1 | ((int16_t)datas[ 6] << 7 )) & 0x07FF;
        sbus_datas->CH5 = ((int16_t)datas[ 6] >> 4 | ((int16_t)datas[ 7] << 4 )) & 0x07FF;
        sbus_datas->CH6 = ((int16_t)datas[ 7] >> 7 | ((int16_t)datas[ 8] << 1 ) | (int16_t)datas[9] << 9 ) & 0x07FF;
        sbus_datas->CH7 = ((int16_t)datas[ 9] >> 2 | ((int16_t)datas[10] << 6 )) & 0x07FF;
        sbus_datas->CH8 = ((int16_t)datas[10] >> 5 | ((int16_t)datas[11] << 3 )) & 0x07FF;

     }
		
}


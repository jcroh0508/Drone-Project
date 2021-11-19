#include "sbus.h"

extern UART_HandleTypeDef huart1;

SBUS_CHLs_DATA Updated_datas;
uint8_t USART1_RX_BUF[100];
const uint8_t Total_Num_Channel = 8;
const uint8_t Total_Data_Byte = 25;
uint16_t USART1_RX_STA = 0;
uint8_t datas_buffer[25];



//void HAL_UART_RxCpltcallback(UART_HandleTypeDef* huart){

//	int i;
//	while (huart->Instance == USART1) 
//	{
//		USART1_RX_BUF[USART1_RX_STA] = datas_buffer[0];
//		if (USART1_RX_STA == 0 && USART1_RX_BUF[USART1_RX_STA] != 0x0F) 
//			break;
//		
//		USART1_RX_STA++;
//		
//		if (USART1_RX_STA > 20) 
//			USART1_RX_STA = 0; 
//		
//		if (USART1_RX_BUF[0] == 0x0F && USART1_RX_BUF[13] == 0x00 && USART1_RX_STA == 14)	//got all the channel datas
//		{
//			update_sbus(USART1_RX_BUF,&Updated_datas );
//			
//			for (i = 0; i<14; i++)		
//				USART1_RX_BUF[i] = 0;
//			
//			USART1_RX_STA = 0;
//			
//		}
//		
//	  	break;
//		
//	}

//}

void update_sbus(uint8_t *datas,SBUS_CHLs_DATA *sbus_datas){

    if (datas[23] == 0)  //ensure that the flag is zero which means datas from sbus is already set
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

void calbration_channels(SBUS_CHLs_DATA *sbus_datas){

if(sbus_datas->CH1>MAX_OTHER_CHANNELS)
	sbus_datas->CH1=MAX_OTHER_CHANNELS;
if(sbus_datas->CH1<MIN_OTHER_CHANNELS)
	sbus_datas->CH1=MIN_OTHER_CHANNELS;

if(sbus_datas->CH2>MAX_OTHER_CHANNELS)
	sbus_datas->CH2=MAX_OTHER_CHANNELS;
if(sbus_datas->CH2<MIN_OTHER_CHANNELS)
	sbus_datas->CH2=MIN_OTHER_CHANNELS;

if(sbus_datas->CH3>MAX_THROTTLE)
	sbus_datas->CH3=MAX_THROTTLE;
if(sbus_datas->CH3<MIN_THROTTLE)
	sbus_datas->CH3=MIN_THROTTLE;

if(sbus_datas->CH4>MAX_OTHER_CHANNELS)
	sbus_datas->CH4=MAX_OTHER_CHANNELS;
if(sbus_datas->CH4<MIN_OTHER_CHANNELS)
	sbus_datas->CH4=MIN_OTHER_CHANNELS;




}


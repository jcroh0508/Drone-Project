#ifndef _SBUS_H_
#define _SBUS_H_

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_uart.h"
#include <string.h>

#define Start_Byte 0x0f
#define Stop_Byter 0x00


typedef struct {

  uint16_t CH1;
	uint16_t CH2;
	uint16_t CH3;
	uint16_t CH4;
	uint16_t CH5;
	uint16_t CH6;
	uint16_t CH7;
	uint16_t CH8;

} SBUS_CHLs_DATA;

#define MAX                   2000
#define MIN                   1000
#define Throttle_channel      3  
#define Yaw_Channle           4
#define Pitch_channel         2
#define Roll_Channle          1


void SBUS_UART_INIT(void);
void HAL_UART_MspInit(UART_HandleTypeDef* huart);
void HAL_UART_RxCpltcallback(UART_HandleTypeDef* huart);
//give a 10ms delay for obtaining datas from the SBUS signal.
void update_sbus(uint8_t *datas,SBUS_CHLs_DATA *sbus_datas);

#endif

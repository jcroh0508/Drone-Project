#ifndef _SBUS_H_
#define _SBUS_H_

#include "stm32f1xx_hal.h"
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

//SBUS_CHLs_DATA Updated_datas;

#define MAX_THROTTLE                   1800
#define MIN_THROTTLE                   200
#define MAX_OTHER_CHANNELS             1550
#define MIN_OTHER_CHANNELS             430
#define Throttle_channel      CH3  
#define Yaw_Channle           CH4
#define Pitch_channel         CH2
#define Roll_Channle          CH1




//give a 10ms delay for obtaining datas from the SBUS signal.
void update_sbus(uint8_t *datas,SBUS_CHLs_DATA *sbus_datas);
void calbration_channels(SBUS_CHLs_DATA *sbus_datas);


#endif

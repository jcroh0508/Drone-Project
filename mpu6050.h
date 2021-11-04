#ifndef __MPU6050_H
#define	__MPU6050_H

#include "stm32f1xx_hal.h"
#include <string.h>

#define MPU_ADDR 0X68

//configuration registers
#define MPU_CFG_REG         0X1A
#define MPU_GYRO_CFG_REG		0X1B	
#define MPU_ACCEL_CFG_REG		0X1C	
#define MPU_SAMPLE_RATE_REG 0X19
#define MPU_PWR_MGMT1_REG		0X6B // battery power management1 register
#define MPU_PWR_MGMT2_REG		0X6C
#define MPU_INT_STATUS_REG  0X3A

//registeINTrs for reading the values of accel
#define MPU_ACCEL_XOUTH_REG		0X3B	
#define MPU_ACCEL_XOUTL_REG		0X3C	
#define MPU_ACCEL_YOUTH_REG		0X3D	
#define MPU_ACCEL_YOUTL_REG		0X3E	
#define MPU_ACCEL_ZOUTH_REG		0X3F	
#define MPU_ACCEL_ZOUTL_REG		0X40	


//Registers for reading the values of gyro
#define MPU_GYRO_XOUTH_REG		0X43	
#define MPU_GYRO_XOUTL_REG		0X44	
#define MPU_GYRO_YOUTH_REG		0X45	
#define MPU_GYRO_YOUTL_REG		0X46	
#define MPU_GYRO_ZOUTH_REG		0X47	
#define MPU_GYRO_ZOUTL_REG		0X48	


typedef struct {

  int16_t x;
	int16_t y;
	int16_t z;
	
}RawValue;

typedef struct {

  float x;
	float y;
	float z;
	
}ScaledValue;

void MPU_INIT(I2C_HandleTypeDef *hi2cx);
void MPU_WriteByte(uint8_t REG_ADDR,uint8_t data);
void MPU_Read(uint8_t REG_ADDR,uint8_t* data,uint8_t NumOfData);
void MPU_CFG(void);
void MPU_GET_RAW_VALUES(RawValue* ACCEL_RAW,RawValue* GYRO_RAW);
void MPU_GET_ACCEL_SCALED(ScaledValue* ACCEL_SCALED,ScaledValue* GYRO_SCALED);

#endif

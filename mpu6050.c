#include "mpu6050.h"


static I2C_HandleTypeDef i2cHandler;
//set the scale of accel and gyro to be 2g and 250deg/s
static float accel_scale_factor = 2000.0f/32768.0f, gyro_scale_factor = 250.0f/32768.0f;

void MPU_INIT(I2C_HandleTypeDef *i2cx){


  memcpy(&i2cHandler, i2cx,sizeof(*i2cx));

}

void MPU_WriteByte(uint8_t REG_ADDR,uint8_t data){
	
	uint8_t datas[2];
	datas[0] = REG_ADDR;
	datas[1] = data;
  
	HAL_I2C_Master_Transmit(&i2cHandler,MPU_ADDR<<1,datas,2,100);
	
  //HAL_I2C_Mem_Write(&i2cHandler,MPU_ADDR<<1,REG_ADDR,1,&data,1,100);

}

void MPU_Read(uint8_t REG_ADDR,uint8_t* data,uint8_t NumOfData){
	
	uint8_t i2cBuf[2];
	i2cBuf[0] = REG_ADDR;

   HAL_I2C_Master_Transmit(&i2cHandler,MPU_ADDR<<1,i2cBuf,1,10);
	 HAL_I2C_Master_Receive(&i2cHandler,MPU_ADDR<<1,data,NumOfData,100);
   //HAL_I2C_Mem_Read(&i2cHandler,MPU_ADDR<<1,REG_ADDR,1,data,NumOfData,100);

}

void MPU_CFG(void){

  MPU_WriteByte(MPU_PWR_MGMT1_REG,0X80);
  HAL_Delay(100);	
	
  MPU_WriteByte(MPU_PWR_MGMT1_REG,0X00);
	
//	MPU_WriteByte(MPU_PWR_MGMT1_REG,0X01);
//	
//  MPU_WriteByte(MPU_PWR_MGMT2_REG,0X00);
	
	MPU_WriteByte(MPU_CFG_REG,0X01);  //set LFP
	
  MPU_WriteByte(MPU_GYRO_CFG_REG,0X18);  //SET SCALE OF GYRO TO +-200 deg/s
	
  MPU_WriteByte(MPU_GYRO_CFG_REG,0X00);  //SET SCALE OF ACCEL TO +-2g
	
  MPU_WriteByte(MPU_SAMPLE_RATE_REG,0X40);
	
}

void MPU_GET_RAW_VALUES(RawValue* ACCEL_RAW,RawValue* GYRO_RAW){

  uint8_t int_status[2];
	uint8_t ACCEL_Datas[6],GYRO_Datas[6];
	MPU_Read(MPU_INT_STATUS_REG,&int_status[1],1);
	
  if(int_status[1]&&0x01){
		
	  MPU_Read(MPU_ACCEL_XOUTH_REG,ACCEL_Datas,6);
		MPU_Read(MPU_GYRO_XOUTH_REG,GYRO_Datas,6);
		
		ACCEL_RAW->x = (ACCEL_Datas[0]<<8)+ACCEL_Datas[1];
		ACCEL_RAW->y = (ACCEL_Datas[2]<<8)+ACCEL_Datas[3];
		ACCEL_RAW->z = (ACCEL_Datas[4]<<8)+ACCEL_Datas[5];
		
		GYRO_RAW->x = (GYRO_Datas[0]<<8)+GYRO_Datas[1];
		GYRO_RAW->y = (GYRO_Datas[2]<<8)+GYRO_Datas[3];
		GYRO_RAW->z = (GYRO_Datas[4]<<8)+GYRO_Datas[5];
	
	}

}


void MPU_GET_ACCEL_SCALED(ScaledValue* ACCEL_SCALED,ScaledValue* GYRO_SCALED){

  RawValue Accel,Gyro;
	MPU_GET_RAW_VALUES(&Accel,&Gyro);
  
  ACCEL_SCALED->x = (Accel.x+0.00f)*accel_scale_factor;
  ACCEL_SCALED->y = (Accel.y+0.00f)*accel_scale_factor;
	ACCEL_SCALED->z = (Accel.z+0.00f)*accel_scale_factor;
	
	GYRO_SCALED->x = (Gyro.x+0.00f)*gyro_scale_factor;
	GYRO_SCALED->y = (Gyro.y+0.00f)*gyro_scale_factor;
	GYRO_SCALED->z = (Gyro.z+.00f)*gyro_scale_factor;
	
}


//void _Accel_Cali(float x_min, float x_max, float y_min, float y_max, float z_min, float z_max)
//{

//	A_X_Bias		= (x_max + x_min)/2.0f;
//	
//	
//	A_Y_Bias		= (y_max + y_min)/2.0f;
//	
//	
//	A_Z_Bias		= (z_max + z_min)/2.0f;
//}

//void MPU6050_Get_Accel_Cali(ScaledData_Def *CaliDef)
//{
//	ScaledData_Def AccelScaled;
//	MPU6050_Get_Accel_Scale(&AccelScaled);
//	
//	//Accel Scale data 
//	CaliDef->x = (AccelScaled.x) - A_X_Bias; // x-Axis
//	CaliDef->y = (AccelScaled.y) - A_Y_Bias;// y-Axis
//	CaliDef->z = (AccelScaled.z) - A_Z_Bias;// z-Axis
//}



#define THROTTLE_MAX 255
#define THROTTLE_MIN 0

#include "math.h"

enum {
	HEAD1,
	HEAD2,
	HEAD3,
	DATASIZE,
	CMD,
	ROLL,
	PITCH,
	YAW,
	THROTTLE,
	AUX,
	CRC,
	PACKETSIZE
};

uint8_t mspPacket[PACKETSIZE];

const int MPU_addr = 0x68; // MPU address
int AcX, AcY, AcZ, Tmp;	// Accelerator
int GyX, GyY, GyZ;			// Gyrosensor 

// angle variables needed for Roll, Pitch, Yaw
float accel_angle_x, accel_angle_y, accel_angle_z;
float gyro_angle_x, gyro_angle_y, gyro_angle_z;
float filtered_angle_x, filtered_angle_y, filtered_angle_z;

// extern: used to declare global variable without any memory assgined to it
extern float roll_output, pitch_output, yaw_output;
extern float motorA_speed, motorB_speed, motorC_spede, motorD_speed;

float baseAcX, baseAcY, baseAcZ;
float baseGyX, baseGyY, baseGyZ;

// time variables
float dt;
unsigned long t_now;
unsigned long t_prev;

// Gyrosensor angles
float gyro_x, gyro_y, gyro_z;

// varialbes needed to be controlled

// ROLL variables
float roll_target_angle = 0.0;
float roll_angle_in;
float roll_rate_in;
float roll_stabilize_kp = 1;
float roll_stabilize_ki = 0;
float roll_rate_kp = 1;
float roll_rate_ki = 0;
float roll_stabilize_iterm;
float roll_rate_iterm;
float roll_output;

// PITCH variables
float pitch_target_angle = 0.0;
float pitch_angle_in;
float pitch_rate_in;
float pitch_stabilize_kp = 1;
float pitch_stabilize_ki = 0;
float pitch_rate_kp = 1;
float pitch_rate_ki = 0;
float pitch_stabilize_iterm;
float pitch_rate_iterm;
float pitch_output;

// YAW variables
float yaw_target_angle = 0.0;
float yaw_angle_in;
float yaw_rate_in;
float yaw_stabilize_kp = 1;
float yaw_stabilize_ki = 0;
float yaw_rate_kp = 1;
float yaw_rate_ki = 0;
float yaw_stabilize_iterm;
float yaw_rate_iterm;
float yaw_output;

// ---------------------------------------------

float base_roll_target_angle;
float base_pitch_target_angle;
float base_yaw_target_angle;

extern float roll_target_angle;
extern float pitch_target_angle;
extern float yaw_target_angle;

// ----- variables for motor speed control --------

// 
float throttle = 0;
float motorA_speed, motorB_speed, motorC_speed, motorD_speed;

int motorA_pin = 6;
int motorB_pin = 10;
int motorC_pin = 9;
int motorD_pin = 5;

// ---------------- Functions ----------------------

void initMPU6050() {
}

void calibAccelGyro() {
	float sumAcX = 0;
	float sumAcY = 0;
	float sumAcZ = 0;
	
	float sumGyX = 0;
	float sumGyY = 0;
	float sumGyZ = 0;
	
	// Get Accel and Gyro data
	// readAccelGyro();
	
	// Get accel and gyrosensor data 10 times
	// and calcualte the average value
	for (int i = 0; i < 10; i++) {
		// readAccelGyro();
		
		sumAcX += AcX, sumAcY += AcY, sumAcZ += AcZ;
		sumGyX += GyX, sumGyY += GyY, sumGyZ += GyZ;
		
		HAL_Delay(100);
		
	}
	
	baseAcX = sumAcX / 10;
	baseAcY = sumAcY / 10;
	baseAcZ = sumAcZ / 10;
	
	baseGyX = sumGyX / 10;
	baseGyY = sumGyY / 10;
	baseGyZ = sumGyZ / 10;
	
}

void calcDT() {
	dt = (t_now - t_prev) / 1000000.0;
	t_prev = t_now;
}

void calcAccelYPR() {
	float accel_x, accel_y, accel_z;
	float accel_xz, accel_yz;
	const float RADIANS_TO_DEGREES = 180 / 3.14159265;
	
	accel_x = AcX - baseAcX;
	accel_y = AcY - baseAcY;
	accel_z = AcZ + (16384 - baseAcZ);
	
	// Roll Angle = accel_angle_y
	accel_yz = sqrt(pow(accel_y, 2) + pow(accel_z, 2));
	accel_angle_y = atan(-accel_x / accel_yz) * RADIANS_TO_DEGREES;
	
	// Pitch Angle = accel_angle_x
	accel_xz = sqrt(pow(accel_x, 2) + pow(accel_z, 2));
	accel_angle_x = atan(accel_y / accel_xz) * RADIANS_TO_DEGREES;
	
	// Yaw angle cannot be calculated using accelerator
	accel_angle_z = 0;
	
}

void calcGyroYPR() {
	const float GYROXYZ_TO_DEGREES_PER_SEC = 131;
	
	gyro_x = (GyX - baseGyX) / GYROXYZ_TO_DEGREES_PER_SEC;
	gyro_y = (GyY - baseGyY) / GYROXYZ_TO_DEGREES_PER_SEC;
	gyro_z = (GyY - baseGyY) / GYROXYZ_TO_DEGREES_PER_SEC;
	
	
}

void calcFilteredYPR() {
	const float AlPHA = 0.96;
	float tmp_angle_x, tmp_angle_y, tmp_angle_z;
	
	tmp_angle_x = filtered_angle_x + gyro_x * dt;
	tmp_angle_y = filtered_angle_y + gyro_y * dt;
	tmp_angle_z = filtered_angle_z + gyro_z * dt;
	
	// Angle after passing the complimentary filter
	filtered_angle_x = AlPHA * tmp_angle_x + (1.0-AlPHA) * accel_angle_x;
	filtered_angle_y = AlPHA * tmp_angle_y + (1.0-AlPHA) * accel_angle_y;
	filtered_angle_z = tmp_angle_z;
	
}


void initYPR() {
	
	// Calculate the initial hovering angle
	// Pass the Roll, Pitch, Yaw angle to the complimentary filter
	for (int i=0; i < 10; i++) {
		
		// readAccelGyro();
		calcDT();
		calcAccelYPR();
		calcGyroYPR();
		calcFilteredYPR();
		
		base_roll_target_angle += filtered_angle_y;
		base_pitch_target_angle += filtered_angle_x;
		base_yaw_target_angle += accel_angle_z;
	}
	
	HAL_Delay(100);
	
	// Calculate Average of Roll, Pitch and Yaw
	roll_target_angle = base_roll_target_angle;
	pitch_target_angle = base_pitch_target_angle;
	yaw_target_angle = base_yaw_target_angle;
	
}

// How dual PID works
/*
When the drone is tilted, we have to consider 2 factors to make drone
to come back to its original position

1. Angle error from the current state
The motor speed changes due to its angle error

2. Current rotation speed
Current rotation speed and direction also affects the speed of the motor

Solution:
First, we need to calculate P and I value of the angle error
Then, we need to calculate P and I value of the accelerator error

Add the P and I for each error

*/

void dualPID(float target_angle, float angle_in, float rate_in,
	float stabilize_kp, float stabilize_ki, float rate_kp, float rate_ki,
	float stabilize_iterm, float rate_iterm, float output) 
	{
		
		// P + I + D
		
		float angle_error;
		float desired_rate;
		float rate_error;
		float stabilize_pterm, rate_pterm;
		
		// dual pid algorithm
		angle_error = target_angle - angle_in;
		
		stabilize_pterm = stabilize_kp * angle_error;
		stabilize_iterm = stabilize_ki * angle_error * dt;
		
		desired_rate = stabilize_pterm;
		
		rate_error = desired_rate - rate_in;
		
		rate_pterm = rate_kp * rate_error; 		// accelerator P value
		rate_iterm = rate_ki * rate_error * dt; // accelerator I value
		
		// output = P + I + D
		output = rate_pterm + rate_iterm + stabilize_iterm;
		
								
								
	}

void calcYPRtoDualPID() {
	roll_angle_in = filtered_angle_y;
	roll_rate_in = gyro_y;
	
	dualPID(roll_target_angle,
          roll_angle_in,
          roll_rate_in,
          roll_stabilize_kp,
          roll_stabilize_ki,
          roll_rate_kp,
          roll_rate_ki,
          roll_stabilize_iterm,
          roll_rate_iterm,
          roll_output
  );
	
	pitch_angle_in = filtered_angle_x;
	pitch_rate_in = gyro_x;
	
	dualPID(pitch_target_angle,
          pitch_angle_in,
          pitch_rate_in,
          pitch_stabilize_kp,
          pitch_stabilize_ki,
          pitch_rate_kp,
          pitch_rate_ki,
          pitch_stabilize_iterm,
          pitch_rate_iterm,
          pitch_output
  );
	
	yaw_angle_in = filtered_angle_z;
	yaw_rate_in = gyro_z;
	
	dualPID(yaw_target_angle,
          yaw_angle_in,
          yaw_rate_in,
          yaw_stabilize_kp,
          yaw_stabilize_ki,
          yaw_rate_kp,
          yaw_rate_ki,
          yaw_stabilize_iterm,
          yaw_rate_iterm,
          yaw_output
  );
	
}
							 
// --------------------- MOTOR SPEED Calculation ---------------------
void calcMotorSpeed() {
	
	// motorA_speed
	// same as Yaw output direction
	// same as Roll output direction
	// same as Pitch output direction
	
	// motorB_speed
	// opposite of Yaw output direction
	// opposite of Roll output direction
	// same as Pitch output direction
	
	// motorC_speed
	// same as Yaw output direction
	// opposite of Roll output direction
	// opposite of Pitch output direction
	
	// motorD_speed
	// opposite of Yaw output direction
	// same as Roll output direction
	// opposite of Pitch output direction
	
	motorA_speed = (throttle == 0) ? 0 : throttle + yaw_output + roll_output + pitch_output;
	motorB_speed = (throttle == 0) ? 0 : throttle + yaw_output - roll_output + pitch_output;
	motorC_speed = (throttle == 0) ? 0 : throttle + yaw_output - roll_output - pitch_output;
	motorD_speed = (throttle == 0) ? 0 : throttle - yaw_output + roll_output - pitch_output;
	
	// analog PWM value only ranges from 0 ~ 255
	// set range within [0, 255] whenever the value goes over the range
	if (motorA_speed < 0) {
		motorA_speed = 0;
	}
	if (motorA_speed > 255) {
		motorA_speed = 255;
	}
	if (motorB_speed < 0) {
		motorB_speed = 0;
	}
	if (motorB_speed > 255) {
		motorB_speed = 255;
	}
	if (motorC_speed < 0) {
			motorC_speed = 0;
		}
	if (motorC_speed > 255) {
		motorC_speed = 255;
	}
	if (motorD_speed < 0) {
			motorD_speed = 0;
		}
	if (motorD_speed > 255) {
		motorD_speed = 255;
	}	
}

void updateMotorSpeed() {
	// update to motor pin
}

// ---------------------------------------------------------------					

void loop() {
	
	// readAccelGyro();
	calcDT();
	
	calcAccelYPR();			// Accel Sensor Roll, Pitch, Yaw angle routine
	calcGyroYPR();			// Gyro Sensor Roll, Pitch, Yaw angle routine
	calcFilteredYPR();	// Complimentary filter to Roll, Pitch, Yaw angle routine
	
	calcYPRtoDualPID(); // dual PID routine
	calcMotorSpeed(); 	// Calculate motor speed using the PID
	
}





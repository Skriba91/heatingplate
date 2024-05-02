#ifndef PID_H
#define PID_H

typedef struct {

	/* Controller gains */
	float Kp;
	float Ki;
	float Kd;

	/* Derivative low-pass filter time constant */
	float tau;

	/* Output limits */
	float limMin;
	float limMax;
	
	/* Integrator limits */
	float limMinInt;
	float limMaxInt;

	/* Sample time (in seconds) */
	float T;

	/* Controller "memory" */
	float integrator;			
	float differentiator;

    float lastError;
	float lastMeasurement;		

	/* Controller output */
	float out;

} PID;

void  PID_Init(PID *pid);


float PID_Update(PID *pid, float target, float measurement);

#endif //PID_H
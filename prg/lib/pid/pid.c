#include "pid.h"

void PID_Init(PID *pid) {

    //Initialise PID variables
    pid->lastError          = 0.0f;
    pid->lastMeasurement    = 0.0f;

    pid->differentiator = 0.0f;
    pid->integrator     = 0.0f;

    pid->out = 0.0f;
}

float PID_Update(PID *pid, float target, float measurement) {
    
    //Calculating error
    float error = target - measurement;

    // P
    float proportional = pid->Kp * error;

    // I
    pid->integrator = pid->integrator + 0.5f * pid->Ki * pid->T * (error + pid->lastError);

    // Anti-wind-up with integrator clamping
    float intLimMin;
    float intLimMax;

    //Calculate integrator limits
    if (pid->limMax > proportional) {
        
        intLimMax = pid->limMax - proportional;

    } else {

        intLimMax = 0.0f;

    }

    if (pid->limMin < proportional) {

        intLimMin = pid->limMin - proportional;

    } else {

        intLimMin = 0.0f;

    }

    //Clamp integrator
    if (pid->integrator > intLimMax) {
        
        pid->integrator = intLimMax;

    } else if (pid->integrator < intLimMin) {
        
        pid->integrator = intLimMin;

    }

    pid->differentiator = -(2.0f * pid->Kd * (measurement - pid->lastMeasurement)	/* Note: derivative on measurement, therefore minus sign in front of equation! */
                        + (2.0f * pid->tau - pid->T) * pid->differentiator)
                        / (2.0f * pid->tau + pid->T);

    //Compute output
    pid->out = proportional + pid->integrator + pid->differentiator;

    if (pid->out > pid->limMax) {

        pid->out = pid->limMax;

    } else if (pid->out < pid->limMin) {

        pid->out = pid->limMin;

    }

    //Saving 
    pid->lastError = error;
    pid->lastMeasurement = measurement;

    //Returning calculated output
    return pid->out;

}
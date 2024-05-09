#include <stdio.h>
#include "pid.h"
#include "stepresonse.h"

/* Controller parameters */
#define PID_KP  18.3617f
#define PID_KI  0.53991f
#define PID_KD  139.1247

#define PID_TAU 0.2f

#define PID_LIM_MIN 0.0f
#define PID_LIM_MAX  2000.0f

#define SAMPLE_TIME_S 1.0f

#define STEP_RESPONSE_SIZE 396

int main() {
    
    /* Initialise PID controller */
    PID pid = { PID_KP, PID_KI, PID_KD,
                PID_TAU,
                PID_LIM_MIN, PID_LIM_MAX,
                SAMPLE_TIME_S };

    PID_Init(&pid);

    /* Simulate response using test system */
    float setpoint = 240.0f;
    printf("measurement: %f\tPID: %f\r\n", stepresponse[0], pid.out);
    for (unsigned i = 0; i < STEP_RESPONSE_SIZE - 1; ++i) {
        float measurement = stepresponse[i];
        PID_Update(&pid, setpoint, measurement);
        printf("Time: %d\tSetpoint: %f\tTemp: %f\tI: %f\tD: %f\tPID: %f\r\n", i, setpoint, measurement, pid.integrator, pid.differentiator, pid.out);

    }

    return 0;
}
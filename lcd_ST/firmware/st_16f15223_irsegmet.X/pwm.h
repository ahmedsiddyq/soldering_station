// pwm.h
#ifndef PWM_H
#define PWM_H
#include <xc.h>
#include <stdint.h>
 
void PWM_Init(void);                  // fixed 1 kHz on RC5 (heater_gate)
void PWM_SetDuty(uint16_t permille);  // 0..1000  (0.0% .. 100.0%)



#endif
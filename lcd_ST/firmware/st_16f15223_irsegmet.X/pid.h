
#ifndef PID_H
#define PID_H
#include <xc.h>
#include <stdint.h>
#include "adc.h"
#include "pwm.h"
int32_t i_PID = 0;
#define S  20
#define KI 1
#define KP 5
#define temp     ADC_Read(ADC_CH_TEMP)
#define CURRENT  ADC_Read(ADC_CH_CURRENT)
#define set_temp ADC_Read(ADC_CH_TEMP_SET)
 
void tempSit(uint16_t tempNa);
#endif
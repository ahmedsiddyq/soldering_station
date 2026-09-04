#include "pid.h"
void tempSit(){ 
int32_t temp_now = (int32_t)ADC_Read(ADC_CH_TEMP);
int32_t set_now  = (int32_t)ADC_Read(ADC_CH_TEMP_SET);

int32_t err = set_now - temp_now;

int32_t p = err * KP;

int32_t iterm_now = (i_PID * KI) / S;
int32_t duty_check = iterm_now + p;

if (!((duty_check >= 1000 && err > 0) ||
      (duty_check <= 0 && err < 0)))
{
    i_PID += err;

    if (i_PID > (1000L * S / KI))
        i_PID = 1000L * S / KI;

    if (i_PID < 0)
        i_PID = 0;
}

int32_t iterm = (i_PID * KI) / S;
int32_t duty = iterm + p;

if (duty > 1000)
    duty = 1000;

if (duty < 0)
    duty = 0;

PWM_SetDuty((uint16_t)duty);
}


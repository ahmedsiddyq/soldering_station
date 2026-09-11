#include <stdint.h>
#include "header.h"

#define _XTAL_FREQ 32000000UL

#define heater_gate RC5
uint8_t v=5;
uint16_t x=0;
void main(void)
{
    pins();
  UART_Init(9600);
  ADC_Init();
 PWM_Init();
 I2C1_Init();
 display_init();


PWM_SetDuty(350);   // 35.0% duty at a fixed 1 kHz
uint16_t raw_temp    = ADC_Read(ADC_CH_TEMP);
uint16_t raw_setpt   = ADC_Read(ADC_CH_TEMP_SET);
uint16_t raw_current = ADC_Read(ADC_CH_CURRENT);

 
    while (1)
    {
 
        __delay_ms(100);
        tempSit();
        display_write_number(ADC_Read(ADC_CH_TEMP));
 
    }
}
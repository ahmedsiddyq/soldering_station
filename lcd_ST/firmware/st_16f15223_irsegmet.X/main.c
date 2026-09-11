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
 uint16_t srt_temp = set_temp;

    while (1)
    {
 
    __delay_ms(100);
    tempSit();
    raw_temp = (uint16_t)((ADC_Read(ADC_CH_TEMP) * 10UL) / 22UL);
    srt_temp = (uint16_t)((set_temp * 10UL) / 22UL);
    UART_Write16String(raw_temp, " t\r\n");
    UART_Write16String(srt_temp, " st\r\n");
    display_write_number(raw_temp);
 
    }
}
#include <stdint.h>
#include "header.h"

#define _XTAL_FREQ 32000000UL

#define heater_gate RC5
uint8_t v=5;
uint16_t x=0;
uint16_t tick_1s = 0;
uint8_t tick_50ms=0;
uint8_t pid_en=0;
uint8_t SW_Dp  =0;
void main(void)
{
pins();
  UART_Init(9600);
  ADC_Init();
  Timer0_1ms_Init();
 PWM_Init();
 I2C1_Init();
 display_init();
display_set_brightness(1);

 uint16_t raw_temp    = ADC_Read(ADC_CH_TEMP);
 uint16_t srt_temp = set_temp;
 uint16_t oldsrt_temp =0;
 int32_t dtt=0;

    while (1)
    {
 
    raw_temp = (uint16_t)((ADC_Read(ADC_CH_TEMP) * 351UL) /553UL);
    srt_temp = (uint16_t)((set_temp * 351UL) /553UL);
   
    
    if(pid_en){
  //  UART_Write16String(raw_temp, " t\r\n");
   // UART_Write16String(srt_temp, " st\r\n");
  //  display_write_number(raw_temp);
    tempSit();
    pid_en=0;
   
    if(!SW_Dp){
     // UART_Write16String(raw_temp, " t\r\n");
    //  UART_Write16String(srt_temp, " st\r\n");
    display_write_number(raw_temp);
    }
    else
    {
     display_write_number(srt_temp);
    }

    }
        
 

    
 
    
    
    
     if (tick_1ms)
    {
   tick_1ms = 0;
   tick_50ms++;
     
 //  UART_Write16String(SW_Dp, " SW_Dp1 \r\n");

        if(SW_Dp){
        tick_1s++;
  //     UART_Write16String(tick_1s, " tick_1s \r\n");
        if(3000<tick_1s)
        { 
        tick_1s=0;
        SW_Dp=0;
        }}
       
        if(50<tick_50ms)
        { 
        tick_50ms=0;
        pid_en=1;
        }
                

    dtt = srt_temp - oldsrt_temp;

    if (dtt < 0){
    dtt = -dtt;}
  //   UART_Write16String(dtt, " dtt1 \r\n");

    if (20 < dtt)
    {
  //   UART_Write16String(dtt, " dtt 2\r\n");
    oldsrt_temp = srt_temp;
    SW_Dp=1;
    }

    }
    }
}
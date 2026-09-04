// adc.h
#ifndef ADC_H
#define ADC_H
#include <xc.h>
#include <stdint.h>

// ADCON0.CHS channel codes for this device
#define ADC_CH_TEMP      5    // RA5
#define ADC_CH_TEMP_SET  4    // RA4
#define ADC_CH_CURRENT   19   // RC3

void ADC_Init(void);
uint16_t ADC_Read(uint8_t channel);

#endif
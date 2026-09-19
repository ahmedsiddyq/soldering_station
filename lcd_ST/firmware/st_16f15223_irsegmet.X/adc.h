// adc.h
#ifndef ADC_H
#define ADC_H
#include <xc.h>
#include <stdint.h>

// ADCON0.CHS channel codes for this device
#define ADC_CH_TEMP      5    // RA5 = ANA5
#define ADC_CH_TEMP_SET  4    // RA4 = ANA4
#define ADC_CH_CURRENT   21   // RC5 = ANC5

void ADC_Init(void);
uint16_t ADC_Read(uint8_t channel);

#endif
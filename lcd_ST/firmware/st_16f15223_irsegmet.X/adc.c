// adc.c
#include "adc.h"
#define _XTAL_FREQ 32000000UL

void ADC_Init(void)
{
    // Fixed Voltage Reference: 4x gain = 4.096V
    FVRCONbits.ADFVR = 0b11;
    FVRCONbits.FVREN = 1;
    while (!FVRCONbits.FVRRDY);   // wait for FVR to stabilize

    ADCON1bits.FM   = 1;          // right-justified 10-bit result
    ADCON1bits.CS   = 0b010;      // FOSC/32 -> Tad = 1us @ 32MHz
    ADCON1bits.PREF = 0b11;       // VREF+ = FVR (4.096V)

    ADCON0bits.ON = 1;            // enable ADC
}

uint16_t ADC_Read(uint8_t channel)
{
 
    ADCON1bits.PREF = (channel == ADC_CH_TEMP) ? 0b11 : 0b00;
    ADCON0bits.CHS  = channel;
    __delay_us(5);           // covers both channel-mux and reference settling
    ADCON0bits.GO = 1;
    while (ADCON0bits.GO);
    return ((uint16_t)ADRESH << 8) | ADRESL;
}
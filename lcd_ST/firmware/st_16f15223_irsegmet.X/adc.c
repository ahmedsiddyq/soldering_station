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
    ADCON0bits.CHS = channel;     // select input
    __delay_us(5);                // let sample cap acquire
    ADCON0bits.GO = 1;            // start conversion
    while (ADCON0bits.GO);        // wait for GO to clear
    return ((uint16_t)ADRESH << 8) | ADRESL;
}
// pwm.c
#include "pwm.h"

void PWM_Init(void)
{
    // Timer2 = period base for PWM3. Fosc/4 = 8MHz, /32 prescale -> 250kHz tick
    PWM3CON = 0;
    T2CLKCONbits.T2CS = 0b001;   // clock source = Fosc/4
    T2CONbits.CKPS    = 0b101;   // 1:32 prescale
    T2CONbits.OUTPS   = 0b0000;  // postscaler unused for PWM frequency
    T2PR = 249;                  // 32MHz / (4*32*250) = 1000 Hz exactly
    T2CONbits.ON = 1;            // start Timer2
 
    PWM3CONbits.POL = 0;          // active-high
    PWM3CONbits.EN  = 1;          // enable PWM3

    PWM_SetDuty(0);               // start off until you command a duty
}

void PWM_SetDuty(uint16_t permille)
{
    if (permille > 1000) permille = 1000;
    PWM3DC = permille << 6;       // 10-bit duty, left-justified in the 16-bit PWM3DC reg
}
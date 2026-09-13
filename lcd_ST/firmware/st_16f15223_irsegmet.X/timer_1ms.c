// timer_1ms.c
#include "timer_1ms.h"

// Fosc/4 = 8MHz, /8 prescale = 1MHz (1us) tick. Need 1000 ticks for 1ms.
// Reload = 65536 - 1000 = 64536 = 0xFC18
#define TMR0_RELOAD_H 0xFC
#define TMR0_RELOAD_L 0x18

volatile uint8_t tick_1ms = 0;


void Timer0_1ms_Init(void)
{
    T0CON1 = 0x43;          // T0CS=Fosc/4, synchronized, prescale 1:8
    TMR0H  = TMR0_RELOAD_H; // must write H before L (hardware buffering)
    TMR0L  = TMR0_RELOAD_L;

    PIR0bits.TMR0IF = 0;
    PIE0bits.TMR0IE = 1;
    T0CON0 = 0x90;          // 16-bit mode, TMR0 enabled

    INTCONbits.PEIE = 1;    // enable peripheral interrupts
    INTCONbits.GIE  = 1;    // enable global interrupts
}

void __interrupt() ISR(void)
{
    if (PIR0bits.TMR0IF)
    {
        PIR0bits.TMR0IF = 0;
        TMR0H = TMR0_RELOAD_H;   // reload ? H before L, every time
        TMR0L = TMR0_RELOAD_L;

        tick_1ms = 1;   

    }
    // if you enable more interrupt sources later (UART RX, ADC done, etc.),
    // add their flag checks here ? this device has only ONE ISR entry point,
    // so every interrupt source gets checked inside this same function.
}

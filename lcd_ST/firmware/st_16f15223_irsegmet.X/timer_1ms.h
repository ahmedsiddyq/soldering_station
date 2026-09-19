// timer_1ms.h
#ifndef TIMER_1MS_H
#define TIMER_1MS_H
#include <xc.h>
#include <stdint.h>

extern volatile uint8_t tick_1ms;   // set by ISR, consumed by main loop

void Timer0_1ms_Init(void);

#endif
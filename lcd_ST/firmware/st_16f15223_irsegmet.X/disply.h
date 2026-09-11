#ifndef DISPlY_H
#define DISPlY_H
#include <xc.h>
#include <stdint.h>
#define _XTAL_FREQ 32000000UL
 
#define DATA_0 0xFC
#define DATA_1 0x60
#define DATA_2 0xDA
#define DATA_3 0xF2
#define DATA_4 0x66
#define DATA_5 0xB6
#define DATA_6 0xBE
#define DATA_7 0xE0
#define DATA_8 0xFE
#define DATA_9 0xF6
#define shift_delay 30
#define CLK_PIN LATCbits.LATC0
#define DATA_PIN LATCbits.LATC1

void display(uint16_t num);


#endif
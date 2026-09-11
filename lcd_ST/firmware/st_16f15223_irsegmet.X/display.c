
#include "disply.h"
void shift_plus(void)
{   
     __delay_us(20);
     CLK_PIN = 1;
     __delay_us(shift_delay);
    CLK_PIN = 0;
     __delay_us(10);
}

void latch_and_enable(void)
{
    // Step 2: latch ? CLK held high, one DATA rising edge
    CLK_PIN = 1;
    __delay_us(shift_delay);
    DATA_PIN = 1;
    __delay_us(shift_delay);
    DATA_PIN = 0;
    __delay_us(shift_delay);

    // Step 3: output enable ? CLK held low, one DATA rising edge
    CLK_PIN = 0;
    __delay_us(shift_delay);
    DATA_PIN = 1;
    __delay_us(shift_delay);
    DATA_PIN = 0;
    __delay_us(shift_delay);
}

void digi(uint8_t num, uint8_t dig)
{
    uint8_t number;

    switch (num)
    {
        case 0: number = DATA_0; break;
        case 1: number = DATA_1; break;
        case 2: number = DATA_2; break;
        case 3: number = DATA_3; break;
        case 4: number = DATA_4; break;
        case 5: number = DATA_5; break;
        case 6: number = DATA_6; break;
        case 7: number = DATA_7; break;
        case 8: number = DATA_8; break;
        case 9: number = DATA_9; break;
        default: number = 0x00; break;
    }

    for (uint8_t i = 0; i < 4; i++)
    {
        DATA_PIN = dig & 0x01;   // output bit 0
        shift_plus();
        dig >>= 1;               // next bit
    }

    for (uint8_t i = 0; i < 6; i++)
    {
        DATA_PIN = 0;
        shift_plus();
    }

    for (uint8_t i = 0; i < 8; i++)
    {
        DATA_PIN = number & 0x01;  // output bit 0
        shift_plus();
        number >>= 1;               // next bit ? fixed from 'data'
    }

  latch_and_enable();
}

void display(uint16_t num)
{
    uint8_t d[3] = {0};

    for (uint8_t z = 0; z < 3; z++)
    {
        d[z] = num % 10;
        num /= 10;
    }

    for (uint8_t z = 0; z < 3; z++)
    {
     digi(d[z], (uint8_t)(1 << z));
    }
}
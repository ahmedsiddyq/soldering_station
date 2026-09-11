
#ifndef I2C1_H
#define I2C1_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

// I2C1 bus speed. FD650B/TM1650-class displays are rated for standard
// mode (100 kHz); do not run this part faster without checking your
// datasheet copy's AC timing table.
#define I2C1_CLOCK_HZ   100000UL

// Must match the clock actually driving the MSSP1 baud rate generator
// (i.e. Fosc, not Fosc/4). Set this to your project's real Fosc, or
// better, make sure _XTAL_FREQ is already defined by your build (MCC /
// main.c) before this header is included.
#ifndef _XTAL_FREQ
#define _XTAL_FREQ 32000000UL   // default: 32 MHz HFINTOSC (typical for this family)
#endif

void  I2C1_Init(void);

// Write 'len' bytes from 'data' to 7-bit address 'addr7'.
// Returns true if the slave ACKed the address and every data byte.
bool  I2C1_Write(uint8_t addr7, const uint8_t *data, uint8_t len);

// Convenience: write a single command/data byte.
bool  I2C1_WriteByte(uint8_t addr7, uint8_t data);

// Read 'len' bytes from 7-bit address 'addr7' into 'data'.
// Master NAKs (stops acknowledging) the final byte, per I2C spec.
bool  I2C1_Read(uint8_t addr7, uint8_t *data, uint8_t len);

#endif // I2C1_H

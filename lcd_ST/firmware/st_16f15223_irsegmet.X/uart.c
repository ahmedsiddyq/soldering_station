// uart.c
#include "uart.h"
#define _XTAL_FREQ 32000000UL

void UART_Init(uint32_t baud)
{
    BAUD1CONbits.BRG16 = 1;   // 16-bit baud rate generator
    TX1STAbits.BRGH    = 1;   // high speed
    TX1STAbits.SYNC    = 0;   // asynchronous mode

    uint32_t spbrg = (_XTAL_FREQ / (4UL * baud)) - 1;
    SP1BRGL = (uint8_t)(spbrg & 0xFF);
    SP1BRGH = (uint8_t)(spbrg >> 8);

    TX1STAbits.TX9  = 0;
    RC1STAbits.RX9  = 0;
    RC1STAbits.CREN = 1;   // enable receiver
    TX1STAbits.TXEN = 1;   // enable transmitter
    RC1STAbits.SPEN = 1;   // enable serial port (must come after PPS/TRIS setup)
}

void UART_Write(char data)
{
    while (!PIR1bits.TX1IF);   // wait until TX1REG is free
    TX1REG = data;
}

void UART_WriteString(const char *str)
{
    while (*str) UART_Write(*str++);
}

uint8_t UART_DataReady(void)
{
    return PIR1bits.RC1IF;
}

char UART_Read(void)
{
    if (RC1STAbits.OERR) {
        RC1STAbits.CREN = 0;
        RC1STAbits.CREN = 1;
    }
    while (!PIR1bits.RC1IF);
    return RC1REG;
}

void putch(char c)
{
    UART_Write(c);
}
void UART_Write16(uint16_t value)
{
    char buffer[6];   // 0 to 65535 + '\0'
    uint8_t i = 0;

    if (value == 0) {
        UART_Write('0');
        return;
    }

    while (value > 0) {
        buffer[i++] = '0' + (value % 10);
        value /= 10;
    }

    while (i > 0) {
        UART_Write(buffer[--i]);
    }
}
void UART_Write16String(uint16_t value, const char *str)
{
    UART_Write16(value);
    UART_WriteString(str);
}
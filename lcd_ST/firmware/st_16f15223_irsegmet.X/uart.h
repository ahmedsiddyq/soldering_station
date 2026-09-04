#ifndef UART_H
#define UART_H
#include <xc.h>
#include <stdint.h>

void UART_Init(uint32_t baud);
void UART_Write(char data);
void UART_WriteString(const char *str);
uint8_t UART_DataReady(void);
char UART_Read(void);

#endif
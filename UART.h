#ifndef UART_H
#define UART_H

#include <avr/io.h>
#include <stdint.h>

void UART_Init(uint16_t ubrr);          // Inicializar UART
void UART_Transmit(char data);          // Enviar un carácter
char UART_Receive(void);                // Recibir un carácter
void UART_Transmit_String(char *str);   // Enviar una cadena de caracteres
void UART_ProcessCommand(char command);

#endif

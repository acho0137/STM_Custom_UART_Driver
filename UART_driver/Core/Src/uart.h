#ifndef UART_H
#define UART_H

#include <stdint.h>

void hsi_init();
void uart_init();
void uart_transmit_byte(char data);
void uart_transmit_string(char* string);

#endif

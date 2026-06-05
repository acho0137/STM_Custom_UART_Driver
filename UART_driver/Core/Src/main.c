#include "uart.h"
#include <stdint.h>
#include <stdio.h>

int main() {
	hsi_init();
	uart_init();
	char string[] = "test UART transmit\n";

	uart_transmit_string(string);

	while(1) {

	}

	return 1;
}

#include "uart.h"
#include "stm32l432xx.h"

void hsi_init() {
	RCC->CR |= RCC_CR_HSION; //start HSI16 clock
	while(!(RCC->CR & RCC_CR_HSIRDY)); //block until clock is ready
	RCC->CCIPR |= 0x2 << RCC_CCIPR_USART2SEL_Pos; //Set HSI as USART2's clock source

	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN; //enable usart2 peripheral clock


}

void uart_init() {
	//configure GPIO
	//MODER[5:4] = 2'b10, MODER[7:6] = 2'b10; 2'b10 sets pins to AF mode
	GPIOA->MODER &= ~(GPIO_MODER_MODE2 | GPIO_MODER_MODE15); //reset bits for PA2 and 3
	GPIOA->MODER |= (0b10 << GPIO_MODER_MODE2_Pos) | (0b10 << GPIO_MODER_MODE15_Pos); //set pins to AF

	//set PA2 to USART2_TX
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL2;
	GPIOA->AFR[0] |= (0b0111 << GPIO_AFRL_AFSEL2_Pos);

	//set PA15 to USART2_RX
	GPIOA->AFR[1] &= ~GPIO_AFRH_AFSEL15;
	GPIOA->AFR[1] |= (0b0011 << GPIO_AFRH_AFSEL15_Pos);

	//configure usart2
	USART2->CR1 &= ~USART_CR1_M; //set word length to 8 data bits and 1 start bit
	USART2->BRR |= 16000000/9600; //fclk/baud-rate
	USART2->CR2 &= ~USART_CR2_STOP; //1 stop bit
	USART2->CR1 |= USART_CR1_UE; //enable UART
	USART2->CR1 |= USART_CR1_TE; //enable transmitter
}

void uart_transmit_byte(char data) {
	while(!(USART2->ISR & USART_ISR_TC)); //check if previous transmission completed
	USART2->TDR = data & 0xFF; //load data into register
}

void uart_transmit_string(char *string) {
	while(*string) {
		uart_transmit_byte(*string);
		string++;
	}
}

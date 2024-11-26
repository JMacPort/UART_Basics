#include "main.h"
#include <stdio.h>

// Prototypes
void UART_Init();

int main() {
    UART_Init();

    while(1) {
    	while(!(USART2 -> SR & (1 << 5)));
        char c = USART2 -> DR;              		// Read data

        printf("Got: %c\r\n", c);         		// Echo it back
        USART2 -> SR &= ~(1 << 6);
    }
}

void UART_Init() {
	RCC -> APB1ENR |= (1 << 17);				// Enable UART2 Clock
	RCC -> AHB1ENR |= (1 << 0);					// Enable GPIOA Clock

	GPIOA -> MODER &= ~(3 << (2 * 2));			// Set PA2 to Alternate Function
	GPIOA -> MODER |= (2 << (2 * 2));
	GPIOA -> MODER &= ~(3 << (2 * 3));			// Set PA3 to Alternate Function
	GPIOA -> MODER |= (2 << (2 * 3));

	GPIOA -> AFR[0] |= (7 << 8);				// Set function to UART2
	GPIOA -> AFR[0] |= (7 << 12);

	USART2 -> BRR = 0x0683;						// Set baud rate to 9600

	USART2 -> CR1 |= (1 << 3);					// Transmit Enabled
	USART2 -> CR1 |= (1 << 2);					// Receive Enabled
	USART2 -> CR1 |= (1 << 13);					// UART2 Enabled
}

int __io_putchar(int c) {
    while(!(USART2 -> SR & (1 << 7)));
    USART2 -> DR = c;

    while(!(USART2 -> SR & (1 << 6)));
    USART2 -> SR &= ~(1 << 6);
    USART2 -> SR &= ~(1 << 5);

    return c;
}

//int __io_getchar() {
//	while(!(USART2 -> SR & (1 << 5)));
//	return USART2 -> DR;
//}

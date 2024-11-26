#include "main.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Prototypes
void UART_Init();
char UART_Get_Char();
bool UART_Read_String(char*, int);


int main() {
    UART_Init();
    char buffer[5];

    while(1) {
       if (UART_Read_String(buffer, sizeof(buffer))) {              		// Read data
    	   printf("You typed: %s\r\n\n", buffer);
       }
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

char UART_Get_Char() {
	while(!(USART2 -> SR & (1 << 5)));
	return USART2 -> DR;
}

bool UART_Read_String(char* string, int length) {
    int i = 0;
    char c;

    while (1) {
        c = UART_Get_Char();

        if (c == '\r' || c == '\n') {
            break;
        }

        if (i >= length - 1) {
            printf("Error! Too many characters\r\n\n");
            string[0] = '\0';
            return false;
        }

        string[i++] = c;
    }

    string[i] = '\0';
    return true;
}

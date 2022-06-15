#include "serialCommunication.h"

#include <stdio.h>
#include <unistd.h>
#include "usart.h"

char * readSerialLine(char * dest, int len){

	uint8_t ch = '\0';
	int idx;
	for(idx = 0; idx < len-2 && ch != '\n'; idx++){
		ch = __io_getchar();
		dest[idx] = ch;
	}

	dest[idx] = '\0';
	return dest;

}

// Syscall related

PUTCHAR_PROTOTYPE {
	uint8_t const carriageRet = '\r';

	if(ch == '\n')
		HAL_UART_Transmit(&huart2, (uint8_t *)&carriageRet, 1, 0xFFFF);	// Because picocom

	HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);

	return ch;
}

int __io_getchar(void){

	uint8_t ch;
	HAL_UART_Receive(&huart2, &ch, 1, HAL_MAX_DELAY);

	// Return character to output
	__io_putchar(ch);

	// Translate carriage return sent by picocom to a newline character
	if(ch == '\r'){
		__io_putchar('\n');
		return '\n';
	}

	return ch;
}

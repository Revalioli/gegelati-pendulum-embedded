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

int __io_putchar(int ch) {
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

	// Translate carriage return to a newline character because some software like picocom does that
	if(ch == '\r'){
		__io_putchar('\n');
		return '\n';
	}

	return ch;
}


int _read(int file, char *ptr, int len){

	if(file == STDIN_FILENO){

		// Workaround because c/c++ standard functions will
		// try to read more characters than requested to reduce
		// the number of calls to _read(), but my implementation of
		// __io_getchar() can't check if the serial port has
		// nothing to send and will wait indefinitely
		// until it receive something.
		//
		// But this is not ideal, as standard functions will retry
		// if they did not have enough characters.
		for (int i = 0; i < len; i++){

			int ch = __io_getchar();
			*ptr++ = ch;

			// This ensure that no more than one line can be read
			// at once by any function calling _read().
			if(ch == '\n'){
				while(i < len){
					*ptr++ = '\0';
					i++;
				}
			}

		}

	}
	else{

		// Base HAL implementation
		int DataIdx;

		for (DataIdx = 0; DataIdx < len; DataIdx++)
		{
			*ptr++ = __io_getchar();
		}
	}

	return len;


}

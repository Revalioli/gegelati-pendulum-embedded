#include "serialCommunication.h"

#include <stdio.h>
#include "usart.h"

PUTCHAR_PROTOTYPE {
	HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);

	return ch;
}


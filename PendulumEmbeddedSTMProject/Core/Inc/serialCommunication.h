#ifndef INC_SERIALCOMMUNICATION_H_
#define INC_SERIALCOMMUNICATION_H_

/**
 * \brief Read one line from the serial input and store it, stop when a newline character
 * is encountered or if len-1 characters have been read.
 * 
 * The dest pointer must have enough memory, or it may lead to an undefined behaviour.
 * 
 * This function directly calls __io_getchar() as a workaround for the buffer behaviour
 * under all standard library functions such as fgets() or readline(), because the UART communication
 * with the serial port doesn't really emulate stdout buffer file.
 *
 * \param[out] dest the destination char*
 * \param[in] len maximum length of the retrieved line, '\0' and '\n' included
 * \retval dest.
 */
char * readSerialLine(char * dest, int len);



// Syscall related

/**
 * \brief Send a character to the UART2 channel, which is used for serial communication with a PC via the usb port.
 * 
 * Called by _write() function, also called by the standard library function write().
 * 
 * \param ch the character to send.
 * \return the character sent.
 */
int __io_putchar(int ch);

/**
 * \brief Read a character from the UART2 channel, which is used for serial communication with a PC via the usb port.
 * 
 * Called by _read() function, also called by the standard library function read().
 * 
 * \return the character that has been read.
 */
int __io_getchar(void);

/**
 * \brief Override base HAL implementation, read len characters from the UART2 channel
 * and store them in ptr.
 * 
 * Specifically when reading using STDIN_FILENO as file number,
 * if a newline is returned by __io_getchar() before having
 * read len characters, then remaining space in ptr is filled with \0.
 * 
 * \param file the file number.
 * \param ptr the char* to store the characters.
 * \param len the number of characters to be read.
 * \return the number of characters read.
 */
int _read(int file, char *ptr, int len);

#endif /* INC_SERIALCOMMUNICATION_H_ */

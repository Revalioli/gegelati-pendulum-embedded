#ifndef INC_SERIALCOMMUNICATION_H_
#define INC_SERIALCOMMUNICATION_H_

/**
 * \brief Read one line from the serial input and store it.
 *
 * Reading the line will end if a newline character is encountered or if len-1 characters have been read.
 * Sufficient allocated memory in dest must be assure, or it may lead to an undefined behaviour.
 *
 * \param[out] dest: the destination char*
 * \param[in] len: maximum length of the retrieved line, '\0' and '\n' included
 * \retval dest pointer.
 */
char * readSerialLine(char * dest, int len);

// Syscall related

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

int __io_getchar(void);

/**
 * \brief Override base HAM implementation.
 * 
 * Specifically when reading using STDIN_FILENO as file number,
 * if a newline is returned by __io_getchar() before having
 * read len characters, then remaining space in ptr is filled with \0.
 * 
 * \param file the file number.
 * \param ptr the char* to store the characters.
 * \param len the number of characters to be read.
 * \return int the number of characters read.
 */
int _read(int file, char *ptr, int len);

#endif /* INC_SERIALCOMMUNICATION_H_ */

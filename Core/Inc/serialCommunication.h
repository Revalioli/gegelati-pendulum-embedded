#ifndef INC_SERIALCOMMUNICATION_H_
#define INC_SERIALCOMMUNICATION_H_

/**
 * \brief Reads one line from the serial input and store it in dest.
 *
 * Reading the line will end if a newline character is encountered or if n-1 characters have been read.
 *
 * \param[out] dest: the destination string
 * \param[in] len: maximum length of the retrieve line, '\0' and '\n' included
 * \retval number of characters that have been read, NULL otherwise.
 */
char * readSerialLine(char * dest, int len);

// Syscall related

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

int __io_getchar(void);

int _read(int file, char *ptr, int len);

#endif /* INC_SERIALCOMMUNICATION_H_ */

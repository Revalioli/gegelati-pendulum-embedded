#ifndef INC_SERIALCOMMUNICATION_H_
#define INC_SERIALCOMMUNICATION_H_

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


#endif /* INC_SERIALCOMMUNICATION_H_ */

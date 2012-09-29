#ifndef SERIALTOOLS_H
#define SERIALTOOLS_H

#include <avr/io.h>

void serial_init(uint32_t baudrate);

void serial_putc(uint8_t byte);

uint8_t serial_getc(void);

inline int serial_available(void) {
  return (UCSRnA & (1<<RXCn));
}

#endif // SERIALTOOLS_H

#ifndef SERIALTOOLS_H
#define SERIALTOOLS_H

#include <avr/io.h>

void serial_init(uint32_t baudrate);

void serial_putc(uint8_t data);

uint8_t serial_getc(void);

int serial_avail(void);

#endif // SERIALTOOLS_H

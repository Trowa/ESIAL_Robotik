#ifndef SERIALTOOLS_H
#define SERIALTOOLS_H

#include <avr/io.h>

void serial_init(uint32_t baudrate);

void serial_putc(uint8_t data);


uint8_t serial_getc(void);

/**
 * Vérifie si un caractère est disponible sur la série
 * Vu la taille de la fonction, on l'inline et donc on la définie ici.
 * @return 1 si un caractère est immédiatement disponible à la lecture, sinon 0.
 */
inline int serial_available(void) {
  return (UCSRnA & (1<<RXCn));
}

#endif // SERIALTOOLS_H

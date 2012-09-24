#include "serialtools.h"

#include <avr/io.h>

#define FOSC 8000000 // Oscillateur interne à 8.0 MHz

void serial_init(uint32_t baudrate) {
  uint16_t ubbr = (FOSC/16/baudrate) - 1; //Calcul de la valeur du registre de sélection du baudrate

  UBRR0H = (uint8_t) (ubbr>>8); // paramétrage de la valeur
  UBRR0L = (uint8_t) ubbr;

  UCSR0B = (1<<RXEN0)|(1<<TXEN0); // Activation des pins RX et TX
  UCSR0C = (3<<UCSZ00)|(0<<USBS0); // 8 bits par frame, 1 bit de stop
}

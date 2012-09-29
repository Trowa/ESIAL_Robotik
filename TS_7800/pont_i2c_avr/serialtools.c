#include "serialtools.h"

#include <avr/io.h>

#define FOSC 8000000 // Oscillateur interne à 8.0 MHz

/**
 * Initialisation de la communication série
 * @param baudrate Vitesse de la communication en Bauds
 */
void serial_init(uint32_t baudrate) {
  uint16_t ubbr = (FOSC/16/baudrate) - 1; //Calcul de la valeur du registre de sélection du baudrate

  UBRR0H = (uint8_t) (ubbr>>8); // paramétrage de la valeur
  UBRR0L = (uint8_t) ubbr;

  UCSR0B = (1<<RXEN0)|(1<<TXEN0); // Activation des pins RX et TX
  UCSR0C = (3<<UCSZ00)|(0<<USBS0); // 8 bits par frame, 1 bit de stop
}

/**
 * Envoie un caractère sur la série
 * @param data Donnée à envoyer
 */
void serial_putc(uint8_t data) {
  while( !(UCSR0A & (1<<UDRE0)) ); // Attente d'une éventuelle communication non terminée
  UDR0 = data; // Met la donnée dans le buffer et commence l'envoi
}

/**
 * Récupération d'un caractère sur la série
 * La fonction est bloquante jusqu'à ce qu'il y ait un caractère à lire.
 * @return Le caractère reçu
 */
uint8_t serial_getc(void) {
  while( !(UCSR0A & (1<<RXC0)) ); // On attend que la donnée arrive
  return UDR0; // On renvoie ce qu'il y a dans le buffer
}

/**
 * Vérifie si un caractère est disponible sur la série
 * @return 1 si un caractère est immédiatement disponible à la lecture, sinon 0.
 */
inline int serial_avail(void) {
  return (UCSR0A & (1<<RXC0));
}

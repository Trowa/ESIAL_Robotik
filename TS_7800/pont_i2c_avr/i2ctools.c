#include "i2ctools.h"
#include <avr/io.h>


#define START       0x08  //TWI start
#define REP_START    0x10   //TWI repeat start
#define MT_SLA_ACK    0x18   //slave ACK has been received

/**
 * Envoie la condition "start" sur l'I2C
 * @return 0 si tout s'est bien passé
 */
int i2c_start() {
  TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN); // Start I2C
  while(!(TWCR & (1<<TWINT))); //Attente transmission
  return ((TWSR & 0xF8) != START); //Vérification d'erreur
}

/**
 * Envoie la condition "stop" sur l'I2C
 * @return 0 si tout s'est bien passé
 */
int i2c_stop() {
  TWCR = (1<<TWINT) | (1<<TWEN)| (1<<TWSTO); // Stop I2C
  return 0;
}

/**
 * Envoie un octet sur l'I2C
 * @param data Donnée à envoyer
 * @return 0 si tout s'est bien passé
 */
int i2c_write(uint8_t data) {
  TWDR = data; //On charge le registre
  TWCR = (1<<TWINT) | (1<<TWEN); //Transmission
  while (!(TWCR & (1<<TWINT))); //Attente transmission...
  return ((TWSR & 0xF8) != MT_SLA_ACK); //Vérification d'erreur
}


#include "i2ctools.h"
#include <avr/io.h>


#define START 0x08 // I2C start
#define REP_START 0x10 // I2C repeat start
#define SLA_W_ACK 0x18 // ACK adresse (SLA+W)
#define SLA_DATA_ACK 0x28 // ACK données
#define I2C_FREQ_DIV 8 //Diviseur de fréquence pour l'I2C (8 -> 250000 bauds)

/**
 * Initialise le bus I2C
 */
void i2c_init() {
  TWBR = I2C_FREQ_DIV;
}

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
 * Envoie l'adresse du périphérique ciblé sur l'I2C
 * @param data Donnée à envoyer
 * @return 0 si tout s'est bien passé
 */
int i2c_write_addr(uint8_t address) {
  TWDR = address; //On charge le registre
  TWCR = (1<<TWINT) | (1<<TWEN); //Transmission
  while (!(TWCR & (1<<TWINT))); //Attente transmission...
  return ((TWSR & 0xF8) != SLA_W_ACK); //Vérification d'erreur
}

/**
 * Envoie un octet sur l'I2C
 * @param data Donnée à envoyer
 * @return 0 si tout s'est bien passé
 */
int i2c_write_data(uint8_t data) {
  TWDR = data; //On charge le registre
  TWCR = (1<<TWINT) | (1<<TWEN); //Transmission
  while (!(TWCR & (1<<TWINT))); //Attente transmission...
  return ((TWSR & 0xF8) != SLA_DATA_ACK); //Vérification d'erreur
}


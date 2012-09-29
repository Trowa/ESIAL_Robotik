#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include "i2ctools.h"
#include "serialtools.h"

#define SERIAL_BAUDRATE 38400

// Commandes Serial
#define I2C_COMM 0x2C // Communication Serial vers I2C

// prototypes
int main(void);

int main() {

  //données I2C
  uint8_t i2c_device_addr;
  uint8_t i2c_register_addr;
  uint8_t i2c_data;

  //données Serial
  uint8_t serial_cmd;

  // Paramétrage du diviseur de fréquence d'horloge à 1,
  // pour une fréquence système maximale
  clock_prescale_set(clock_div_1);
  i2c_init();
  serial_init(SERIAL_BAUDRATE);

  //Boucle principale
  while(1) {
    if(serial_avail()) {
      serial_cmd = serial_getc();
      switch(serial_cmd) {
        case I2C_COMM:
          // A présent, on attend 3 octets depuis la série : Adresse, N° registre, donnée

          // On démarre la comm' I2C
          i2c_start();

          // Récupération et transfert de l'adresse du périphérique
          i2c_device_addr = serial_getc();
          i2c_write_addr(i2c_device_addr);

          // Récupération du n° de registre et transfert
          i2c_register_addr = serial_getc();
          i2c_write_data(i2c_register_addr);

          // Récupération de la donnée et transfert
          i2c_data = serial_getc();
          i2c_write_data(i2c_data);

          // Stop
          i2c_stop();

        default:
          ;
      }
    }
  }

  return 0;
}

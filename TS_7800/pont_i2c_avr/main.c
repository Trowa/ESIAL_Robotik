#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include "i2ctools.h"
#include "serialtools.h"

// prototypes
int main(void);


//données I2C
uint8_t device_addr;
uint8_t register_addr;
uint8_t data;

int main() {

  // Paramétrage du diviseur de fréquence d'horloge à 1,
  // pour une fréquence système maximale
  clock_prescale_set(clock_div_1);


  return 0;
}

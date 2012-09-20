#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/power.h>

int main() {

  // Paramétrage du diviseur de fréquence d'horloge à 1,
  // pour une fréquence système maximale
  clock_prescale_set(clock_div_1);


  return 0;
}

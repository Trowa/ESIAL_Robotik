#include "ts7800_gpio.h"
#include "memoryAccess.h"

#include <stdio.h>
#include <stdlib.h>

//S'il y a une erreur dans le constructeur, on appelle cette fonction
void gpioNameError(char rowName, uint8_t pinNumber) {
	fprintf(stderr, "GPIO inexistant : %c%d\n", rowName, pinNumber);
	exit(1);
}

/*
 * Construction d'une structure gpio
 * rowName peut être A, B, C, ou D
 * pinNumber est un entier entre 0 et 31
 * Toutes les combinaisons ne sont pas valides, cf. manuel ; de toute
 * façon, cette fonction va vous arrêter si vous tenter de déconner.
 */
gpio_t getGPIO(char rowName, uint8_t pinNumber) {
	
	gpio_t gpio;
	gpio.rowName = rowName;
	gpio.pinNumber = pinNumber;
	
	/*
	 * En même temps que l'on attribut les bonnes adresses, on effectue
	 * quelques vérifications ; ça limite les ardeurs de ceux qui
	 * déconnent trop en atelier et qui ne lisent pas le manuel.
	 * Accessoirement, il n'est pas prévu de tester l'écriture vers des
	 * GPIOs qui n'existent pas "juste pour voir ce que ça fait"...
	 */
	switch(rowName) {
		case 'A':
			if(pinNumber > 30) {
				gpioNameError(rowName, pinNumber);
			}
			gpio.rowOffsetData = OFFSET_ROW_A_DATA;
			gpio.rowOffsetDirection = OFFSET_ROW_A_DIRECTION;
			break;
		case 'B':
			if(pinNumber == 0 || pinNumber == 2 || pinNumber == 4 || pinNumber==8 || pinNumber==9
					|| pinNumber==28 || pinNumber==30 || pinNumber > 31) {
				gpioNameError(rowName, pinNumber);
			}
			gpio.rowOffsetData = OFFSET_ROW_B_DATA;
			gpio.rowOffsetDirection = OFFSET_ROW_B_DIRECTION;
			break;
		
		case 'C':
			if(pinNumber==0 || pinNumber > 18) {
				gpioNameError(rowName, pinNumber);
			}
			gpio.rowOffsetData = OFFSET_ROW_C_DATA;
			gpio.rowOffsetDirection = OFFSET_ROW_C_DIRECTION;
			break;
			
		case 'D':
			if(pinNumber==0 || pinNumber==8 || pinNumber==16 || pinNumber > 17) {
				gpioNameError(rowName, pinNumber);
			}
			gpio.rowOffsetData = OFFSET_ROW_D_DATA;
			gpio.rowOffsetDirection = OFFSET_ROW_D_DIRECTION;
			break;
			
		default:
			gpioNameError(rowName, pinNumber);
			break;
	}
	
	//on renvoie la structure
	return gpio;
	
}

//Direction de la GPIO
void inline setGPIOInput(gpio_t gpio) {
	setBitRegister(gpio.rowOffsetDirection, gpio.pinNumber, LOW);
}

void inline setGPIOOutput(gpio_t gpio) {
	setBitRegister(gpio.rowOffsetDirection, gpio.pinNumber, HIGH);
}

//Récupère la valeur de la GPIO
uint8_t inline getGPIOValue(gpio_t gpio) {
	return getBitRegister(gpio.rowOffsetData, gpio.pinNumber);
}

//Donne une valeur à une GPIO
void inline setGPIOValue(gpio_t gpio, uint8_t value) {
	setBitRegister(gpio.rowOffsetData, gpio.pinNumber, value);
}

#ifndef TS7800_GPIO
#define TS7800_GPIO

#include <stdint.h>

/*
 * Fonctions pour utiliser les GPIOs de la TS7800 sur le connecteur PC-104
 * A NE PAS INCLURE DIRECTEMENT DANS UN PROGRAMME : INCLURE UNIQUEMENT "ts7800io.h" !
 */

//Quelques adresses importantes
#define OFFSET_ROW_A_DATA 0x10
#define OFFSET_ROW_B_DATA 0x14
#define OFFSET_ROW_C_DATA 0x18
#define OFFSET_ROW_D_DATA 0x1C
#define OFFSET_ROW_A_DIRECTION 0x20
#define OFFSET_ROW_B_DIRECTION 0x24
#define OFFSET_ROW_C_DIRECTION 0x28
#define OFFSET_ROW_D_DIRECTION 0x2C

//Nouveau type : gpio_t
struct gpio_struct {
	char rowName; //Nom de la rangée : A, B, C ou D
	uint8_t pinNumber; //numéro de la GPIO dans la rangée
	uint8_t rowOffsetData; //Adresse (déplacement) du registre de données
	uint8_t rowOffsetDirection; //idem, pour la direction (input ou output)
};
typedef struct gpio_struct gpio_t;

//Construction de la structure, avec quelques vérifications
gpio_t getGPIO(char rowName, uint8_t pinNumber);

//Direction de la GPIO
void inline setGPIOInput(gpio_t gpio);
void inline setGPIOOutput(gpio_t gpio);

//getter et setter de la valeur de la GPIO
uint8_t inline getGPIOValue(gpio_t gpio) ;
void inline setGPIOValue(gpio_t gpio, uint8_t value);

#endif

#ifndef TS7800IO
#define TS7800IO

#include <stdint.h>
#include "ts7800_gpio.h"
//#include "ts7800_can.h" <- fichier à construire !

/*
 * Fonctions permettant d'utiliser les LEDs de la TS7800
 * Ce fichier inclut aussi les fichiers "ts7800_gpio.h"
 * et "ts7800_can.h", donc c'est le seul et unique fichier que vous
 * devez inclure dans votre programme.
 */

//Définition des valeurs des bits
#define HIGH 1
#define LOW 0

//Fonction à appeler avant toutes les autres, sous peine de segfault
void initIO();

//Gestion de la LED verte
void inline setGreenLedOn();
void inline setGreenLedOff();

//Gestion de la LED rouge
void setRedLedOn();
void setRedLedOff();


#endif

#ifndef MEMORY_ACCESS
#define MEMORY_ACCESS

#include <stdint.h>

/*
 * Permet les accès directs en mémoire. A ne pas utiliser directement
 * dans une application, utiliser plutôt les fonctions décrites dans
 * "ts7800io.h" et confrères, qui, au moins, sont testées.
 */
 
//Définition des valeurs des bits
#define HIGH 1
#define LOW 0
 
//initialisation de l'accès mémoire
void memoryInit();

//sortie gentil du programme après un signal
void exit_gracefully();

//sélection d'une adresse sur le bus I2C de l'avr
void inline twi_select(uint8_t addr, uint8_t dir);

//écriture d'une donnée sur le bus I2C de l'avr
int inline twi_write(uint8_t dat);

//fin de communication sur le bus I2C de l'avr
void inline twi_stop();

//écrit une donnée à l'adresse indiqué vers l'avr, utilise l'I2C
int twiWriteToAddress(uint8_t adress, uint8_t data);

//change la valeur d'un bit dans un registre mémoire
//Ecrit dans le registre désigné par regOffest. Ecrit en réalité à
//l'adresse 0xE8000000 + regOffset. (RTFM)
int setBitRegister(uint8_t regOffset, uint8_t bitOffset, uint8_t newBitValue);

//Récupère la valeur d'un bit dans un registre.
uint8_t getBitRegister(uint8_t regOffset, uint8_t bitOffset);

#endif


#include <avr/io.h>
#include <avr/interrupt.h> 
#include <avr/power.h>

#define DD_SCK     PINB5
#define DD_MISO     PINB4
#define DD_MOSI     PINB3
#define DDR_SPI    DDRB

//Routines de débug : passent la pin 28 (PC5) à haut ou bas
#define DEBUG_UP (PORTC |= (1 << 5))
#define DEBUG_DOWN (PORTC &= ~(1 << 5))


//Permet de choisir entre l'encodage 4x et l'encodage 2x
//#define _4XENCODING

/*
 * Pour mémoire :
 * Codeur 1A : pin PD3 - INT1
 * Codeur 1B : pin PD5 - PCIE2
 * Codeur 2A : pin PD2 - INT0
 * Codeur 2B : pin PB6 - PCIE0
 */

volatile int8_t count1=0x00;
volatile int8_t count2=0x00;
volatile int8_t oldCount2=0x00;


// Gestionnaire d'interruptions SPI
ISR (SPI_STC_vect)
{	
	//Quand on arrive ici, l'ancien compte du codeur 1 vient d'être transmis

	SPCR &= ~(1<<SPIE); //On désactive l'interruption du SPI
	sei(); //On réactive les interruptions, à présent uniquement pour les tics codeurs

	SPDR = oldCount2; //On charge le registre de données du SPI (SPDR) avec l'ancien compte codeur 2
	
	while(!(SPSR & (1<<SPIF))); //On attend que le compte codeur 2 soit transmis
	

	cli(); //On désactive les interruptions (section critique)

	oldCount2 = count2; //On sauvegarde la valeur du compte codeur 2
	
	uint8_t tmp = SPSR; //On accède au registre SPSR pour effacer le bit SPIF de statut de la communication SPI
	SPDR = count1; //On charge SPDR avec le compte codeur 1 courant, qui sera envoyé à la prochaine itération

	//On reset le compte des codeurs
	count1 = 0;
	count2 = 0;

	//On réactive l'interruption du SPI
	SPCR |= (1<<SPIE);

	//Les interruptions sont réactivées automatiquement à la sortie du gestionnaire d'interruptions
}



//Gestionnaires d'interruptions Codeurs

	ISR(INT1_vect) { //Codeur 1A - PIN PD3
		if(PIND & (1 << PD3)) {
			if(PIND & (1 << PD5)) {
				count1++;
			} else {
				count1--;
			}
		} else {
			if(PIND & (1 << PD5)) {
				count1--;
			} else {
				count1++;
			}
		}
	}

	ISR(INT0_vect) { //Codeur 2A - PIN PD2
		if(PIND & (1 << PD2)) {
			if(PINB & (1 << PB6)) {
				count2++;
			} else {
				count2--;
			}
		} else {
			if(PINB & (1 << PB6)) {
				count2--;
			} else {
				count2++;
			}
		}
	}

//Si on est en encodage 4x, on gère aussi les interruptions sur les pins B
#ifdef _4XENCODING
	ISR(PCINT2_vect) { //Codeur 1B - PIN PD5
		if(PIND & (1 << PD5)) {
			if(PIND & (1 << PD3)) {
				count1--;
			} else {
				count1++;
			}
		} else {
			if(PIND & (1 << PD3)) {
				count1++;
			} else {
				count1--;
			}
		}
	}


	ISR(PCINT0_vect) { //Codeur 2B - PIN PB6
		if(PINB & (1 << PB6)) {
			if(PIND & (1 << PD2)) {
				count2--;
			} else {
				count2++;
			}
		} else {
			if(PIND & (1 << PD2)) {
				count2++;
			} else {
				count2--;
			}
		}
	}

#endif

int main() {

	clock_prescale_set(clock_div_1); //Paramétrage du diviseur de fréquence d'horloge à 1, pour une fréquence système maximale
	
	//Initialisation du SPI en mode esclave
	DDR_SPI = (1<<DD_MISO); //MISO (Master Input - Slave Output) en output, le reste en input
	SPCR = (1<<SPE)|(1<<CPOL)|(1<<SPIE)|(1<<CPHA); //Paramétrage et activation du SPI
	
	
	EICRA = 0b00000101;	//INT0 et INT1 activées sur tous les fronts

	EIMSK = 0b00000011; //Active INT0 et INT1
	
	//Si en encodage 4x
	#ifdef _4XENCODING
		PCICR = 0b00000101; //Active PCIE0 (PCINT0..7) et PCIE2 (PCINT16..23)
	
		PCMSK2 = 0b00100000; //Active PCINT21
		PCMSK0 = 0b01000000; //Active PCINT6
	#endif
		
	DDRD = 0b00000000; //PD2, 3, 5 en Input (et le reste aussi parce que OSEF)
	//DDRB |= 0b00000000; //PB6 en Input (on met un OU parce que DDRB = DDR_SPI)

	
	DDRC = 0b00100000; //Débug : pin 28 (PC5) en output

	
	PORTD = 0b00101100; //PD2, 3, 5 en Pull-up active
	PORTB |= 0b01000000; //PB6 en Pull-up active

	sei(); //Activation des interruptions
	
	
	while(1) 
	{
		//On ne fait rien ensuite, on ne gère que les interruptions
	}

}

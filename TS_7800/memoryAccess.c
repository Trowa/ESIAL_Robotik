#include "memoryAccess.h"
#include "twsi.h"
#include <sys/mman.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <assert.h> 
#include <fcntl.h>

uint32_t *twi_regs = NULL; //registres I2C vers l'avr
uint32_t *regs = NULL; //registres du FPGA, dont les GPIOs

//Pointeurs vers certains registres intéressants
volatile uint32_t *data, *control, *status;

//Initialisation de l'accès mémoire
void memoryInit() {
	
	int memoryDevice = -1;
	memoryDevice = open("/dev/mem", O_RDWR|O_SYNC); //On ouvre le périphérique mémoire
	if(memoryDevice == -1) {
		perror("Impossible d'accéder à la mémoire");
		fprintf(stderr, "Avez-vous les droits superutilisateur ?\n");
		exit(1);
	}
	
	//pointeurs vers des pages mémoires intéressantes
	twi_regs = (uint32_t *)mmap(0, getpagesize(), PROT_READ|PROT_WRITE, MAP_SHARED, memoryDevice, 0xF1011000);
	regs = (uint32_t *)mmap(0, getpagesize(), PROT_READ|PROT_WRITE, MAP_SHARED, memoryDevice,  0xe8000000);
	
	//pointeurs vers des registres cools
    control = &twi_regs[CONTROL];
    data = &twi_regs[DATA];
    status = &twi_regs[STATUS];
	
}

void exit_gracefully() {
	twi_stop();
	exit(0);
}

void inline twi_select(uint8_t addr, uint8_t dir) {
	volatile int i;
	uint32_t s;

	do {
		*control = START|TWSIEN;        // Send start signal
		for(i = 0; i < 1000; i++);

		while((*control & IFLG) == 0);

	} while((*status == MT_START) || (*status == MT_REP_START)); // retry forever

	*data = addr|dir;     // Send SLA+W/R

	/* XXX: The following for() delay is ABSOLUTELY NECESSARY otherwise
	 * the Marvel TWSI controller shifts out wrong data as the SLA+W.
	 * There is no official documentation or errata regarding this but
	 * there is a clue in Marvell supplied TWSI Linux code: 
	 * arch/arm/mach-mv88fxx81/Soc/mvTwsi.c line 276 after it adds an 
	 * arbitrary 1ms delay with a comment: "Wait 1 milli to prevent TWSI
	 * write after write problems"
	 *  
	 * The below for() loop however was the only one that seemed
	 * necessary for proper operation while testing.  However, after
	 * discovering that comment of code, delays were added after EVERY
	 * TWSI register write.  
	 *
	 * The number 60 is precisely 2x the required number of 30
	 * discovered during testing.  --joff
	 */
	for(i = 0; i < 60; i++);

	*control = ACK|TWSIEN;
	for(i = 0; i < 60; i++);

	while ((*control & IFLG) == 0);
	s = *status;
	assert((s == MT_SLA_ACK) || (s == MR_SLA_ACK)); 
}

int inline twi_write(uint8_t dat) {
	volatile int i;
	uint32_t s;

	*data = dat;
	for(i = 0; i < 60; i++);

	*control = ACK;
	for(i = 0; i < 60; i++);

	while((*control & IFLG) == 0x0);        // Wait for an I2C event

	s = *status;
	if(!(s == ST_DATA_ACK) || (s == MT_DATA_ACK )) {
		return -1;
	}

	return 0;
}

void inline twi_stop() {
	*control = STOP|TWSIEN; //Send stop signal
}

int twiWriteToAdress(uint8_t adress, uint8_t data) {

	int error = 0;
	
	twi_select(AVR_ADDR, WRITE); //on écrit vers l'AVR
	error = twi_write(OTP_W);
	if(error == 0) {
		error = twi_write(adress); //on écrit l'adresse
	}
	if(error == 0) {
		twi_write(data); //on écrit la donnée
	}
	
	twi_stop(); //on arrête la communication
	
	return error;

}

int setBitRegister(uint8_t regOffset, uint8_t bitOffset, uint8_t newBitValue) {
	
	//Quelques contrôles
	if(newBitValue != LOW && newBitValue != HIGH) {
		fprintf(stderr, "Un bit ne prend pour valeur que LOW et HIGH, pas %d !\n", newBitValue);
		return -1;
	}
	if(bitOffset >= 32) {
		fprintf(stderr, "Les registres ont une taille de 32 bits : %d : valeur hors limites !\n", bitOffset);
		return -1;
	}
	
	uint32_t *regToWrite = &regs[(regOffset / sizeof(uint32_t))]; //regsitre
	
	//écriture du bit
	if(newBitValue == HIGH) {
		*regToWrite |= 1 << bitOffset;
	} else if(newBitValue == LOW) {
		*regToWrite &= !(1 << bitOffset);
	}
	
	return 0;
}

uint8_t getBitRegister(uint8_t regOffset, uint8_t bitOffset) {
	
	//contrôle
	if(bitOffset >= 32) {
		fprintf(stderr, "Les registres ont une taille de 32 bits : %d : valeur hors limites !\n", bitOffset);
		return -1;
	}
	
	uint32_t *regToRead = &regs[(regOffset / sizeof(uint32_t))]; //registre
	
	uint8_t value = (*regToRead & (1 << regOffset)) ? 1 : 0; //on récupère le bit
	return value;
}

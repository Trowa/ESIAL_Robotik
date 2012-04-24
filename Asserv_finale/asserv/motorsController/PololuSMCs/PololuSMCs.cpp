#include "PololuSMCs.h"

//deux pololuSMCs sur deux series differentes
PololuSMCs::PololuSMCs(PinName smcSerialTXG, PinName smcSerialRXG, PinName smcSerialTXD, PinName smcSerialRXD) {

    //Tout est fait dans les constructeurs des PololuSMC
    smcSerial = NULL;
    motorG =  new OnePololuSMC(smcSerialTXG, smcSerialRXG);
    motorD =  new OnePololuSMC(smcSerialTXD, smcSerialRXD);
    
}

//Deux pololuSMCs sur une seule serie
PololuSMCs::PololuSMCs(char idMotorG, char idMotorD, PinName smcSerialTX, PinName smcSerialRX) {

    smcSerial = new Serial(smcSerialTX, smcSerialRX);
    motorG = new OnePololuSMC(idMotorG, smcSerial);
    motorD = new OnePololuSMC(idMotorD, smcSerial);
    
    smcSerial->baud(9600); //on initialise la connexion serie
    smcSerial->putc(0xAA); //on envoie l'octet d'init, pour que les cartes detectent le bitrate
    wait(0.1);
    smcSerial->putc(0x85); //parametrage de la vitesse vers l'avant
    smcSerial->putc(0); //octet de poids faible
    smcSerial->putc(0); //octet de poids fort
    smcSerial->putc(0x83); //autorisation de demarrage
}


void PololuSMCs::vitesseG(int speed) {
	if(inverseMoteurG) {
		speed = -speed;
	}
    if(!swapMoteurs) {
        motorG->setSpeed(speed);
    } else {
        motorD->setSpeed(speed);
    }
}

void PololuSMCs::vitesseD(int speed) {
    if(inverseMoteurD) {
		speed = -speed;
	}
	if(!swapMoteurs) {
        motorD->setSpeed(speed);
    } else {
        motorG->setSpeed(speed);
    }
}

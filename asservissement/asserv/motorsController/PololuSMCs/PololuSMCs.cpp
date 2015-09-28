
/*
#include "../config/config.h"

#include "PololuSMCs.h"

//deux pololuSMCs sur deux séries différentes
PololuSMCs::PololuSMCs(PinName smcSerialTXG, PinName smcSerialRXG, PinName smcSerialTXD, PinName smcSerialRXD) {

    //Tout est fait dans les constructeurs des PololuSMC
    smcSerial = NULL;
    motorG =  new OnePololuSMC(smcSerialTXG, smcSerialRXG);
    motorD =  new OnePololuSMC(smcSerialTXD, smcSerialRXD);

}

//Deux pololuSMCs sur une seule série
PololuSMCs::PololuSMCs(char idMotorG, char idMotorD, PinName smcSerialTX, PinName smcSerialRX) {

    smcSerial = new Serial(smcSerialTX, smcSerialRX);
    motorG = new OnePololuSMC(idMotorG, smcSerial);
    motorD = new OnePololuSMC(idMotorD, smcSerial);

    smcSerial->baud(9600); //on initialise la connexion série
    smcSerial->putc(0xAA); //on envoie l'octet d'init, pour que les cartes détectent le bitrate
    wait(0.1);
    smcSerial->putc(0x85); //paramétrage de la vitesse vers l'avant
    smcSerial->putc(0); //octet de poids faible
    smcSerial->putc(0); //octet de poids fort
    smcSerial->putc(0x83); //autorisation de démarrage
}

PololuSMCs::~PololuSMCs()
{
    // On arrête les moteurs à la destruction
    vitesseG(0);
    vitesseD(0);
}

void PololuSMCs::vitesseG(int speed) {
  if(Config::inverseMoteurG) {
    speed = -speed;
  }
    if(!Config::swapMoteurs) {
        motorG->setSpeed(speed);
    } else {
        motorD->setSpeed(speed);
    }
}

void PololuSMCs::vitesseD(int speed) {
    if(Config::inverseMoteurD) {
    speed = -speed;
  }
  if(!Config::swapMoteurs) {
        motorD->setSpeed(speed);
    } else {
        motorG->setSpeed(speed);
    }
}

*/

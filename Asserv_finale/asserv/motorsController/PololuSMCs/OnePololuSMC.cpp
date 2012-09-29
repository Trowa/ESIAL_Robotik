
/*
#include "../config/config.h"

#include "OnePololuSMC.h"

//Un premier contructeur, permettant de gérer un PololuSMC sur une connexion série
OnePololuSMC::OnePololuSMC(PinName smcSerialTX, PinName smcSerialRX) {

  smcSerial = new Serial(smcSerialTX, smcSerialRX);
  id = 0;
  smcSerial->baud(9600); //on initialise la connexion série
    smcSerial->putc(0xAA); //on envoie l'octet d'init, pour que la carte détecte le bitrate
    wait(0.1);
  smcSerial->putc(0x85); //paramétrage de la vitesse vers l'avant
    smcSerial->putc(0); //octet de poids faible
    smcSerial->putc(0); //octet de poids fort
    smcSerial->putc(0x83); //autorisation de démarrage
}

//Un deuxième constructeur, pour pouvoir gérer plusieurs PololuSMC grâce à la classe PololuSMCs
OnePololuSMC::OnePololuSMC(uint8_t id, Serial* smcSerial) {
    this->smcSerial = smcSerial;
    this->id = id;
}

void OnePololuSMC::inverseMotor() {
    inverse = !inverse;
}


void OnePololuSMC::setSpeed(int speed) {

    if(inverse) {
        speed = -speed;
    }

    if(id != 0) { //si l'id n'est pas nul, on utilise le prolotole Made In Polulu
       smcSerial->putc(0xAA); //octet d'init
       smcSerial->putc(id); //on envoie l'id
    }

    if(speed >= 0) {
        smcSerial->putc( id==0 ? 0x85 : 0x05); //paramétrage de la vitesse vers l'avant
    } else {
        smcSerial->putc( id==0 ? 0x86 : 0x06);  //paramétrage de la vitesse vers l'arrière
    }

    smcSerial->putc(0); //octet de poids faible
    smcSerial->putc((uint8_t)(abs(speed)<=100 ? abs(speed) : 100));

}
*/

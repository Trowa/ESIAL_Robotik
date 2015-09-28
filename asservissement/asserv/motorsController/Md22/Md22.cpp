#include "Md22.h"

// Définition des adresses
#define md22Address 0xB0    //Adresse de la MD22 ( pour tous les switchs de mode à ON)
#define modeReg  0x00       //Registre de mode
#define moteurGReg 0x02     //Registre vitesse moteur gauche ) correspondent aux numéros
#define moteurDReg 0x01     //Registre vitesse moteur droit  ) des moteurs sur la MD22
#define accReg 0x03         //Registre d'accélération

// register value configuration
#define controlMode 0x01    //Mode (0x01 : vitesse allant de -128 to 127)
#define md22acc 0x00        //Accélération

// Constructeur d'une instance de MD22
Md22::Md22(PinName I2CsdaPin , PinName I2CsclPin)
    : i2cLink(I2CsdaPin , I2CsclPin)   // I2C link
{
    // set Mode
    i2cLink.start();
    i2cLink.write(md22Address);
    i2cLink.write(modeReg);
    i2cLink.write(controlMode);
    i2cLink.stop();

    //set Acceleration
    i2cLink.start();
    i2cLink.write(md22Address);
    i2cLink.write(accReg);
    i2cLink.write(md22acc);
    i2cLink.stop();

    // set motor speeds to 0
    i2cLink.start();
    i2cLink.write(md22Address);
    i2cLink.write(moteurGReg);
    i2cLink.write(0);
    i2cLink.stop();

    i2cLink.start();
    i2cLink.write(md22Address);
    i2cLink.write(moteurDReg);
    i2cLink.write(0);
    i2cLink.stop();

}

Md22::~Md22()
{
    // On arrête les moteurs à la destruction
    vitesseG(0);
    vitesseD(0);
}

void Md22::vitesseG(int vitMoteurG)
{
    if (Config::reglageCodeurs)
        return;

    if (Config::inverseMoteurG) {
        vitMoteurG = -vitMoteurG;
    }

    if (vitMoteurG > Config::V_MAX_POS_MOTOR) {
        vitMoteurG = Config::V_MAX_POS_MOTOR;
    } else if (vitMoteurG > 0 && vitMoteurG < Config::V_MIN_POS_MOTOR) {
        vitMoteurG = 0;
    }

    if (vitMoteurG < Config::V_MAX_NEG_MOTOR) {
        vitMoteurG = Config::V_MAX_NEG_MOTOR;
    } else if (vitMoteurG < 0 && vitMoteurG > Config::V_MIN_NEG_MOTOR) {
        vitMoteurG = 0;
    }

    i2cLink.start();
    i2cLink.write(md22Address);

    if (!Config::swapMoteurs) {
        i2cLink.write(moteurGReg);
    } else {
        i2cLink.write(moteurDReg);
    }

    this->vitMoteurG = vitMoteurG;
    i2cLink.write((int8_t)vitMoteurG);
    i2cLink.stop();
}



void Md22::vitesseD(int vitMoteurD)
{
    if (Config::reglageCodeurs)
        return;

    if (Config::inverseMoteurD) {
        vitMoteurD = -vitMoteurD;
    }

    if (vitMoteurD > Config::V_MAX_POS_MOTOR) {
        vitMoteurD = Config::V_MAX_POS_MOTOR;
    } else if (vitMoteurD > 0 && vitMoteurD < Config::V_MIN_POS_MOTOR) {
        vitMoteurD = 0;
    }

    if (vitMoteurD < Config::V_MAX_NEG_MOTOR) {
        vitMoteurD = Config::V_MAX_NEG_MOTOR;
    } else if (vitMoteurD < 0 && vitMoteurD > Config::V_MIN_NEG_MOTOR) {
        vitMoteurD = 0;
    }

    i2cLink.start();
    i2cLink.write(md22Address);

    if (!Config::swapMoteurs) {
        i2cLink.write(moteurDReg);
    } else {
        i2cLink.write(moteurGReg);
    }

    this->vitMoteurD = vitMoteurD;
    i2cLink.write((int8_t)vitMoteurD);
    i2cLink.stop();
}



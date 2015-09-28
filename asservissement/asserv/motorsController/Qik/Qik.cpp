#include "Qik.h"

// Adress definition
#define VitDposAdress 0x88
#define VitDnegAdress 0x8A
#define VitGposAdress 0x8C
#define VitGnegAdress 0x8E

Qik::Qik(PinName txPinToQik, PinName rxPinToQik)

    : qikSerial(txPinToQik, rxPinToQik)

{
    qikSerial.baud(38400);
    qikSerial.format(8 , Serial::None , 1);

    // qikSerial.putc(0xAA); //if baud rate jumper is not set

    this->vitesseD(0);
    this->vitesseG(0);


}

Qik::~Qik()
{
    // On arrête les moteurs à la destruction
    vitesseG(0);
    vitesseD(0);
}

void Qik::vitesseD(int vitMoteurD)
{
    if (Config::inverseMoteurD)
        vitMoteurD = -vitMoteurD;

    if (vitMoteurD > Config::V_MAX_POS_MOTOR)
        vitMoteurD = Config::V_MAX_POS_MOTOR;

    if (vitMoteurD < Config::V_MAX_NEG_MOTOR)
        vitMoteurD = Config::V_MAX_NEG_MOTOR;

    if (vitMoteurD >= 0) {
        if (!Config::swapMoteurs) {
            qikSerial.putc(VitDposAdress);
        } else {
            qikSerial.putc(VitGposAdress);
        }

        qikSerial.putc(vitMoteurD);
    } else {
        if (!Config::swapMoteurs) {
            qikSerial.putc(VitDnegAdress);
        } else {
            qikSerial.putc(VitGnegAdress);
        }

        qikSerial.putc(-vitMoteurD);
    }
}


void Qik::vitesseG(int vitMoteurG)
{
    if (Config::inverseMoteurG)
        vitMoteurG = -vitMoteurG;

    if (vitMoteurG > Config::V_MAX_POS_MOTOR)
        vitMoteurG = Config::V_MAX_POS_MOTOR;

    if (vitMoteurG < Config::V_MAX_NEG_MOTOR)
        vitMoteurG = Config::V_MAX_NEG_MOTOR;

    if (vitMoteurG >= 0) {
        if (!Config::swapMoteurs) {
            qikSerial.putc(VitGposAdress);
        } else {
            qikSerial.putc(VitDposAdress);
        }

        qikSerial.putc(vitMoteurG);
    } else {
        if (!Config::swapMoteurs) {
            qikSerial.putc(VitGnegAdress);
        } else {
            qikSerial.putc(VitDnegAdress);
        }

        qikSerial.putc(-vitMoteurG);
    }
}



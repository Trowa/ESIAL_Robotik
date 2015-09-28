#ifndef MOTORCONTROLLER
#define MOTORCONTROLLER

#include "mbed.h"
#include "../config/config.h"

// interface du contrôleur de moteurs

class MotorsController
{

public:
    virtual void vitesseG(int vitMoteurG) = 0;
    virtual void vitesseD(int vitMoteurD) = 0;
    virtual int getVitesseG() = 0;
    virtual int getVitesseD() = 0;
    virtual ~MotorsController() { }

    void inverseMoteurDroit() {
        Config::inverseMoteurG = !Config::inverseMoteurG;
    }
    void inverseMoteurGauche() {
        Config::inverseMoteurD = !Config::inverseMoteurD;
    }
    void echangeMoteurs() {
        Config::swapMoteurs = !Config::swapMoteurs;
    }
};

#endif

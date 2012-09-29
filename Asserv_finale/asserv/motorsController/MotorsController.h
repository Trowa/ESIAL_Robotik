#ifndef MOTORCONTROLLER
#define MOTORCONTROLLER

#include "mbed.h"
#include "../config/config.h"

// interface du contrôleur de moteurs

class MotorsController {

  public:
    virtual void vitesseG(int vitMoteurG) = 0;
    virtual void vitesseD(int vitMoteurD) = 0;

    void inverseMoteurDroit() { inverseMoteurG = !inverseMoteurG; }
    void inverseMoteurGauche() { inverseMoteurD = !inverseMoteurD; }
    void echangeMoteurs() { swapMoteurs = !swapMoteurs; }
};

#endif

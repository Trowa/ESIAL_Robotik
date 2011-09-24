#ifndef MOTORCONTROLLER
#define MOTORCONTROLLER

#include "../config.h"
#include "mbed.h"

// interface du controleur de moteurs

class MotorsController {
    
  public:
    virtual void vitesseG(int vitMoteurG){}
    virtual void vitesseD(int vitMoteurD){}
        
    void inverseMoteurDroit() { inverseMoteurG = !inverseMoteurG; }
    void inverseMoteurGauche() { inverseMoteurD = !inverseMoteurD; }
    void echangeMoteurs() { swapMoteurs = !swapMoteurs; }
        
  protected:
    bool inverseMoteurG;
    bool inverseMoteurD;
    bool swapMoteurs;
};

#endif
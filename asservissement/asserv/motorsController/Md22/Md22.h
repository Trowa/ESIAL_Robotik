#ifndef MD22
#define MD22

#include "../MotorsController.h"

// Instance d'une MD22
class Md22 : public MotorsController
{
public:

    // Constructeur
    Md22(PinName I2CsdaPin , PinName I2CsclPin);
    // Destructeur
    ~Md22();

    // Paramétrage des vitesses
    virtual void vitesseG(int vitMoteurG);
    virtual void vitesseD(int vitMoteurD);
    virtual int getVitesseG() { return vitMoteurG; }
    virtual int getVitesseD() { return vitMoteurD; }

private:
    int vitMoteurG, vitMoteurD;
    // Communication I2C entre la Mbed et la MD22
    I2C i2cLink;
};



#endif

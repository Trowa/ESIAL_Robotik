#ifndef MD22
#define MD22

#include "../MotorsController.h"

// Définition des adresses
#define md22Address 0xB0    //Adresse de la MD22 ( pour tous les switchs de mode à ON)
#define modeReg  0x00       //Registre de mode
#define moteurGReg 0x02     //Registre vitesse moteur gauche ) correspondent aux numéros
#define moteurDReg 0x01     //Registre vitesse moteur droit  ) des moteurs sur la MD22
#define accReg 0x03         //Registre d'accélération

// register value configuration
#define controlMode 0x01    //Mode (0x01 : vitesse allant de -128 to 127)
#define md22acc 0x00        //Accélération

// Instance d'une MD22
class Md22 : public MotorsController
{
    public:

        // Constructeur
        Md22( PinName I2CsdaPin , PinName I2CsclPin);
        // Destructeur
        ~Md22();

        // Paramétrage des vitesses
        virtual void vitesseG(int vitMoteurG);
        virtual void vitesseD(int vitMoteurD);

    private:

        // Communication I2C entre la Mbed et la MD22
        I2C i2cLink;
 };



#endif

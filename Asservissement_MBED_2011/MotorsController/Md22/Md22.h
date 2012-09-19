#ifndef MD22
#define MD22

#include "Md22_config.h"
#include "../MotorsController.h"

// address definition

#define md22Address 0xB0    //MD22 address (all mode switches ON)
#define modeReg  0x00       //Operating mode register
#define moteurGReg 0x02     //Left motor speed register   ) correspond to the motor
#define moteurDReg 0x01     //Right motor speed register  ) numbers on the MD22
#define accReg 0x03         //Acceleration register



// Instance of a MD22 - type MotorController
class Md22 : public MotorsController
{
    public:
    
        // Constructor
        Md22( PinName I2CsdaPin , PinName I2CsclPin);
       
        // set speed methods
        void vitesseG(int vitMoteurG);
        void vitesseD(int vitMoteurD);
        
    private:
        
        // I2C link between mbed and MD22
        I2C i2cLink;
 };
 
 #endif
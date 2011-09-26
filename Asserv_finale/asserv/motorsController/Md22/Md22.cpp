#include "Md22.h"



// Constructor for a MD22 instance
// parameters are defined in MD22_config.h
Md22::Md22( PinName I2CsdaPin , PinName I2CsclPin)
: i2cLink( I2CsdaPin , I2CsclPin ) // I2C link
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

Md22::~Md22() {
}

void Md22::vitesseG(int vitMoteurG)
{
    if(inverseMoteurG)
        vitMoteurG=-vitMoteurG;
    
    if(vitMoteurG>V_MAX_POS_MOTOR)
        vitMoteurG=V_MAX_POS_MOTOR;
    if(vitMoteurG<V_MAX_NEG_MOTOR)
        vitMoteurG=V_MAX_NEG_MOTOR;
        
    i2cLink.start();
    i2cLink.write(md22Address);
    if(!swapMoteurs) {
        i2cLink.write(moteurGReg);
    } else {
        i2cLink.write(moteurDReg);
    }
    i2cLink.write(vitMoteurG);
    i2cLink.stop();
}



void Md22::vitesseD(int vitMoteurD)
{
    
    if(inverseMoteurD)
        vitMoteurD=-vitMoteurD;
        
    if(vitMoteurD>V_MAX_POS_MOTOR)
        vitMoteurD=V_MAX_POS_MOTOR;
    if(vitMoteurD<V_MAX_NEG_MOTOR)
        vitMoteurD=V_MAX_NEG_MOTOR;
        
    i2cLink.start();
    i2cLink.write(md22Address);
    if(!swapMoteurs) {
        i2cLink.write(moteurDReg);
    } else {
        i2cLink.write(moteurGReg);
    }
    i2cLink.write(vitMoteurD);
    i2cLink.stop();
}

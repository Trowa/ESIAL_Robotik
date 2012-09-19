#ifndef MOTORS
#define MOTORS

#include <WProgram.h>
#include <CompactQik2s9v1.h>
#include <Wire.h>


/*
 *    Motors : 
 *    
 *    Classe envellope gérant les moteurs et la carte liée ( MD22, qik , .. )
 *       Ici l'implémentation est faite pour une MD22
 */


class Motors
{
   public:
     //Motors(byte addresseMoteurG, byte addresseMoteurD);
     Motors(CompactQik2s9v1 *qik);
     void init();
     
     
     void vitesseG(int val);
     void vitesseD(int val);
     
   private:
      byte md22Address;
      byte modeReg;
      byte softReg;
      byte accelReg;
      int addrMoteurD, addrMoteurG;
      
      CompactQik2s9v1* motors;
      
      
    void setMode(int mode);
    void setAcceleration(byte accel);
    void setMotorSpeed(byte motor, byte motor_speed);

  
};


#endif

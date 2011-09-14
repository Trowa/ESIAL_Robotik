#include "Motors.h"
#include <CompactQik2s9v1.h>

//Motors::Motors(byte addresseMoteurG, byte addresseMoteurD)
Motors::Motors(CompactQik2s9v1 *qik)
{
  motors = qik;
  /*
         addrMoteurD = addresseMoteurD;
         addrMoteurG = addresseMoteurG;   
  
  
        // Differentes valeurs pour dialoguer avec la MD22 
          md22Address = 0x58;
          modeReg = 0x00;
          softReg =  0x07 ;
          accelReg = 0x03;
          */
}

void Motors::init()
{
  /*
   // Init° de la MD22
        Wire.begin();
        delay(100);
        setMode(1);
        setAcceleration(0);
        
      // On est pret on demande une vitesse nulle
        vitesseG(0);
        vitesseD(0);
  */
}

void Motors::vitesseG(int val)
{
  
  if( val >= 0 )
    motors->motor0Forward(val);
  else
    motors->motor0Reverse(-val);     
}


void Motors::vitesseD(int val)
{
  
  if( val >= 0 )
    motors->motor1Forward(val);
  else
    motors->motor1Reverse(-val);
}


void Motors::setMotorSpeed(byte motor, byte motor_speed)
{
  
  
  Wire.beginTransmission(md22Address);            
  Wire.send(motor);
  Wire.send(motor_speed);
  Wire.endTransmission();
}

void Motors::setMode(int mode) // Mode de la MD22
{
  Wire.beginTransmission(md22Address);           
  Wire.send(modeReg);
  Wire.send(mode);
  Wire.endTransmission();
}

void Motors::setAcceleration(byte accel) // Acceleration geré par la MD22
{
  Wire.beginTransmission(md22Address); 
  Wire.send(accelReg);
  Wire.send(accel);
  Wire.endTransmission();
}


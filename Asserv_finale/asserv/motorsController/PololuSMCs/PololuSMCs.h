#ifndef POLOLU_SMCS
#define POLOLU_SMCS
/*
#include "../../config.h"

#include "OnePololuSMC.h"
#include "../MotorsController.h"

class PololuSMCs : public MotorsController {
  
  public:
  
  //Deux pololuSMCs sur une s�rie
  PololuSMCs(char idMotorG, char idMotorD, PinName smcSerialTX, PinName smcSerialRX);
  
  //Deux pololuSMCs sur deux s�ries
  PololuSMCs(PinName smcSerialTXG, PinName smcSerialRXG, PinName smcSerialTXD, PinName smcSerialRXD);
  
  virtual void vitesseG(int vitMoteurG);
  virtual void vitesseD(int vitMoteurD);
  
  void inverseMoteurDroit() { motorD->inverseMotor(); }
  void inverseMoteurGauche() { motorG->inverseMotor(); }
  
  private:
  
  Serial* smcSerial;
  
  OnePololuSMC* motorG;
  OnePololuSMC* motorD;
  
  
  
};
*/
#endif

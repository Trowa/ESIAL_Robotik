#ifndef POLOLU_SMCS
#define POLOLU_SMCS
/*
#include "../config/config.h"

#include "OnePololuSMC.h"
#include "../MotorsController.h"

class PololuSMCs : public MotorsController {

  public:

  //Deux pololuSMCs sur une série
  PololuSMCs(char idMotorG, char idMotorD, PinName smcSerialTX, PinName smcSerialRX);

  //Deux pololuSMCs sur deux séries
  PololuSMCs(PinName smcSerialTXG, PinName smcSerialRXG, PinName smcSerialTXD, PinName smcSerialRXD);

  ~PololuSMCs();

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

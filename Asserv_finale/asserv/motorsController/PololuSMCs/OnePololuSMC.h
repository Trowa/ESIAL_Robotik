#ifndef ONE_POLOLU_SMC
#define ONE_POLOLU_SMC

#include "mbed.h"

class OnePololuSMC {
  
 public:
  
  OnePololuSMC(PinName smcSerialTX, PinName smcSerialRX); //Un premier contructeur, permettant de gerer un PololuSMC sur une connexion serie
  OnePololuSMC(char id, Serial* smcSerial); //Un deuxieme constructeur, pour pouvoir gerer plusieurs PololuSMC grace a la classe PololuSMCs
  void setSpeed(int speed);
  void inverseMotor();
   
 private:
  
  char id;
  bool inverse;
  Serial* smcSerial;
  
};


#endif

#ifndef MAIN
#define MAIN

#include "mbed.h"
#include "../config.h"
#include "../odometrie/Odometrie.h"
#include "../motorsController/Md22/Md22.h"
//#include "../motorsController/Qik/Qik.h"
//#include "../motorsController/PololuSMCs/PololuSMCs.h"
#include "../consignController/ConsignController.h"
#include "../commandManager/CommandManager.h"
#include "../debug/DebugUDP.h"

  // Ticker pour l'interruption de la boucle d'asserv
  Ticker Live;
  void Live_isr(void);

  // M&#65533;thodes utiles
  int main();
  void resetAsserv();
  void ecouteSerie();
  void readLineFromSerie(char* buffer, int bufferSize);

  // Objets qui vont bien pour asservir le bestiau
  Odometrie *odometrie;
  MotorsController *motorController;
  ConsignController *consignController;
  CommandManager *commandManager;
  DigitalOut refLed(LED2);
  DigitalOut liveLed(LED4);
  DigitalOut gotoLed(LED3);
  #ifdef DEBUG
    DigitalOut dataLed(LED1);
    char debugLedStatus;
      DebugUDP *debugUdp;
      UDPSocket udp;
      uint64_t temps;
  #endif

#endif

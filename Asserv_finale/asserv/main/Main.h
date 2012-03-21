#ifndef MAIN
#define MAIN

#include "mbed.h"
#include "../config.h"
#include "../odometrie/Odometrie.h"
#include "../motorsController/Md22/Md22.h"
#include "../motorsController/Qik/Qik.h"
#include "../consignController/ConsignController.h"
#include "../commandManager/CommandManager.h"
#include "../debug/DebugUDP.h"

  // Ticker pour l'interruption de la boucle d'asserv
  Ticker Live;
  void Live_isr(void);

  // Méthodes utiles
  int main();
  void resetAsserv();

  // Objets qui vont bien pour asservir le bestiau
  Odometrie *odometrie;
  MotorsController *motorController;
  ConsignController *consignController;
  CommandManager *commandManager;
  #ifdef DEBUG
  	DigitalOut dataLed(LED1);
	DigitalOut refLed(LED2);
	DigitalOut receiveLed(LED3);
	DigitalOut liveLed(LED4);
	char debugLedStatus;
  	DebugUDP *debugUdp;
  	UDPSocket udp;
  	uint64_t temps;
  #endif

#endif

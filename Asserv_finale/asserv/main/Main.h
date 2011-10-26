#ifndef MAIN
#define MAIN

#include "mbed.h"
#include "../config.h"
#include "../odometrie/Odometrie.h"
#include "../motorsController/Md22/Md22.h"
#include "../consignController/ConsignController.h"
#include "../commandManager/CommandManager.h"

  // Ticker pour l'interruption de la boucle d'asserv
  Ticker Live;
  void Live_isr(void);

  // Méthodes utiles
  int main();
  void resetAsserv();
  void ecouteSerie();

  // Objets qui vont bien pour asservir le bestiau
  Odometrie *odometrie;
  MotorsController *motorController;
  ConsignController *consignController;
  CommandManager *commandManager;

#endif

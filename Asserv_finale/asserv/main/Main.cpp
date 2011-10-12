#include "Main.h"

int main() {
  // Retour PC pour controlle
  pc.baud(115200);
  pc.printf("Initialisation\n");

  // On change la priorité de l'interruption timer pour quelle soit plus basse que celle des ticks codeurs
  NVIC_SetPriority(TIMER3_IRQn, 1);

  // On initialise les objets qui vont bien pour l'asserv
  odometrie = new Odometrie();
  motorController = new Md22(p9, p10);
  consignController = new ConsignController(odometrie, motorController);
  commandManager = new CommandManager(50, consignController, odometrie);

  consignController->setQuadRamp_Dist(false);
  consignController->setQuadRamp_Angle(false);

  // On attache l'interruption timer à la méthode Live_isr
  Live.attach(Live_isr, 0.005);
}

void resetAsserv() {
  // On détruit tout les objets (sauf les moteurs, on s'en fiche de ça)
  odometrie->~Odometrie();
  consignController->~ConsignController();
  commandManager->~CommandManager();
  // Et on les refait !!!
  odometrie = new Odometrie();
  consignController = new ConsignController(odometrie, motorController);
  commandManager = new CommandManager(50, consignController, odometrie);
}

// On rafraichie l'asservissement régulièrement
void Live_isr() {   
  odometrie->refresh();      
  consignController->perform();
}

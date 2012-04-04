#include "Main.h"

int main() {
  // Retour PC pour controlle
  pc.baud(115200);
  pc.printf("Initialisation\n");

  // On change la priorité de l'interruption timer pour quelle soit plus basse que celle des ticks codeurs
  NVIC_SetPriority(TIMER3_IRQn, 1);

  // On initialise les objets qui vont bien pour l'asserv
  odometrie = new Odometrie();
  //motorController = new Qik(p9, p10);
  motorController = new PololuSMCs(p9, p10, p28, p27);
  consignController = new ConsignController(odometrie, motorController);
  commandManager = new CommandManager(50, consignController, odometrie);
  #ifdef DEBUG
  	debugUdp = new DebugUDP(commandManager,odometrie);
	dataLed = 0;
	refLed = 1;
	receiveLed = 0;
	liveLed = 0;
  #endif

  consignController->setQuadRamp_Dist(true);
  consignController->setQuadRamp_Angle(false);

  // On attache l'interruption timer à la méthode Live_isr
  Live.attach(Live_isr, 0.005);
  
  /*while(1) {
    ecouteSerie();
  }
  */
}

/*void ecouteSerie() {
  if(pc.readable()) {
    char c = 0;
    c = pc.getc();
    
    switch(c) {
      case 'g' :
        // Go 10cm
        pc.printf("consigne avant : %d\n", consignController->getDistConsigne());
        consignController->add_dist_consigne(Utils::mmToUO(odometrie,100));
        pc.printf("consigne apres : %d\n", consignController->getDistConsigne());
        break;
      case 'b' :
        // Backward 10cm
        pc.printf("consigne avant : %d\n", consignController->getDistConsigne());
        consignController->add_dist_consigne(-Utils::mmToUO(odometrie,100));
        pc.printf("consigne apres : %d\n", consignController->getDistConsigne());
        break;
      case 'l' :
        // Left 45
        consignController->add_angle_consigne(Utils::degToUO(odometrie,45));
        break;
      case 'r' :
        // Right 45
        consignController->add_angle_consigne(-Utils::degToUO(odometrie,45));
        break;
      case 'c' :
        // Carre
        commandManager->addGoTo(1000,0);
        commandManager->addGoTo(1000,1000);
        commandManager->addGoTo(0,1000);
        commandManager->addGoTo(0,0);
    }

  }
}
*/

void resetAsserv() {
  // On détruit tout les objets (sauf les moteurs, on s'en fiche de ça)
  delete odometrie;
  odometrie = NULL;
  delete consignController;
  consignController = NULL;
  delete commandManager;
  commandManager = NULL;
  // Et on les refait !!!
  odometrie = new Odometrie();
  consignController = new ConsignController(odometrie, motorController);
  commandManager = new CommandManager(50, consignController, odometrie);
}

// On rafraichie l'asservissement régulièrement
void Live_isr() {   
  odometrie->refresh();      
  consignController->perform();
  //pc.printf("alive!\n");
  #ifdef DEBUG
    temps++;
  	static int refreshPeriod = 0;
	if(refreshPeriod++==10) {
		Net::poll();
	  	if(debugUdp->getDebugSend()) {
		  	debugUdp->addData("t", (double) (temps*0.05));
		  	debugUdp->sendData();
			dataLed = 1 - dataLed;
		}
		refreshPeriod = 0;
	} else {
		dataLed = 0;
		debugUdp->dropCurrentData();
	}
	liveLed = 1 - liveLed;
  #endif
}

#include "Main.h"

int main() {
  // Retour PC pour controlle
  pc.baud(115200);
  pc.printf("Initialisation\n");
  // On initialise les objets qui vont bien pour l'asserv
  odometrie = new Odometrie();
  //motorController = new Qik(p9, p10);
  motorController = new PololuSMCs(p13, p14, p28, p27);
  consignController = new ConsignController(odometrie, motorController);
  commandManager = new CommandManager(50, consignController, odometrie);
  #ifdef DEBUG
  	debugUdp = new DebugUDP(commandManager,odometrie);
	dataLed = 0;
	refLed = 1;
	receiveLed = 0;
	liveLed = 0;
  #endif
  
  
  consignController->setQuadRamp_Dist(false);
  consignController->setQuadRamp_Angle(false);

  // On attache l'interruption timer à la méthode Live_isr
  Live.attach(Live_isr, ASSERV_REFRESH_PERIOD);
  
  //On est prêt !
  pc.printf("GOGO !");
  
  while(1) {
    ecouteSerie();
  }
  
}

void ecouteSerie() {

  double consigneValue1 = 0;
  double consigneValue2 = 0;
  char* bufferConsigne;

  if(pc.readable()) {
    char c = 0;
    c = pc.getc();
    
    switch(c) {
	  //Test de débug
      case 'z' :
        // Go 10cm
        //pc.printf("consigne avant : %d\n", consignController->getDistConsigne());
        consignController->add_dist_consigne(Utils::mmToUO(odometrie,100));
        //pc.printf("consigne apres : %d\n", consignController->getDistConsigne());
		pc.putc('z');
        break;
      case 's' :
        // Backward 10cm
        //pc.printf("consigne avant : %d\n", consignController->getDistConsigne());
        consignController->add_dist_consigne(-Utils::mmToUO(odometrie,100));
        //pc.printf("consigne apres : %d\n", consignController->getDistConsigne());
		pc.putc('s');
        break;
      case 'q' :
        // Left 45
        consignController->add_angle_consigne(Utils::degToUO(odometrie,45));
		pc.putc('q');
        break;
      case 'd' :
        // Right 45
        consignController->add_angle_consigne(-Utils::degToUO(odometrie,45));
		pc.putc('d');
        break;
	  	
	  //Arrêt d'urgence
	  case 'h': //Halte!
	    commandManager->setEmergencyStop();
		pc.putc('h');
	    break;
		
	  //Reset de l'arrêt d'urgence
	  case 'r':
	    commandManager->resetEmergencyStop();
		pc.putc('r');
		break;
		
	  case 'g': //Go : va à un point précis
	  	bufferConsigne = (char*) malloc(64);
		readLineFromSerie(bufferConsigne, 64);
	  	sscanf(bufferConsigne, "%lf#%lf", &consigneValue1, &consigneValue2); //X, Y
		commandManager->addGoTo((int64_t) consigneValue1, (int64_t) consigneValue2);
		pc.printf("g%lf#%lf\n", consigneValue1, consigneValue2);
		break;
		
	  case 'v': //aVance d'un certain nombre de mm
	    bufferConsigne = (char*) malloc(48);
		readLineFromSerie(bufferConsigne, 48);
	    sscanf(bufferConsigne, "%lf", &consigneValue1);
		commandManager->addStraightLine((int64_t) consigneValue1);
		pc.printf("v%lf\n", consigneValue1);
		break;
		
	  case 't': //Tourne d'un certain angle en degrés
	    bufferConsigne = (char*) malloc(48);
		readLineFromSerie(bufferConsigne, 48);
	    sscanf(bufferConsigne, "%lf", &consigneValue1);
	    commandManager->addTurn((int64_t) consigneValue1);
		pc.printf("t%lf\n", consigneValue1);
	    break;
		
	  case 'f': //faire Face à un point précis, mais ne pas y aller, juste se tourner
	  	bufferConsigne = (char*) malloc(64);
		readLineFromSerie(bufferConsigne, 64);
	  	sscanf(bufferConsigne, "%lf#%lf", &consigneValue1, &consigneValue2); //X, Y
		commandManager->addGoToAngle((int64_t) consigneValue1, (int64_t) consigneValue2);
		pc.printf("g%lf#%lf\n", consigneValue1, consigneValue2);
		break;
		
	  case 'p': //retourne la Position et l'angle courants du robot
	    pc.printf("x%lfy%lfa%lf\n", (double)Utils::UOTomm(odometrie, odometrie->getX()),
									(double)Utils::UOTomm(odometrie, odometrie->getY()),
									odometrie->getTheta());
	    break;
		
	  default:
	    pc.putc('n');
		break;
    }

  }
}

void readLineFromSerie(char* buffer, int bufferSize) {
	memset(buffer, 0, bufferSize);
	char c = 0;
	int writeIndex = 0;
	while(c != '\n') {
		if(pc.readable()) {
			c = pc.getc();
			buffer[writeIndex] = c;
			if(writeIndex == bufferSize-2) {
				buffer[writeIndex + 1] = '\0';
				return;
			} else {
				writeIndex++;
			}
		}
	}
	buffer[writeIndex]='\0';
}


void resetAsserv() {
  //On arrête le traitement de l'asserv
  Live.detach();

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
  #ifdef DEBUG
  	debugUdp->setNewObjectPointers(commandManager, odometrie);
  #endif
  
  //On reprend l'asserv
  Live.attach(Live_isr, ASSERV_REFRESH_PERIOD);
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


#include "CommandManager.h"


//--------------------------------------------------------
//                       COMMAND MANAGER
//--------------------------------------------------------


/*
 * Fonctions de "commandes"  externes, tout droit, tourner, arc de cercle, aller au point (x,y), ...
 */
 
 
CommandManager::CommandManager(int capacity , ConsignController *ctrlr, Odometrie *odo) {
  liste = new CMDList(capacity);
  cnsgCtrl = ctrlr;
  odometrie = odo;
  currCMD.type = CMD_NULL;
  nextCMD.type = CMD_NULL;
  emergencyStop = false;
  currentConsignFinished = true;
}

CommandManager::~CommandManager() {
	delete liste;
}

/*
 * Pour ajouter des commandes � la file, on donne la position � parcourir en mm ou en degr�, 
 * le commandManager fait les convertion en UO lui m�me
 */
bool CommandManager::addStraightLine(int64_t valueInmm) {
  return liste->enqueue( CMD_GO , Utils::mmToUO(odometrie, valueInmm), 0 );
}

bool CommandManager::addTurn(int64_t angleInDeg) {
  return liste->enqueue( CMD_TURN , Utils::degToUO(odometrie, angleInDeg) , 0);
}

bool CommandManager::addGoTo(int64_t posXInmm, int64_t posYInmm) {
  return liste->enqueue( CMD_GOTO ,Utils::mmToUO(odometrie, posXInmm) ,Utils::mmToUO(odometrie, posYInmm) );
}

bool CommandManager::addGoToAngle(int64_t posXInmm, int64_t posYInmm) {
  return liste->enqueue( CMD_GOTOANGLE ,Utils::mmToUO(odometrie, posXInmm) ,Utils::mmToUO(odometrie, posYInmm) );
}


/*
 * Fonction de mise a jour...
 */
void CommandManager::perform() {
  /*
  * On demande un arr�t d'urgence, donc on fixe la consigne � atteindre sur la position courante
  */
  if(emergencyStop) {
    cnsgCtrl->set_dist_consigne(cnsgCtrl->getAccuDist());
    cnsgCtrl->set_angle_consigne(cnsgCtrl->getAccuAngle());
    return;
  }

  /*
  * Si la QuadRampDerivee n'est pas termin�e, dans le cas d'un GoTo on ajuste la trajectoire
  * Sinon on attend tranquillement la fin
  */
  if ( !cnsgCtrl->areRampsFinished() ) { 

    if (currCMD.type == CMD_GO || currCMD.type == CMD_TURN ) { // On avance ou on tourne sur place
      return; //Dans ce cas, on attend simplement d'etre arrive :)
    } else if (currCMD.type == CMD_GOTO) { // On est en plein GoTo, donc on est entrain de se planter et on ajuste
      computeGoTo();
    } else if (currCMD.type == CMD_GOTOANGLE) { // On est en plein GoTo en angle, donc on est entrain de se planter et on ajuste
      computeGoToAngle();
    }

  /*
  * Si on a termin� la QuadRampDerivee, on passe � la commande suivante
  */
  } else {
    
    // ToDo R�fl�chir � l'enchainement de commande pour ne pas s'arr�ter au moment de calculer la suivante    
    //currCMD = nextCMD; // La consigne suivante devient la consigne courante
    //nextCMD = liste.dequeue(); // On essaye de recuperer la prochaine consigne
    currCMD = liste->dequeue(); // On essaye de recuperer la prochaine consigne

    // On vient de terminer la consigne courrante, on le signale en haut lieu
    if(currentConsignFinished == false) {
      //iaCom.printf("d\n");
      pc.printf("D sent \n");
    }
        
    currentConsignFinished = false;
        
    if( currCMD.type == CMD_NULL ) { // S'il n'y a plus de commande, on est arriv� � bon port
      currentConsignFinished = true;
      return; // Il n'y en a pas...
    }
        
    if( currCMD.type == CMD_GO ) { // On avance ou on recule de la consigne
      cnsgCtrl->add_dist_consigne(currCMD.value);
    } else if ( currCMD.type == CMD_TURN ) { // On tourne de la consigne
      cnsgCtrl->add_angle_consigne(currCMD.value);
    } else if ( currCMD.type == CMD_GOTO ) { // On appel computeGoTo qui se d�brouille pour aller en (x,y)
      computeGoTo();
    } else if (currCMD.type == CMD_GOTOANGLE) { // On appel computeGoToAngle qui se d�brouille pour s'aligner avec (x,y)
      computeGoToAngle();
    }
  }  
}

/*
 * On a une commande GoTo(x,y) avec x et y deux points dans le rep�re du robot
 * (0,0) est la position initiale du robot apr�s calage bordure OU coin de la table
 * TODO D�cider de cette connerie
 */
void CommandManager::computeGoTo(){

  double deltaX = currCMD.value - odometrie->getX(); // Diff�rence entre la cible et le robot selon X
  double deltaY = currCMD.secValue - odometrie->getY();  // Diff�rence entre la cible et le robot selon Y
  
  // On a besoin de min et max pour le calcul de l'angle entre le cap cible et le cap courant
  double max = abs(deltaX)>abs(deltaY) ? abs(deltaX) : abs(deltaY);
  double min = abs(deltaX)<=abs(deltaY) ? abs(deltaX) : abs(deltaY);

  // Valeur absolue de la distance � parcourir en allant tout droit pour atteindre la consigne
  int64_t deltaDist = 0;
  if (max != 0) {
    deltaDist = (int64_t)(max * sqrt (1.0 + ( min / max ) * ( min / max ) ));
  }
    
  // Cap que doit atteindre le robot
  double thetaCible = atan2(deltaY, deltaX);
    

  // La diff�rence entre le thetaCible (= cap � atteindre) et le theta (= cap actuel du robot) donne l'angle � parcourir
  double deltaTheta = thetaCible - odometrie->getTheta();
    
  // On ajuste l'angle � parcourir pour ne pas faire plus d'un demi-tour
  // Exemple, tourner de 200 degr�s est plus chiant que de tourner de -160 degr�s
  if ( deltaTheta > PI ){
    deltaTheta -= 2.0*PI;
  } else if (deltaTheta < -PI){
    deltaTheta += 2.0*PI;
  }
        
  /*//TODO a tester en conditions r�elles et extr�mes de mauvaises utilisations
  if ( abs(deltaTheta) < angleThreshold ) { // Si on est dans la fen�tre de d�part en angle
    // La consigne est la somme de la distance � parcourir et de notre position dans l'accu
    int64_t consigne_dist = deltaDist + cnsgCtrl->getAccuDist();
    cnsgCtrl->set_dist_consigne( consigne_dist ); // On set la consigne
  }
  // Dans tout les cas, on essaie d'atteindre le bon cap
  // La consigne a atteindre en angle est la somme du deltaTheta en UO et de l'accumulateur du r�gu
  int64_t consigne_angle = Utils::radToUO(odometrie, deltaTheta) + cnsgCtrl->getAccuAngle();
  cnsgCtrl->set_angle_consigne( consigne_angle ); // On set la consigne*/

  //  Ancienne version, juste au cas o�. A virer une fois op�rationnelle
  // On projette la distance � parcourir sur l'axe X du repaire mobile du robot
  double projectedDist = deltaDist * cos(deltaTheta);
  int64_t consigne_dist;
  if ( deltaDist < Utils::mmToUO(odometrie, returnThreshold) ){
        consigne_dist = projectedDist + cnsgCtrl->getAccuDist() ;
        cnsgCtrl->set_dist_consigne( consigne_dist );
  } else {
        cnsgCtrl->set_angle_consigne(
            Utils::radToUO(odometrie, deltaTheta) + cnsgCtrl->getAccuAngle()
            ) ; //on se met dans la bonne direction
        if( abs( thetaCible - odometrie->getTheta() ) < angleThreshold ) {
            consigne_dist = deltaDist + cnsgCtrl->getAccuDist();
        } else {
            consigne_dist = cnsgCtrl->getAccuDist();
        }
        cnsgCtrl->set_dist_consigne( consigne_dist );
  }

}

/*
 * On a une commande GoToAngle(x,y), on veut que le cap du robot pointe vers ce point
 */
void CommandManager::computeGoToAngle(){

  double deltaX = currCMD.value - odometrie->getX(); // Diff�rence entre la cible et le robot selon X
  double deltaY = currCMD.secValue - odometrie->getY();  // Diff�rence entre la cible et le robot selon Y
  
  // On a besoin de min et max pour le calcul de l'angle entre le cap cible et le cap courant
  double max = abs(deltaX)>abs(deltaY) ? abs(deltaX) : abs(deltaY);
  double min = abs(deltaX)<=abs(deltaY) ? abs(deltaX) : abs(deltaY);
    
  // Cap que doit atteindre le robot
  double thetaCible = atan2(deltaY, deltaX);
    

  // La diff�rence entre le thetaCible (= cap � atteindre) et le theta (= cap actuel du robot) donne l'angle � parcourir
  double deltaTheta = thetaCible - odometrie->getTheta();
    
  // On ajuste l'angle � parcourir pour ne pas faire plus d'un demi-tour
  // Exemple, tourner de 200 degr�s est plus chiant que de tourner de -160 degr�s
  if ( deltaTheta > PI ){
    deltaTheta -= 2.0*PI;
  } else if (deltaTheta < -PI){
    deltaTheta += 2.0*PI;
  }
        
  //TODO a tester en conditions r�elles et extr�mes de mauvaises utilisations
  // La consigne a atteindre en angle est la somme du deltaTheta en UO et de l'accumulateur du r�gu
  int64_t consigne_angle = Utils::radToUO(odometrie, deltaTheta) + cnsgCtrl->getAccuAngle();
  cnsgCtrl->set_angle_consigne( consigne_angle ); // On set la consigne
    
}

void CommandManager::setEmergencyStop() { //Gestion d'un eventuel arret d'urgence
    emergencyStop = true;
    cnsgCtrl->set_dist_consigne(cnsgCtrl->getAccuDist());
    cnsgCtrl->set_angle_consigne(cnsgCtrl->getAccuAngle());
    while(currCMD.type != CMD_NULL) {
      currCMD = liste->dequeue();
    }   
    currentConsignFinished = true;
}

void CommandManager::resetEmergencyStop() {
    emergencyStop = false;
    //cnsgCtrl->angle_Regu_On();
    //cnsgCtrl->dist_Regu_On();
}

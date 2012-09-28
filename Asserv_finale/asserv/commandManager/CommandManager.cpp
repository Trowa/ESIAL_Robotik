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
 * Pour ajouter des commandes &#65533; la file, on donne la position &#65533; parcourir en mm ou en degr&#65533;,
 * le commandManager fait les convertion en UO lui m&#65533;me
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
  * On demande un arret d'urgence, donc on fixe la consigne a atteindre sur la position courante
  */
  if(emergencyStop) {
    cnsgCtrl->set_dist_consigne(cnsgCtrl->getAccuDist());
    cnsgCtrl->set_angle_consigne(cnsgCtrl->getAccuAngle());
    while(currCMD.type != CMD_NULL) { //On s'assure que la liste des commandes est vide
      currCMD = liste->dequeue();
    }
    currentConsignFinished = true; //Du coup, on fait comme si la consigne courante était fini
    return;
  }

  /*
  * Si la QuadRampDerivee n'est pas terminee, dans le cas d'un GoTo on ajuste la trajectoire
  * Sinon on attend tranquillement la fin
  */
  if ( !cnsgCtrl->areRampsFinished() ) {

    if (currCMD.type == CMD_GO || currCMD.type == CMD_TURN ) { // On avance ou on tourne sur place
      return; //Dans ce cas, on attend simplement d'etre arrive :)
    } else if (currCMD.type == CMD_GOTO) { // On est en plein GoTo, donc on est en train de se planter et on ajuste
      computeGoTo();
    } else if (currCMD.type == CMD_GOTOANGLE) { // On est en plein GoTo en angle, donc on est en train de se planter et on ajuste
      computeGoToAngle();
    }

  /*
  * Si on a termine la QuadRampDerivee, on passe a la commande suivante
  */
  } else {

    // ToDo Reflechir a l'enchainement de commande pour ne pas s'arreter au moment de calculer la suivante
    //currCMD = nextCMD; // La consigne suivante devient la consigne courante
    //nextCMD = liste.dequeue(); // On essaye de recuperer la prochaine consigne
    currCMD = liste->dequeue(); // On essaye de recuperer la prochaine consigne

    // On vient de terminer la consigne courrante, on le signale en haut lieu
    if(currentConsignFinished == false) {
      //iaCom.printf("d\n"); //ou iaCom.putc('d');
      pc.putc('d');
      pc.putc('\n');
      //pc.printf("D sent \n");
    }

    currentConsignFinished = false;

    if( currCMD.type == CMD_NULL ) { // S'il n'y a plus de commande, on est arrive a bon port
      currentConsignFinished = true;
      return; // Il n'y en a pas...
    }

    if( currCMD.type == CMD_GO ) { // On avance ou on recule de la consigne
      cnsgCtrl->add_dist_consigne(currCMD.value);
    } else if ( currCMD.type == CMD_TURN ) { // On tourne de la consigne
      cnsgCtrl->add_angle_consigne(currCMD.value);
    } else if ( currCMD.type == CMD_GOTO ) { // On appel computeGoTo qui se debrouille pour aller en (x,y)
      computeGoTo();
    } else if (currCMD.type == CMD_GOTOANGLE) { // On appel computeGoToAngle qui se debrouille pour s'aligner avec (x,y)
      computeGoToAngle();
    }
  }
}

/*
 * On a une commande GoTo(x,y) avec x et y deux points dans le repere du robot
 * (0,0) est la position initiale du robot apres calage bordure
 * TODO decider de cette connerie
 */
void CommandManager::computeGoTo(){

  double deltaX = currCMD.value - odometrie->getX(); // Difference entre la cible et le robot selon X
  double deltaY = currCMD.secValue - odometrie->getY();  // Difference entre la cible et le robot selon Y

  // On a besoin de min et max pour le calcul de l'angle entre le cap cible et le cap courant
  double max = fabs(deltaX)>fabs(deltaY) ? fabs(deltaX) : fabs(deltaY);
  double min = fabs(deltaX)<=fabs(deltaY) ? fabs(deltaX) : fabs(deltaY);

  // Valeur absolue de la distance a parcourir en allant tout droit pour atteindre la consigne
  int64_t deltaDist = 0;
  if (max != 0) {
    deltaDist = (int64_t)(max * sqrt (1.0 + ( min / max ) * ( min / max ) ));
  }

  // Cap que doit atteindre le robot
  double thetaCible = atan2(deltaY, deltaX);


  // La difference entre le thetaCible (= cap a atteindre) et le theta (= cap actuel du robot) donne l'angle a parcourir
  double deltaTheta = thetaCible - odometrie->getTheta();

  // On ajuste l'angle a parcourir pour ne pas faire plus d'un demi-tour
  // Exemple, tourner de 200 degres est plus chiant que de tourner de -160 degres
  if ( deltaTheta > PI ){
    deltaTheta -= 2.0*PI;
  } else if (deltaTheta < -PI){
    deltaTheta += 2.0*PI;
  }

  /*//TODO a tester en conditions reelles et extremes de mauvaises utilisations
  if ( fabs(deltaTheta) < angleThreshold ) { // Si on est dans la fenetre de depart en angle
    // La consigne est la somme de la distance a parcourir et de notre position dans l'accu
    int64_t consigne_dist = deltaDist + cnsgCtrl->getAccuDist();
    cnsgCtrl->set_dist_consigne( consigne_dist ); // On set la consigne
  }
  // Dans tout les cas, on essaie d'atteindre le bon cap
  // La consigne a atteindre en angle est la somme du deltaTheta en UO et de l'accumulateur du r&#65533;gu
  int64_t consigne_angle = Utils::radToUO(odometrie, deltaTheta) + cnsgCtrl->getAccuAngle();
  cnsgCtrl->set_angle_consigne( consigne_angle ); // On set la consigne*/

  //  Ancienne version, juste au cas ou. A virer une fois operationnelle
  // On projette la distance a parcourir sur l'axe X du repaire mobile du robot
  double projectedDist = deltaDist * cos(deltaTheta);
  //printf("dd=%lld - rT=%lld - rTUO=%lld - ", deltaDist, returnThreshold, Utils::mmToUO(odometrie, returnThreshold));
  int64_t consigne_dist;
  if ( deltaDist < Utils::mmToUO(odometrie, returnThreshold) ){
        consigne_dist = projectedDist + cnsgCtrl->getAccuDist();
        cnsgCtrl->set_dist_consigne( consigne_dist );
  } else {
        cnsgCtrl->set_angle_consigne(
            Utils::radToUO(odometrie, deltaTheta) + cnsgCtrl->getAccuAngle()
            ) ; //on se met dans la bonne direction
        //pc.printf("dT=%ldd - ",Utils::radToUO(odometrie, deltaTheta) + cnsgCtrl->getAccuAngle());
        //pc.printf("Td=%f - aT=%f\n", fabs( thetaCible - odometrie->getTheta()), angleThreshold);
        if( fabs( thetaCible - odometrie->getTheta() ) < angleThreshold ) {
            consigne_dist = deltaDist + cnsgCtrl->getAccuDist();
        } else {
            consigne_dist = cnsgCtrl->getAccuDist();
        }
        cnsgCtrl->set_dist_consigne( consigne_dist );
        //pc.printf("Cd=%lld\n", consigne_dist);
  }

}

/*
 * On a une commande GoToAngle(x,y), on veut que le cap du robot pointe vers ce point
 */
void CommandManager::computeGoToAngle(){

  double deltaX = currCMD.value - odometrie->getX(); // Diff&#65533;rence entre la cible et le robot selon X
  double deltaY = currCMD.secValue - odometrie->getY();  // Diff&#65533;rence entre la cible et le robot selon Y

  // On a besoin de min et max pour le calcul de l'angle entre le cap cible et le cap courant
  double max = fabs(deltaX)>fabs(deltaY) ? fabs(deltaX) : fabs(deltaY);
  double min = fabs(deltaX)<=fabs(deltaY) ? fabs(deltaX) : fabs(deltaY);

  // Cap que doit atteindre le robot
  double thetaCible = atan2(deltaY, deltaX);


  // La diff&#65533;rence entre le thetaCible (= cap &#65533; atteindre) et le theta (= cap actuel du robot) donne l'angle &#65533; parcourir
  double deltaTheta = thetaCible - odometrie->getTheta();

  // On ajuste l'angle &#65533; parcourir pour ne pas faire plus d'un demi-tour
  // Exemple, tourner de 200 degr&#65533;s est plus chiant que de tourner de -160 degr&#65533;s
  if ( deltaTheta > PI ){
    deltaTheta -= 2.0*PI;
  } else if (deltaTheta < -PI){
    deltaTheta += 2.0*PI;
  }

  //TODO a tester en conditions r&#65533;elles et extr&#65533;mes de mauvaises utilisations
  // La consigne a atteindre en angle est la somme du deltaTheta en UO et de l'accumulateur du r&#65533;gu
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

// TODO ce truc n'a rien &#65533; foutre ici ... Il devrait &#65533;tre dans le CommandManager
void CommandManager::calageBordureGros(int sens) {

  cnsgCtrl->setLowSpeed(true);

  // On recule 2sec
  cnsgCtrl->add_dist_consigne(Utils::mmToUO(odometrie,-300));
  wait(3);
  cnsgCtrl->angle_Regu_Off(); //on coupe le régu d'angle pour s'aligner avec la bordure
  wait(2); //et on attend encore

  // On considere qu'on est contre la bordure, on reset la postion du robot
  odometrie->resetTheta();
  cnsgCtrl->reset_regu_angle();
  odometrie->resetX();
  cnsgCtrl->reset_regu_dist();

  // On remet le regulateur d'angle
  cnsgCtrl->angle_Regu_On();

  // On avance un peu pour sortir de la zone
  cnsgCtrl->add_dist_consigne(Utils::mmToUO(odometrie,600));
  wait(4);

  // En fonction de la couleur, on tourne dans un sens ou l'autre
  int mult = sens==0 ? 1 : -1;
  cnsgCtrl->add_angle_consigne(mult * Utils::degToUO(odometrie, 90));
  wait(2);

  // On recule dans la bordure
  cnsgCtrl->add_dist_consigne(Utils::mmToUO(odometrie,-800));
  wait(4);
  cnsgCtrl->angle_Regu_Off(); //on coupe le régu d'angle pour s'aligner avec la bordure
  wait(2); //et on attend encore

  // On reset l'axe Y du robot et on se decolle de la bordure
  odometrie->resetY();
  cnsgCtrl->reset_regu_dist();
  cnsgCtrl->angle_Regu_On();
  cnsgCtrl->add_dist_consigne(Utils::mmToUO(odometrie,140));
  wait(3);


  // On tourne pour entrer a nouveau dans la zone de depart
  mult = sens==0 ? -1 : 1;
  cnsgCtrl->add_angle_consigne(mult * Utils::degToUO(odometrie, 90));
  wait(2);

  // On se met en postion de d&#65533;part
  cnsgCtrl->add_dist_consigne(Utils::mmToUO(odometrie,-380));
  wait(4);

  // On remet la marche arri&#65533;re &#65533; vitesse normale
  cnsgCtrl->setLowSpeed(false);

}

void CommandManager::calageBordurePetit(int sens) {

  cnsgCtrl->setLowSpeed(true);

  // On recule 5sec sans r&#65533;gulateur d'angle
  cnsgCtrl->angle_Regu_Off();
  cnsgCtrl->add_dist_consigne(Utils::mmToUO(odometrie,-100));
  wait(5);

  // Au bout de 5sec, on consid&#65533;re qu'on est contre la bordure, on reset la postion du robot
  odometrie->resetTheta();
  cnsgCtrl->reset_regu_angle();
  odometrie->resetX();
  cnsgCtrl->reset_regu_dist();

  // On remet le r&#65533;gulateur d'angle
  cnsgCtrl->angle_Regu_On();

  // On avance un peu pour sortir de la zone
  cnsgCtrl->add_dist_consigne(Utils::mmToUO(odometrie,550));
  wait(5);

  // En fonction de la couleur, on tourne dans un sens ou l'autre
  int mult = sens==0 ? 1 : -1;
  cnsgCtrl->add_angle_consigne(mult * Utils::degToUO(odometrie, 90));
  wait(3);

  // On recule dans la bordure
  cnsgCtrl->angle_Regu_Off();
  cnsgCtrl->add_dist_consigne(Utils::mmToUO(odometrie,-300));
  wait(5);

  // On reset l'axe Y du robot et on se d&#65533;colle de la bordure
  odometrie->resetY();
  cnsgCtrl->reset_regu_dist();
  cnsgCtrl->angle_Regu_On();
  cnsgCtrl->add_dist_consigne(Utils::mmToUO(odometrie,140));
  wait(3);


  // On tourne pour entrer &#65533; nouveau dans la zone de d&#65533;part
  mult = sens==0 ? -1 : 1;
  cnsgCtrl->add_angle_consigne(mult * Utils::degToUO(odometrie, 90));
  wait(3);

  // On se met en postion de d&#65533;part
  cnsgCtrl->add_dist_consigne(Utils::mmToUO(odometrie,-500));
  wait(5);

  // On remet la marche arri&#65533;re &#65533; vitesse normale
  cnsgCtrl->setLowSpeed(false);

}

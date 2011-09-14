#include "CommandManager.h"


//--------------------------------------------------------
//                       COMMAND MANAGER
//--------------------------------------------------------


/*
 * Fonctions de "commandes"  externes, tout droit, tourner, arc de cercle, aller au point (x,y), ...
 */
 
 
CommandManager::CommandManager(int capacity , ConsignController *ctrlr) 
    : liste(capacity)
{
    cnsgCtrl = ctrlr;
    currCMD.type = CMD_NULL;
    nextCMD.type = CMD_NULL;
    emergencyStop = false;
    currentConsignFinished = true;
}


bool CommandManager::addStraightLine(int64_t valueInmm) {
    return liste.enqueue( CMD_GO , cnsgCtrl->getOdometrie()->mmToUO(valueInmm), 0 );
}

bool CommandManager::addTurn(int64_t angleInDeg) {
    return liste.enqueue( CMD_TURN , cnsgCtrl->getOdometrie()->degToUO(angleInDeg) , 0);
}

bool CommandManager::addArcCircle(int64_t angleInDeg, int64_t radiusInmm) {
    return  liste.enqueue( CMD_CIRCLE , angleInDeg ,radiusInmm ); // cnsgCtrl->getOdometrie()->degToUO(angleInDeg) ,  cnsgCtrl->getOdometrie()->mmToUO(radiusInmm) );
}

bool CommandManager::addGoTo(int64_t posXInmm, int64_t posYInmm) {
    return liste.enqueue( CMD_GOTO ,cnsgCtrl->getOdometrie()->mmToUO(posXInmm)  ,cnsgCtrl->getOdometrie()->mmToUO(posYInmm) );
}

bool CommandManager::addGoToAngle(int64_t posXInmm, int64_t posYInmm) {
    return liste.enqueue( CMD_GOTOANGLE ,cnsgCtrl->getOdometrie()->mmToUO(posXInmm)  ,cnsgCtrl->getOdometrie()->mmToUO(posYInmm) );
}


/*
 * Fonction de mise a jour...
 */
void CommandManager::perform() {

    if(emergencyStop) { //Demande d'arret d'urgence
        //TODO FOIREUX !!!!!!!!!!!!!!!!!!!!! Le robot part en avant avec l'inertie puis recule
        cnsgCtrl->set_dist_consigne(cnsgCtrl->getAccuDist());
        cnsgCtrl->set_angle_consigne(cnsgCtrl->getAccuAngle());
        /*
        cnsgCtrl->angle_Regu_Off();
        cnsgCtrl->dist_Regu_Off();
        cnsgCtrl->getMotorsController()->vitesseG(0);
        cnsgCtrl->getMotorsController()->vitesseD(0);
        */
        return;
    }

    if ( !cnsgCtrl->areRampsFinished() ) { //la consigne courante n'est pas encore finie...
        if (currCMD.type == CMD_GO || currCMD.type == CMD_TURN ) {
            return; //Dans ce cas, on attend simplement d'etre arrive :)
     
        } else if ( currCMD.type == CMD_CIRCLE ) {
            cnsgCtrl->add_angle_consigne( cnsgCtrl->getOdometrie()->degToUO(3) );
            cnsgCtrl->add_dist_consigne(  (PI*1/180) * cnsgCtrl->getOdometrie()->mmToUO( currCMD.secValue )  );
        }
        else if (currCMD.type == CMD_GOTO) 
        {
            computeGoTo();
        }
        else if (currCMD.type == CMD_GOTOANGLE)
        {
            computeGoToAngle();
        }
        
    }
    else 
    {
        //currCMD = nextCMD; // La consigne suivante devient la consigne courante
        //nextCMD = liste.dequeue(); // On essaye de recuperer la prochaine consigne
        currCMD = liste.dequeue(); // On essaye de recuperer la prochaine consigne
        
        if(currentConsignFinished == false) {
            //iaCom.printf("d\n");
            printf("D sent \n");
        }
        
        currentConsignFinished = false;
        
        if( currCMD.type == CMD_NULL ) {
            currentConsignFinished = true;
            return; // Il n'y en a pas...
        }
        
        if( currCMD.type == CMD_GO ) {
            cnsgCtrl->add_dist_consigne(currCMD.value);
            
        } else if ( currCMD.type == CMD_TURN ) {
            cnsgCtrl->add_angle_consigne(currCMD.value);
        
        } else if ( currCMD.type == CMD_CIRCLE ) {
            cnsgCtrl->add_angle_consigne( cnsgCtrl->getOdometrie()->degToUO(3) );
            cnsgCtrl->add_dist_consigne(  (PI*1/180) * cnsgCtrl->getOdometrie()->mmToUO( currCMD.secValue )  );
        
        } else if ( currCMD.type == CMD_GOTO ) {
            computeGoTo();
        }
        else if (currCMD.type == CMD_GOTOANGLE)
        {
            computeGoToAngle();
        }
    }
    
}


void CommandManager::computeGoTo(){
    double deltaX = currCMD.value    - cnsgCtrl->getOdometrie()->getX();
    double deltaY = currCMD.secValue - cnsgCtrl->getOdometrie()->getY();
    double max = abs(deltaX)>abs(deltaY) ? abs(deltaX) : abs(deltaY);
    double min = abs(deltaX)<=abs(deltaY) ? abs(deltaX) : abs(deltaY);
    double deltaDist = 0;
    if (max != 0) {
        deltaDist = max * sqrt (1.0 + ( min / max ) * ( min / max ) );
    }
    
    double thetaCible = atan2(deltaY, deltaX);
    

    // distance projetee sur la direction du robot
    float d = thetaCible - cnsgCtrl->getOdometrie()->getTheta();
    
    if ( d > PI ){
        d -= 2.0*PI;
    } else if (d < -PI){
        d += 2.0*PI;
    }
    
    double projectedDist = deltaDist * cos(d);

    int64_t consigne_d;
    
    //TODO a corriger pour ne pas partir en marche arriere
    if ( deltaDist < cnsgCtrl->getOdometrie()->mmToUO(returnThreshold) ){
        consigne_d = projectedDist + cnsgCtrl->getAccuDist() ;
        cnsgCtrl->set_dist_consigne( consigne_d );
    } else {
        cnsgCtrl->set_angle_consigne(
            cnsgCtrl->getOdometrie()->radToUO( d ) + cnsgCtrl->getAccuAngle()
            ) ; //on se met dans la bonne direction
        if( abs( thetaCible - cnsgCtrl->getOdometrie()->getTheta() ) < angleThreshold ) {
            consigne_d = deltaDist + cnsgCtrl->getAccuDist();
        } else {
            consigne_d = cnsgCtrl->getAccuDist();
        }
        cnsgCtrl->set_dist_consigne( consigne_d );
    }
   // printf("%f %f\n", cnsgCtrl->getOdometrie()->UOtoMM(projectedDist), cnsgCtrl->getOdometrie()->UOtoMM(consigne_d) );   
    
}

void CommandManager::computeGoToAngle(){
    double deltaX = currCMD.value    - cnsgCtrl->getOdometrie()->getX();
    double deltaY = currCMD.secValue - cnsgCtrl->getOdometrie()->getY();
    
    double thetaCible = atan2(deltaY, deltaX);
    

    // distance projetee sur la direction du robot
    float d = thetaCible - cnsgCtrl->getOdometrie()->getTheta();
    
    if ( d > PI ){
        d -= 2.0*PI;
    } else if (d < -PI){
        d += 2.0*PI;
    }
    cnsgCtrl->set_angle_consigne( cnsgCtrl->getOdometrie()->radToUO( d ) + cnsgCtrl->getAccuAngle() ) ; //on se met dans la bonne direction
    
   // printf("%f %f\n", cnsgCtrl->getOdometrie()->UOtoMM(projectedDist), cnsgCtrl->getOdometrie()->UOtoMM(consigne_d) );   
    
}

void CommandManager::setEmergencyStop() { //Gestion d'un eventuel arret d'urgence
    emergencyStop = true;
    cnsgCtrl->set_dist_consigne(cnsgCtrl->getAccuDist());
    cnsgCtrl->set_angle_consigne(cnsgCtrl->getAccuAngle());
    while(currCMD.type != CMD_NULL)
        currCMD = liste.dequeue();
        
    currentConsignFinished = true;
}

void CommandManager::resetEmergencyStop() {
    emergencyStop = false;
    //cnsgCtrl->angle_Regu_On();
    //cnsgCtrl->dist_Regu_On();
}
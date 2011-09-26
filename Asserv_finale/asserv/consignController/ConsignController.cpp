#include "ConsignController.h"

//#define DEBUG
#ifdef DEBUG
    #include "../UDPData.h"
#endif

ConsignController::ConsignController(Odometrie *odo, MotorsController *mot ) 
  : angle_regu(false), dist_regu(true) {
    
  // Au depart on se place a (0,0)
  angle_consigne = 0;
  dist_consigne = 0;

  odometrie = odo;
  motors = mot;

  // Les 2 regu sont actifs par defaut
  angle_regu_on = true;
  dist_regu_on = true;

  // Heu ... Pk on désactive la QuadRampDerivee en angle ????
  angle_regu.setfiltreQuadRampDeriveeON(false);
    
}

ConsignController::~ConsignController() {}

void ConsignController::add_angle_consigne(int64_t delta) {
    angle_consigne += delta;
}

void ConsignController::add_dist_consigne(int64_t delta) {
    dist_consigne += delta;
}

void ConsignController::set_angle_consigne(int64_t consigne) {
    angle_consigne = consigne;
}

void ConsignController::set_dist_consigne(int64_t consigne) {
    dist_consigne = consigne;
}

void ConsignController::calage_bordure(int sens) {
    
  dist_regu.setVitesseMarcheArriere(DIST_QUAD_1ST_NEG/2 );
    
  // On recule 5sec sans régulateur d'angle
  angle_Regu_Off();
  add_dist_consigne(Utils::mmToUO(odometrie,-100));
  wait(5);
  
  // Au bout de 5sec, on considère qu'on est contre la bordure, on reset la postion du robot
  odometrie->resetTheta();
  reset_regu_angle();
  odometrie->resetX();
  reset_regu_dist();
    
  // On remet le régulateur d'angle
  angle_Regu_On();
  
  // On avance un peu pour sortir de la zone
  add_dist_consigne(Utils::mmToUO(odometrie,550));
  wait(5);
    
  // En fonction de la couleur, on tourne dans un sens ou l'autre
  int mult = sens==0 ? 1 : -1;
  add_angle_consigne(mult * Utils::degToUO(odometrie, 90));
  wait(3);
    
  // On recule dans la bordure
  angle_Regu_Off();
  add_dist_consigne(Utils::mmToUO(odometrie,-300));
  wait(5);
    
  // On reset l'axe Y du robot et on se décolle de la bordure
  odometrie->resetY();
  reset_regu_dist();
  angle_Regu_On();
  add_dist_consigne(Utils::mmToUO(odometrie,140));
  wait(3);
    
    
  // On tourne pour entrer à nouveau dans la zone de départ  
  mult = sens==0 ? -1 : 1;
  add_angle_consigne(mult * Utils::degToUO(odometrie, 90));
  wait(3);
    
  // On se met en postion de départ
  add_dist_consigne(Utils::mmToUO(odometrie,-500));
  wait(5);

  // On remet la marche arrière à vitesse normale
  dist_regu.setVitesseMarcheArriere(DIST_QUAD_1ST_NEG );

}

void ConsignController::perform() {
  
  int64_t dist_output = 0;
  int64_t angle_output = 0;
  
  if ( angle_regu_on ) {
    angle_output = angle_regu.manage(angle_consigne, odometrie->getDeltaThetaBrut() );
  }
  if ( dist_regu_on ) {
    dist_output = dist_regu.manage(dist_consigne, odometrie->getDeltaDist() );
  }

  //Calcul des vitesses a appliquer
  int64_t VmoteurD = Utils::constrain( dist_output + angle_output , V_MAX_NEG_MOTOR , V_MAX_POS_MOTOR);
  int64_t VmoteurG = Utils::constrain( dist_output - angle_output, V_MAX_NEG_MOTOR , V_MAX_POS_MOTOR);

  // On donne l'ordre aux moteurs
  motors->vitesseG(VmoteurG);
  motors->vitesseD(VmoteurD);
  
}

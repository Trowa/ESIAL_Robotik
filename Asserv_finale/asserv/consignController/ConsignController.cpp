#include "ConsignController.h"

//#define DEBUG
#ifdef DEBUG
    #include "../debug/DebugUDP.h"
#endif

// Constructeur prenant deux objets initialiser avec l'asserv en paramètre
ConsignController::ConsignController(Odometrie *odo, MotorsController *mot ) 
  : angle_regu(false), dist_regu(true) {
    
  // Au depart on se place a (0,0)
  angle_consigne = 0;
  dist_consigne = 0;

  // On set l'odométrie et le controlle des moteurs
  odometrie = odo;
  motors = mot;

  // Les 2 regu sont actifs par defaut
  angle_regu_on = true;
  dist_regu_on = true;
    
}

// Destructeur
ConsignController::~ConsignController() {}

/*
 * Les consignes, que ce soit des add ou des set sont en UO !!!!
 */
void ConsignController::add_angle_consigne(int64_t deltaUO) {
    angle_consigne += deltaUO;
}

void ConsignController::add_dist_consigne(int64_t deltaUO) {
    dist_consigne += deltaUO;
}

void ConsignController::set_angle_consigne(int64_t consigneUO) {
    angle_consigne = consigneUO;
}

void ConsignController::set_dist_consigne(int64_t consigneUO) {
    dist_consigne = consigneUO;
}

// TODO ce truc n'a rien à foutre ici ... Il devrait être dans le CommandManager
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

// On recalcule la consigne à appliquer aux moteurs et on leur envois l'ordre
void ConsignController::perform() {
  
  int64_t dist_output = 0; // Calcul de la sortie moteur en distance
  int64_t angle_output = 0; // Calcul de la sortie moteur en angle
  
  // Si le régulateur d'angle est actif, il doit calculer la consigne angulaire en fonction de la différence des tics codeurs (variation d'angle en UO)
  if ( angle_regu_on ) {
    angle_output = angle_regu.manage(angle_consigne, odometrie->getDeltaThetaBrut() );
  }
  // Si le régu de distance est actif, il doit calculer la consigne de distance en fonction de la moyenne des tics codeurs (variation de distance en UO)
  if ( dist_regu_on ) {
    dist_output = dist_regu.manage(dist_consigne, odometrie->getDeltaDist() );
  }
  
  //Calcul des vitesses a appliquer en les bornant évidemment
  int64_t VmoteurD = Utils::constrain( dist_output + angle_output , V_MAX_NEG_MOTOR , V_MAX_POS_MOTOR);
  int64_t VmoteurG = Utils::constrain( dist_output - angle_output, V_MAX_NEG_MOTOR , V_MAX_POS_MOTOR);

  // On donne l'ordre aux moteurs et roulez jeunesse !!
  motors->vitesseG(VmoteurG);
  motors->vitesseD(VmoteurD);
/*
  pc.printf("VG=%d  ", VmoteurG);
  pc.printf("VD=%d\n", VmoteurD);
*/
}

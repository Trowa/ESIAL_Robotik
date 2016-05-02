#include "ConsignController.h"

//#define DEBUG
#ifdef DEBUG
#include "../debug/DebugUDP.h"
#endif

// Constructeur prenant deux objets initialisé avec l'asserv en paramètre
ConsignController::ConsignController(Odometrie *odo, MotorsController *mot)
    : angle_regu(false), dist_regu(true)
{

    // Au départ on se place à (0,0)
    angle_consigne = 0;
    dist_consigne = 0;

    // On set l'odométrie et le contrôle des moteurs
    odometrie = odo;
    motors = mot;

    // Les 2 regus sont actifs par défaut
    angle_regu_on = true;
    dist_regu_on = true;
}

// Destructeur
ConsignController::~ConsignController() {}

/*
 * Les consignes, que ce soit des add ou des set sont en UO !!!!
 */
void ConsignController::add_angle_consigne(int64_t deltaUO)
{
    angle_consigne += deltaUO;
}

void ConsignController::add_dist_consigne(int64_t deltaUO)
{
    dist_consigne += deltaUO;
}

void ConsignController::set_angle_consigne(int64_t consigneUO)
{
    angle_consigne = consigneUO;
}

void ConsignController::set_dist_consigne(int64_t consigneUO)
{
    dist_consigne = consigneUO;
}



// On recalcule la consigne à appliquer aux moteurs et on leur envoie l'ordre
void ConsignController::perform()
{

    int64_t dist_output = 0; // Calcul de la sortie moteur en distance
    int64_t angle_output = 0; // Calcul de la sortie moteur en angle

    // Si le régulateur d'angle est actif, il doit calculer la consigne angulaire en fonction de la différence des tics codeurs (variation d'angle en UO)
    if (angle_regu_on && !Config::disableAngleRegu) {
        angle_output = angle_regu.manage(angle_consigne, odometrie->getDeltaThetaBrut());
    }

    // Si le régu de distance est actif, il doit calculer la consigne de distance en fonction de la moyenne des tics codeurs (variation de distance en UO)
    if (dist_regu_on && !Config::disableDistanceRegu) {
        dist_output = dist_regu.manage(dist_consigne, odometrie->getDeltaDist());
    }

    //Calcul des vitesses à appliquer en les bornant évidemment
    int64_t VmoteurD = Utils::constrain(dist_output + angle_output , Config::V_MAX_NEG_MOTOR , Config::V_MAX_POS_MOTOR);
    int64_t VmoteurG = Utils::constrain(dist_output - angle_output, Config::V_MAX_NEG_MOTOR , Config::V_MAX_POS_MOTOR);

    // On donne l'ordre aux moteurs et roulez jeunesse !!
    motors->vitesseG(VmoteurG);
    motors->vitesseD(VmoteurD);

    //printf("VG=%d  ", VmoteurG);
    //printf("VD=%d\n", VmoteurD);
}


void ConsignController::setLowSpeed(bool b)
{
    if (b) {
        dist_regu.setVitesseMarcheArriere(Config::DIST_QUAD_1ST_NEG / 16);
        dist_regu.setVitesseMarcheAvant(Config::DIST_QUAD_1ST_POS / 6);
    } else {
        dist_regu.setVitesseMarcheArriere(Config::DIST_QUAD_1ST_NEG);
        dist_regu.setVitesseMarcheAvant(Config::DIST_QUAD_1ST_POS);
    }
}

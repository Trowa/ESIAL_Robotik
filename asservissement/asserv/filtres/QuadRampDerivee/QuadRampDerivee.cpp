#include "QuadRampDerivee.h"

#ifdef DEBUG
#include "../../debug/DebugUDP.h"
extern DebugUDP *debugUdp;
#endif

//--------------------------------------------------------
//                      QUADRAMPm
//-------------------------------------------------------
QuadRampDerivee::QuadRampDerivee(bool isDistance)
{

#ifdef DEBUG
    this->isDistance = isDistance;
#endif

    if (isDistance) {
        // Dérivées premières de la consigne ( dans notre cas, la vitesse )
        derivee_1ier_pos = Config::DIST_QUAD_1ST_POS;
        derivee_1ier_neg = Config::DIST_QUAD_1ST_NEG;

        // Dérivées secondes de la consigne ( dans notre cas, l'accélération )
        derivee_2nd_pos_av = Config::DIST_QUAD_AV_2ND_ACC;
        derivee_2nd_neg_av = Config::DIST_QUAD_AV_2ND_DEC;

        //la même chose pour le déplacement vers l'arrière
        derivee_2nd_pos_ar = Config::DIST_QUAD_AR_2ND_ACC;
        derivee_2nd_neg_ar = Config::DIST_QUAD_AR_2ND_DEC;

        //On ajoute de l' "anticipation " pour éviter que la vitesse ne chute brutalement à zéro quand le point d'arrivée arrive....( bien pourri ca dis donc -_-" )
        gainAnticipation_av = Config::DIST_QUAD_AV_ANTICIPATION_GAIN_COEF;
        gainAnticipation_ar = Config::DIST_QUAD_AR_ANTICIPATION_GAIN_COEF;

        //Pour éviter les vibrations à l'arrivée
        tailleFenetreArrivee = Config::DIST_TAILLE_FENETRE_ARRIVEE;
    } else {
        // Dérivées premières de la consigne ( dans notre cas, la vitesse ) et le sens n'a pas d'importance en rotation
        derivee_1ier_pos = Config::ANGLE_QUAD_1ST_POS;
        derivee_1ier_neg = Config::ANGLE_QUAD_1ST_POS; // pas de NEG pour l'angle

        // Dérivées secondes de la consigne ( dans notre cas, l'accélération )
        derivee_2nd_pos_av = Config::ANGLE_QUAD_2ND_ACC;
        derivee_2nd_neg_av = Config::ANGLE_QUAD_2ND_DEC;

        //la même chose pour le déplacement vers l'arrière et le sens n'a pas d'importance en rotation
        derivee_2nd_pos_ar = Config::ANGLE_QUAD_2ND_ACC;
        derivee_2nd_neg_ar = Config::ANGLE_QUAD_2ND_DEC;

        //On ajoute de l' "anticipation " pour éviter que la vitesse ne chute brutalement à zéro quand le point d'arrivée arrive....( bien pourri ca dis donc -_-" )
        gainAnticipation_av = Config::ANGLE_QUAD_ANTICIPATION_GAIN_COEF;
        gainAnticipation_ar = Config::ANGLE_QUAD_ANTICIPATION_GAIN_COEF;

        //Pour éviter les vibrations à l'arrivée
        tailleFenetreArrivee = Config::ANGLE_TAILLE_FENETRE_ARRIVEE;
    }

    // La vitesse initiale est nulle
    prevConsigneVitesse = 0;
    // Comme il n'y a pas encore de consigne, on est arrivé
    arrivee = true;
}

// Destructeur
QuadRampDerivee::~QuadRampDerivee() {};

// On filtre la consigne pour prendre en compte l'accélération et la décélération
int64_t QuadRampDerivee::filtre(int64_t consigne, int64_t position_actuelle , int64_t vitesse)
{

    // Reset du flag "arrivee" signalant que la consigne est atteinte
    arrivee = false;

    //Calcul de la position du pivot qui sert à déterminer si l'on doit commencer à décélérer ou non
    char sens = (consigne - position_actuelle >= 0) ? 1 : -1;
    int64_t position_pivot;

    if (sens == 1) {
        position_pivot = consigne + ((vitesse >= 0) ? -1 : 1) * (((vitesse * vitesse) / (2 * derivee_2nd_neg_av)) + llabs(vitesse) * gainAnticipation_av);
    } else {
        position_pivot = consigne + ((vitesse >= 0) ? -1 : 1) * (((vitesse * vitesse) / (2 * derivee_2nd_neg_ar)) + llabs(vitesse) * gainAnticipation_ar);
    }

    //Calcul de la consigne d'accélération qui dépend dans le sens dans lequel on roule et vient de config.h
    int64_t acceleration_consign;

    if (position_pivot >= position_actuelle) {
        acceleration_consign = (sens == 1) ? derivee_2nd_pos_av : derivee_2nd_neg_ar;
    } else {
        acceleration_consign = (sens == 1) ? -derivee_2nd_neg_av : -derivee_2nd_pos_ar;
    }

    // Calcul de la consigne de vitesse
    int64_t consigneVitesse = prevConsigneVitesse + acceleration_consign;
    // On borne la consigne, parce que faut éviter de trop déconner en atelier
    consigneVitesse = Utils::constrain(consigneVitesse, -derivee_1ier_neg, derivee_1ier_pos);
    // On stocke la nouvelle consigne pour l'itération suivante
    prevConsigneVitesse = consigneVitesse;

    //Du debug
#ifdef DEBUG

    char name[32];

    strcpy(name, "pivot");
    name[5] = (isDistance ? 'D' : 'A');
    name[6] = 0;
    debugUdp->addData(name, (double) position_pivot);

    strcpy(name, "consigneVitesse");
    name[15] = (isDistance ? 'D' : 'A');
    name[16] = 0;
    debugUdp->addData(name, (double) consigneVitesse);

#endif

    // On vérifie si on est dans la fenêtre d'arrivée et si oui, on est arrivé à la fin de la rampe
    if (llabs(consigne - position_actuelle) < tailleFenetreArrivee) {
        prevConsigneVitesse = 0; // On reset la consigne precedente
        arrivee = true;
        return consigne;
    }

    //On retourne la consigne de position
    return position_actuelle + consigneVitesse;
}

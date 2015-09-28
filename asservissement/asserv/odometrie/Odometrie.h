#ifndef ODOMETRIE
#define ODOMETRIE

#include "../config/config.h"
#include "../codeurs/CodeursInterface.h"

class Odometrie
{

public:
    // Constructeur
    /*
    * Lors de la création de l'objet, on calcul la distance entre les roues en UO et le nombre d'UO par front
    * Les infos nécessaires au calcul sont dans config.h
    */
    Odometrie();
    ~Odometrie();

    // Reset de la position du robot
    void resetX(float xval);
    void resetY(float yval);
    void resetTheta();

    // Mise à jour de la position du robot
    void refresh();

    //Position en unités arbritraires ( Unite Odometrie [UO] pour les distances, et rad pour l'angle)
    /*
    * La classe Utils contient les méthodes de conversions pour que les mesures soient humainement compréhensible
    */
    int64_t getX() {
        return x;   // Renvoie la position en X par rapport au point de départ
    }
    int64_t getY() {
        return y;   // Renvoie la position en Y par rapport au point de départ
    }
    double getTheta() {
        return theta;   // Renvoie l'angle par rapport au cap de départ
    }

    // Variation de la position depuis la derniàre mise à jour
    double getDeltaTheta() {
        return deltaTheta;    // Variation du cap du robot
    }
    int64_t getDeltaDist() {
        return deltaDist;    // Variation de la distance du robot
    }
    int64_t getDeltaThetaBrut() {
        return compteurD - compteurG;    // Variation d'angle en UO donnée par la différence de compte entre les codeurs
    }

    // Getteurs config
    double getFrontParMetre() {
        return frontParMetre;
    }
    double getDistanceRouesUO() {
        return distanceRouesUO;
    }

    //Inversion logicielle des codeurs, des fois qu'on ait fait de la merde dans le cablage ...
    void inverseCodeurDroit() {
        Config::inverseCodeurD = !Config::inverseCodeurD;    //sens de rotation inverse pour le codeur droit
    }
    void inverseCodeurGauche() {
        Config::inverseCodeurG = !Config::inverseCodeurG;    //idem, pour le codeur gauche
    }
    void echangeCodeurs() {
        Config::swapCodeurs = !Config::swapCodeurs;    // codeurGauche <-> codeurDroit
    }

private:

    // Données sur les codeurs
    double frontParMetre; // Nombre de tics codeurs pour un mètre parcouru
    int64_t distanceRouesUO; // Distance entre les roues en UO
    double ratioCodeurs; // Ratio frontParMetreCodeurMax / frontParMetreCodeurMin pour compenser la différence physique entre codeur
    bool applyRatioOnG; // Si vrai, le ratio s'applique au codeur Gauche, sinon le Droit

    // Position actuelle
    int64_t  x, y; //En UO
    double theta; //En radian

    // Variation pour la mise a jour en cours
    int64_t deltaDist; // En UO
    double deltaTheta; //En radian
    int64_t compteurG; // Nombre de tics codeur G depuis dernier refresh
    int64_t compteurD; // Nombre de tics codeur D depuis dernier refresh

    //Codeurs
    CodeursInterface* codeurs;

};

#endif

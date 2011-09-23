#ifndef ODOMETRIE
#define ODOMETRIE

#define PI 3.14159265358979

#include "../config.h"

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
    void resetX() { x = 0;}
    void resetY() { y = 0;}
    void resetTheta() { theta = 0;}
        
    // Mise a jour de la position du robot
    void refresh();
        
    //Position en unites arbritraires ( Unite Odometrie [UO] pour les distances, et rad pour l'angle)
    /*
    * Pour passer des UO au mm, la formule est : valeur_en_UO/(frontParMetre*uOParFront)*1000
    * Pour passer des radians au degrés : theta*180/PI
    */
    int64_t getX() {return x;}
    int64_t getY() {return y;}
    double getTheta() {return theta;}
        
    // Variation de la position depuis la derniere mise a jour
    double getDeltaTheta() { return deltaTheta; } // Variation du cap du robot 
    int64_t getDeltaDist() { return deltaDist; } // Variation de la distance du robot

    // Getteurs config
    double getFrontParMetre() { return frontParMetre; }
    double getDistanceRouesUO() { return distanceRouesUO; }
        
    //Inversion logicielle des codeurs
    void inverseCodeurDroit() {inverseCodeurD=!inverseCodeurD; } //sens de rotation inverse pour le codeur droit
    void inverseCodeurGauche() {inverseCodeurG=!inverseCodeurG; } //idem, pour le codeur gauche
    void echangeCodeurs() {swapCodeurs=!swapCodeurs; } // codeurGauche <-> codeurDroit
        
  private:
        
    // Donnees sur les codeurs
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
        
};

#endif

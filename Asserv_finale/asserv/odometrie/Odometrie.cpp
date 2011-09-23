#include "mbed.h"
#include "odometrie.h"

// Constructeur
/*
* Lors de la création de l'objet, on calcul la distance entre les roues en UO et le nombre d'UO par front
* Les infos nécessaires au calcul sont dans config.h
*/
// TODO Changera surement de forme avec la carte dédié aux codeurs et ajouter un destructeur
Odometrie::Odometrie() {

  // Initialisation des compteurs
  compteurG = 0;
  compteurD = 0;

  // Initialisation de la position
  x = 0;
  y = 0;
  theta = 0;

  // Calcul de frontParMetre et de ratioCodeur
  if(frontParMetreCodeurG != frontParMetreCodeurD) {
    double min,max;
    if(frontParMetreCodeurG>frontParMetreCodeurD) {
      min = frontParMetreCodeurD;
      max = frontParMetreCodeurG;
      applyRatioOnG = false;
    } else {
      min = frontParMetreCodeurG;
      max = frontParMetreCodeurD;
      applyRatioOnG = true;
    }
        
    ratioCodeurs = max/min;
    frontParMetre = max;
           
  } else {
    ratioCodeurs = 1;
    frontParMetre = frontParMetreCodeurD;
  }

  // Calcul de la distance entre les roues en UO
  distanceRouesUO = distRoues * frontParMetre * uOParFront / 1000;

}

// Destructeur
Odometrie::~Odometrie() {}
        
// Mise a jour de la position du robot
void Odometrie::refresh() {

  // TODO récupération des comptes des codeurs
  compteurG = 0;
  compteurD = 0;
  // Fin TODO

  // On ajuste le sens des codeurs en cas de problème de cablage
  if(swapCodeurs) { //inversion des deux codeurs
    int64_t temp = compteurG;
    compteurG = compteurD;
    compteurD = temp;
  }
  if(inverseCodeurD) { //Changement de sens de rotation du codeur droit
    compteurD = -compteurD;
  }
  if(inverseCodeurG) { //Changement de sens de rotation du codeur gauche
    compteurG = -compteurG;
  }

  //On transforme ces valeurs en Unites Odometrique
  compteurD = compteurD * uOParFront;
  compteurG = compteurG * uOParFront;

  // On applique le ratio pour prendre en compte la différence entre les codeurs
  if(applyRatioOnG) {
    compteurG = compteurG * ratioCodeurs;
  } else {
    compteurD = compteurD * ratioCodeurs;
  }

  /* 
  * deltaDist = la distance parcourue par le robot pendant l'iteration = moyenne des distances des codeurs
  * deltaTheta = la variation de l'angle pendant l'iteration = rapport de la différence des distances codeurs sur la 
  *               distance entre les roues
  */
  deltaDist = (compteurG + compteurD)/2; // En UO
  int64_t diffCount = compteurD - compteurG; // On conserve la difference entre les comptes en UO
  deltaTheta = diffCount / distanceRouesUO; // En radian

  if ( abs(diffCount) < 1 ) { // On considere le mouvement comme une ligne droite
    // Mise a jour de la position
    x += deltaDist * cos(theta);
    y += deltaDist * sin(theta);
  } else { //On approxime en considerant que le robot suit un arc de cercle
    // On calcule le rayon de courbure du cercle
    double R = deltaDist/deltaTheta;
    //Mise a jour de la position
    x += R * (-sin(theta) + sin(theta + deltaTheta) );
    y += R * (cos(theta) - cos(theta + deltaTheta) );
    // Mise a jour du cap
    theta += deltaTheta;

    // On limite le cap a +/- PI afin de ne pouvoir tourner dans les deux sens et pas dans un seul
    if ( theta > PI )
      theta -= 2 * PI ;
    else if ( theta <= -PI )
      theta += 2 * PI ;
    }

}

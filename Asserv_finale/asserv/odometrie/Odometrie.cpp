#include "mbed.h"
#include "Odometrie.h"
#include "../codeurs/CodeursDirects.h"
//#include "../codeurs/CodeursAVR.h"

#ifdef DEBUG
#include "../debug/DebugUDP.h"
    extern DebugUDP *debugUdp;
#endif

// Constructeur
/*
* Lors de la cr&#65533;ation de l'objet, on calcul la distance entre les roues en UO et le nombre d'UO par front
* Les infos n&#65533;cessaires au calcul sont dans config.h
*/
Odometrie::Odometrie() {

  //Instanciation des codeurs
  codeurs = new CodeursDirects(p25, p26, p15, p16); //Avec des codeurs branch&#65533;s directement sur la Mbed
  //codeurs = new CodeursAVR(p5, p6, p7, p8); //OU avec des codeurs branch&#65533;s sur un AVR avec lequel on communique en SPI

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
Odometrie::~Odometrie() {
    delete codeurs;
}

// Mise a jour de la position du robot
void Odometrie::refresh() {

  //ATTENTION : CODE A DECOMMENTER EN FONCTIONNEMENT NORMAL

  //R&#65533;cup&#65533;ration des comptes des codeurs
  codeurs->getCounts(&compteurG, &compteurD);

  //pc.printf("CG=%lld CD=%lld\n", compteurG, compteurD);
  //pc.printf(compteurD==0 ? "***\n" : ".\n");

  //On transforme ces valeurs en Unites Odometrique
  compteurD = compteurD * uOParFront;
  compteurG = compteurG * uOParFront;

  // On applique le ratio pour prendre en compte la diff&#65533;rence entre les codeurs
  if(applyRatioOnG) {
    compteurG = compteurG * ratioCodeurs;
  } else {
    compteurD = compteurD * ratioCodeurs;
  }

  //FIN ATTENTION

  //ATTENTION : CODE A COMMENTER EN FONCTIONNEMENT NORMAL
  /*
  // Bout de code permettant de mesurer le nombre de tics codeurs par m&#65533;tre.
  // Si ce code n'est PAS comment&#65533;, l'asservissement fera N'IMPORTE QUOI ! Vous &#65533;tes pr&#65533;venu !

  //D&#65533;claration temporaire pour les comptes des codeurs
  int64_t tempCompteG;
  int64_t tempCompteD;
  //R&#65533;cup&#65533;ration des comptes des codeurs
  codeurs->getCounts(&tempCompteG, &tempCompteD);

  //On rajoute les comptes r&#65533;cup&#65533;r&#65533;s aux totaux
  compteurD += tempCompteD;
  compteurG += tempCompteG;

  //renvoi des r&#65533;sultats sur la s&#65533;rie
  pc.printf("CG=%lld \tCD=%lld\n", compteurG, compteurD);
  */
  //FIN ATTENTION

  /*
  * deltaDist = la distance parcourue par le robot pendant l'iteration = moyenne des distances des codeurs
  * deltaTheta = la variation de l'angle pendant l'iteration = rapport de la diff&#65533;rence des distances codeurs sur la
  *               distance entre les roues
  */
  deltaDist = (compteurG + compteurD)/2; // En UO
  int64_t diffCount = compteurD - compteurG; // On conserve la difference entre les comptes en UO
  deltaTheta = (double)diffCount / (double)distanceRouesUO; // En radian

  if ( llabs(diffCount) < 1 ) { // On considere le mouvement comme une ligne droite
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
    if ( theta > PI ) {
      theta -= 2 * PI ;
    } else if ( theta <= -PI ) {
      theta += 2 * PI ;
    }
  }

    //Si le debug est en route
    #ifdef DEBUG
       if(debugUdp->getDebugSend()) {
   /* on ajoute les valeurs et on les envoies */
        //uint64_t XMM =  this->getXmm();
        //uint64_t YMM =  this->getYmm();
        debugUdp->addData("X", (double) x);
        debugUdp->addData("Y", (double) y);
        debugUdp->addData("Theta", (double) theta);
        debugUdp->addData("deltaDist", (double) deltaDist);
        debugUdp->addData("deltaTheta", (double) deltaTheta);
   /*    udp_data_add("X",XMM);
           udp_data_add("Y", YMM);
           udp_data_add("T", this->getThetaDeg() );
           udp_data_add("Vl", deltaDist/(frontParMetre*uOParFront)*1000 );
           udp_data_add("Va", deltaTheta*180/PI );
    */
    }
    #endif


    //pc.printf("x=%lld-y=%lld\n", x, y);
    /*
    pc.printf("y=%d  ",y);
    pc.printf("theta=%f\n",theta);
    */
}

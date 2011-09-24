#include "QuadRampDerivee.h"

//--------------------------------------------------------
//                      QUADRAMPm
//-------------------------------------------------------
QuadRampDerivee::QuadRampDerivee(bool isDistance) {
  
  if (isDistance) {
    // Derivees premieres de la consigne ( dans notre cas, la vitesse )
    derivee_1ier_pos = DIST_QUAD_1ST_POS;
    derivee_1ier_neg = DIST_QUAD_1ST_NEG;

    // Derivees secondes de la consigne ( dans notre cas, l'acceleration ) 
    derivee_2nd_pos_av = DIST_QUAD_AV_2ND_ACC;
    derivee_2nd_neg_av = DIST_QUAD_AV_2ND_DEC;
     
    //la meme chose pour le deplacement vers l'arriere
    derivee_2nd_pos_ar = DIST_QUAD_AR_2ND_ACC;
    derivee_2nd_neg_ar = DIST_QUAD_AR_2ND_DEC;
     
    //On ajoute de l' "anticipation " pour eviter que la vitesse ne chute brutalement a zero quand le point d'arrivee arrive....( bien pourri ca dis donc -_-" )
    gainAnticipation_av = DIST_QUAD_AV_ANTICIPATION_GAIN_COEF;
    gainAnticipation_ar = DIST_QUAD_AR_ANTICIPATION_GAIN_COEF;
     
    //Pour eviter les vibrations a l'arrivee
    tailleFenetreArrivee = DIST_TAILLE_FENETRE_ARRIVEE;
  } else {
    // Derivees premieres de la consigne ( dans notre cas, la vitesse )
    derivee_1ier_pos = ANGLE_QUAD_1ST_POS;
    derivee_1ier_neg = ANGLE_QUAD_1ST_POS;

    // Derivees secondes de la consigne ( dans notre cas, l'acceleration ) 
    derivee_2nd_pos_av = ANGLE_QUAD_2ND_ACC;
    derivee_2nd_neg_av = ANGLE_QUAD_2ND_DEC;
     
    //la meme chose pour le deplacement vers l'arriere
    derivee_2nd_pos_ar = ANGLE_QUAD_2ND_ACC;
    derivee_2nd_neg_ar = ANGLE_QUAD_2ND_DEC;
     
    //On ajoute de l' "anticipation " pour eviter que la vitesse ne chute brutalement a zero quand le point d'arrivee arrive....( bien pourri ca dis donc -_-" )
    gainAnticipation_av = ANGLE_QUAD_ANTICIPATION_GAIN_COEF;
    gainAnticipation_ar = ANGLE_QUAD_ANTICIPATION_GAIN_COEF;
     
    //Pour eviter les vibrations a l'arrivee
    tailleFenetreArrivee = ANGLE_TAILLE_FENETRE_ARRIVEE;
  }
  
  prevConsigneVitesse = 0;
  arrivee=true;
}

QuadRampDerivee::~QuadRampDerivee() {};

int64_t QuadRampDerivee::filtre(int64_t consigne, int64_t position_actuelle , int64_t vitesse) {
    
  // Reset du flag "arrivee" signalant que la consigne est atteinte
  arrivee = false;

  //Calcul de la position du pivot
  char sens = ( consigne - position_actuelle >= 0 ) ? 1 : -1;
  int64_t position_pivot;
  if(sens == 1) {
    position_pivot = consigne + ( ( vitesse >= 0 ) ? -1 : 1) * ( ((vitesse * vitesse )/(2*derivee_2nd_neg_av)) + abs(vitesse) * gainAnticipation_av );
  } else {
    position_pivot = consigne + ( ( vitesse >= 0 ) ? -1 : 1) * ( ((vitesse * vitesse )/(2*derivee_2nd_neg_ar)) + abs(vitesse) * gainAnticipation_ar );
  }
    
  //Calcul de la consigne d'acceleration  
  int64_t acceleration_consign;
   
  if( position_pivot >= position_actuelle )
    acceleration_consign = (sens == 1) ? derivee_2nd_pos_av : derivee_2nd_neg_ar;
  else
    acceleration_consign = (sens == 1) ? -derivee_2nd_neg_av : -derivee_2nd_pos_ar;

  //Calcul de la consigne de vitesse  
  int64_t consigneVitesse = prevConsigneVitesse + acceleration_consign;
  consigneVitesse = Utils::constrain(consigneVitesse,-derivee_1ier_neg, derivee_1ier_pos);
  prevConsigneVitesse = consigneVitesse;
      
  // On verifie si on est dans la fenetre d'arrivee
  if ( abs(consigne-position_actuelle) < tailleFenetreArrivee ) {
    prevConsigneVitesse = 0; // On reset la consigne precedente 
    arrivee = true;
    return consigne;
  }
   
  //On retourne la consigne de position
  return position_actuelle+consigneVitesse; 
}

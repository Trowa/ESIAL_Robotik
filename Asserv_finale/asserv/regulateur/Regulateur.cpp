#include "Regulateur.h"

// Constructeur
Regulateur::Regulateur(bool isDistance) : filtreQuadRampDerivee(isDistance), filtrePid(isDistance) {
  filtreQuadRampDeriveeON = true;
  accumulateur = 0;
}

// Destructeur
Regulateur::~Regulateur() {
}

// Calcul de l'erreur
int64_t Regulateur::manage(int64_t consigne, int64_t feedback_odometrie) {
 
  // On ajoute dans l'accumulateur la variation de déplacement mesuré par l'odométrie
  accumulateur += feedback_odometrie;
    
  // On filtre la consigne a l'aide de la QuadRampeDerivee pour savoir si l'on est en phase d'accélération, constante ou de décélération
  // Cette étape permet d'avoir un comportement linéaire au lieu de binaire (= soit à l'arrêt, soit à fond) qui provoque des secouses et peut 
  // renverser le robot. A la place, on accélère et déccélère tranquillement et ça, c'est beau :p
  int64_t consigneFiltree = filtreQuadRampDerivee.filtre(consigne, accumulateur, feedback_odometrie); 

  // On calcul l'erreur, c'est à dire la différence entre la consigne à suivre et la position réelle stockée dans l'accumulateur
  int64_t erreur;
  if ( filtreQuadRampDeriveeON ) {
    erreur = consigneFiltree - accumulateur;
  } else {
    erreur = consigne - accumulateur;  
  }
  
  // On envoit l'erreur au PID qui va la filtrer par rapport aux paramètres du robot et aux coefficients d'asservissement
  // On obtient alors la vitesse a envoyer aux moteurs pour corriger l'erreur courante
  int64_t erreurFiltree = filtrePid.filtre(erreur, feedback_odometrie);
  
  return erreurFiltree;
}

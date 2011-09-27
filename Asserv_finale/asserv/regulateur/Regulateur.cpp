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
 
  // On ajoute dans l'accumulateur la variation de d�placement mesur� par l'odom�trie
  accumulateur += feedback_odometrie;
    
  // On filtre la consigne a l'aide de la QuadRampeDerivee pour savoir si l'on est en phase d'acc�l�ration, constante ou de d�c�l�ration
  // Cette �tape permet d'avoir un comportement lin�aire au lieu de binaire (= soit � l'arr�t, soit � fond) qui provoque des secouses et peut 
  // renverser le robot. A la place, on acc�l�re et d�cc�l�re tranquillement et �a, c'est beau :p
  int64_t consigneFiltree = filtreQuadRampDerivee.filtre(consigne, accumulateur, feedback_odometrie); 

  // On calcul l'erreur, c'est � dire la diff�rence entre la consigne � suivre et la position r�elle stock�e dans l'accumulateur
  int64_t erreur;
  if ( filtreQuadRampDeriveeON ) {
    erreur = consigneFiltree - accumulateur;
  } else {
    erreur = consigne - accumulateur;  
  }
  
  // On envoit l'erreur au PID qui va la filtrer par rapport aux param�tres du robot et aux coefficients d'asservissement
  // On obtient alors la vitesse a envoyer aux moteurs pour corriger l'erreur courante
  int64_t erreurFiltree = filtrePid.filtre(erreur, feedback_odometrie);
  
  return erreurFiltree;
}

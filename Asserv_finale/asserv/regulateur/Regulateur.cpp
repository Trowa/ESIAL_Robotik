#include "Regulateur.h"

Regulateur::Regulateur(bool isDistance) : filtreQuadRampDerivee(isDistance), filtrePid(isDistance) {
  filtreQuadRampDeriveeON = true;
  accumulateur = 0;
}

Regulateur::~Regulateur() {
}

int64_t Regulateur::manage(int64_t consigne, int64_t feedback_odometrie) {
 
  accumulateur += feedback_odometrie;
    
  int64_t consigneFiltree = filtreQuadRampDerivee.filtre(consigne, accumulateur, feedback_odometrie); 
      
  int64_t erreur;
  if ( filtreQuadRampDeriveeON ) {
    erreur = consigneFiltree - accumulateur;
  } else {
    erreur = consigne - accumulateur;  
  }
  
  int64_t erreurFiltree = filtrePid.filtre(erreur, feedback_odometrie); //PID Tommy
  
  return erreurFiltree;
}

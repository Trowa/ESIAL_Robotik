#include "ConsigController.h"

//--------------------------------------------------------
//                       REGULATEUR
//--------------------------------------------------------

long Regulateur::manage(long consigne, long feedback)
{
     feedbackFiltree = filtreFeedback->filtre(feedback); 
     consigneFiltree = filtreConsigne->filtre(consigne, feedbackFiltree, feedback);
     erreur = consigneFiltree - feedbackFiltree;
     erreurFiltree = filtreErreur->filtre(erreur);
     
     return erreurFiltree;
}




//--------------------------------------------------------
//                       CONSIGN CONTROLLER
//--------------------------------------------------------

ConsignController::ConsignController(Odometrie *odo, Motors *mot )
{
  // Au départ on se place à (0,0)
  angle_consigne = 0;
  dist_consigne = 0;
  
  odometrie = odo;
  motors = mot;
  
  // Les 2 régu sont actifs par défaut
  angle_regu_on = false;
  dist_regu_on = true;
  
  
  /*
   * Régulateur d'angle
   */
  
    //Filtre erreur (PID)
    filtreErreur_angle.setCoeffs(ANGLE_KP, ANGLE_KI , ANGLE_KD);
    filtreErreur_angle.setRatioSortie(ANGLE_OUT_RATIO);
    filtreErreur_angle.setSortieMax(ANGLE_MAX_OUTPUT);
    filtreErreur_angle.setMaxIntegral(ANGLE_MAX_INTEGRAL);
    
    //Filtre consigne (quadRamp Derivée )
    filtreConsigne_angle.setDerivee1ier(ANGLE_QUAD_1ST_POS , ANGLE_QUAD_1ST_NEG );
    filtreConsigne_angle.setDerivee2nd(ANGLE_QUAD_2ND_ACC , ANGLE_QUAD_2ND_DEC );    
    filtreConsigne_angle.setCoeffGainAnticipation(ANGLE_QUAD_ANTICIPATION_GAIN_COEF);    
    filtreConsigne_angle.setTailleFenetreArrivee(ANGLE_TAILLE_FENETRE_ARRIVEE);    
    
  angle_regu.setFiltres( &filtreConsigne_angle , &filtreFeedback_angle , &filtreErreur_angle);



  /*
   * Régulateur de distance
   */
   
   //Filtre erreur (PID)
    filtreErreur_dist.setCoeffs(DIST_KP, DIST_KI , DIST_KD);
    filtreErreur_dist.setRatioSortie(DIST_OUT_RATIO);
    filtreErreur_dist.setSortieMax(DIST_MAX_OUTPUT);
    filtreErreur_dist.setMaxIntegral(DIST_MAX_INTEGRAL);
    
    //Filtre consigne (quadRamp Derivée )
    filtreConsigne_dist.setDerivee1ier(DIST_QUAD_1ST_POS , DIST_QUAD_1ST_NEG );
    filtreConsigne_dist.setDerivee2nd(DIST_QUAD_2ND_ACC , DIST_QUAD_2ND_DEC );  
    filtreConsigne_dist.setCoeffGainAnticipation(DIST_QUAD_ANTICIPATION_GAIN_COEF);
    filtreConsigne_dist.setTailleFenetreArrivee(DIST_TAILLE_FENETRE_ARRIVEE);
    
  dist_regu.setFiltres( &filtreConsigne_dist , &filtreFeedback_dist , &filtreErreur_dist);
  
  
}

void ConsignController::add_angle_consigne(long delta)
{ 
   angle_consigne += delta; 
}

void ConsignController::add_dist_consigne(long delta)
{ 
   dist_consigne += delta; 
}

void ConsignController::perform()
{
   long dist_output = 0; 
   long angle_output = 0;

    if ( angle_regu_on )
      angle_output = angle_regu.manage(angle_consigne, odometrie->getDeltaThetaBrut() );
    if ( dist_regu_on )
     dist_output = dist_regu.manage(dist_consigne, odometrie->getDeltaDist() );

    //Calcul des vitesses à appliquer
    long VmoteurD = constrain( dist_output + angle_output , V_MAX_NEG_MOTOR , V_MAX_POS_MOTOR);
    long VmoteurG = constrain( dist_output - angle_output, V_MAX_NEG_MOTOR , V_MAX_POS_MOTOR);
    

   motors->vitesseG(VmoteurG);
   motors->vitesseD(VmoteurD);

}

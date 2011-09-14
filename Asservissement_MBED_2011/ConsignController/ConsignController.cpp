
#include "ConsignController.h"
#include "../Filtres/Filtres_config.h"
#include "../Utils/Utils.h"

//#define DEBUG
#ifdef DEBUG
    #include "../UDPData.h"
#endif

ConsignController::ConsignController(Odometrie *odo, MotorsController *mot ) {
    // Au depart on se place a (0,0)
    angle_consigne = 0;
    dist_consigne = 0;

    odometrie = odo;
    motors = mot;

    // Les 2 regu sont actifs par defaut
    angle_regu_on = true;
    dist_regu_on = true;


    /*
     * Regulateur d'angle
     */

    //Filtre erreur (PID)
    angle_regu.setKP(ANGLE_KP);
    //angle_regu.setKI(ANGLE_KI);
    angle_regu.setKD(ANGLE_KD);
    angle_regu.setRatioSortie(ANGLE_OUT_RATIO);
    angle_regu.setSortieMax(ANGLE_MAX_OUTPUT);
    angle_regu.setKRepartition(ANGLE_COEFF_REPARTITION);
//    angle_regu.setMaxIntegral(ANGLE_MAX_INTEGRAL);


    //Filtre sortie (Rampe avec anticipation de deceleration)
    angle_regu.setMaxStepForward(ANGLE_MAX_STEP_FORWARD);
    angle_regu.setMaxStepBackward(ANGLE_MAX_STEP_BACKWARD);
    angle_regu.setDecelerationCoeffForward(ANGLE_DECELERATION_COEFF_FORWARD);
    angle_regu.setDecelerationCoeffBackward(ANGLE_DECELERATION_COEFF_BACKWARD);

    angle_regu.setMinDecelerationDistForward(ANGLE_MIN_DIST_DECELERATION_FORWARD);
    angle_regu.setMaxDecelerationDistForward(ANGLE_MAX_DIST_DECELERATION_FORWARD);
    angle_regu.setMinDecelerationDistBackward(ANGLE_MIN_DIST_DECELERATION_BACKWARD);
    angle_regu.setMaxDecelerationDistBackward(ANGLE_MAX_DIST_DECELERATION_BACKWARD);

    angle_regu.setMinDecelerationSpeedForward(ANGLE_MIN_DECELERATION_SPEED_FORWARD);
    angle_regu.setMinDecelerationSpeedBackward(ANGLE_MIN_DECELERATION_SPEED_BACKWARD);

    //Filtre consigne (quadRamp Derivee )
    angle_regu.setDerivee1ier(ANGLE_QUAD_1ST_POS , ANGLE_QUAD_1ST_NEG );
    angle_regu.setDerivee2ndAv(ANGLE_QUAD_2ND_ACC , ANGLE_QUAD_2ND_DEC );
    angle_regu.setCoeffGainAnticipationAv(ANGLE_QUAD_ANTICIPATION_GAIN_COEF);
    angle_regu.setDerivee2ndAr(ANGLE_QUAD_2ND_ACC , ANGLE_QUAD_2ND_DEC );
    angle_regu.setCoeffGainAnticipationAr(ANGLE_QUAD_ANTICIPATION_GAIN_COEF);
    angle_regu.setTailleFenetreArrivee(ANGLE_TAILLE_FENETRE_ARRIVEE);

    // angle_regu.setFiltres( &filtreConsigne_angle , &filtreFeedback_angle , &filtreErreur_angle);



    /*
     * Regulateur de distance
     */

    //Filtre erreur (PID)
    dist_regu.setKP(DIST_KP);
    //dist_regu.setKI(DIST_KI);
    dist_regu.setKD(DIST_KD);
    dist_regu.setRatioSortie(DIST_OUT_RATIO);
    dist_regu.setSortieMax(DIST_MAX_OUTPUT);
//    dist_regu.setMaxIntegral(DIST_MAX_INTEGRAL);
    dist_regu.setKRepartition(DIST_COEFF_REPARTITION);


    //Filtre sortie (Rampe avec anticipation de deceleration)
    dist_regu.setMaxStepForward(DIST_MAX_STEP_FORWARD);
    dist_regu.setMaxStepBackward(DIST_MAX_STEP_BACKWARD);
    dist_regu.setDecelerationCoeffForward(DIST_DECELERATION_COEFF_FORWARD);
    dist_regu.setDecelerationCoeffBackward(DIST_DECELERATION_COEFF_BACKWARD);

    dist_regu.setMinDecelerationDistForward(DIST_MIN_DIST_DECELERATION_FORWARD);
    dist_regu.setMaxDecelerationDistForward(DIST_MAX_DIST_DECELERATION_FORWARD);
    dist_regu.setMinDecelerationDistBackward(DIST_MIN_DIST_DECELERATION_BACKWARD);
    dist_regu.setMaxDecelerationDistBackward(DIST_MAX_DIST_DECELERATION_BACKWARD);

    dist_regu.setMinDecelerationSpeedForward(DIST_MIN_DECELERATION_SPEED_FORWARD);
    dist_regu.setMinDecelerationSpeedBackward(DIST_MIN_DECELERATION_SPEED_BACKWARD);


    //Filtre consigne (quadRamp Derivee )
    dist_regu.setDerivee1ier(DIST_QUAD_1ST_POS , DIST_QUAD_1ST_NEG );
    dist_regu.setDerivee2ndAv(DIST_QUAD_AV_2ND_ACC , DIST_QUAD_AV_2ND_DEC );
    dist_regu.setCoeffGainAnticipationAv(DIST_QUAD_AV_ANTICIPATION_GAIN_COEF);
    dist_regu.setDerivee2ndAr(DIST_QUAD_AR_2ND_ACC , DIST_QUAD_AR_2ND_DEC );
    dist_regu.setCoeffGainAnticipationAr(DIST_QUAD_AR_ANTICIPATION_GAIN_COEF);
    dist_regu.setTailleFenetreArrivee(DIST_TAILLE_FENETRE_ARRIVEE);

    // dist_regu.setFiltres( &filtreConsigne_dist , &filtreFeedback_dist , &filtreErreur_dist);

    angle_regu.setConsigneOnOff(false);
    
}


void ConsignController::add_angle_consigne(int64_t delta) {
    angle_consigne += delta;
}

void ConsignController::add_dist_consigne(int64_t delta) {
    dist_consigne += delta;
}

void ConsignController::reset_consignes() {
    angle_consigne = 0;
    dist_consigne = 0;
}

void ConsignController::set_angle_consigne(int64_t consigne) {
    angle_consigne = consigne;
}

void ConsignController::set_dist_consigne(int64_t consigne) {
    dist_consigne = consigne;
}



void ConsignController::calage_bordure(int sens)
{
/*
    dist_regu.setDerivee1ier(DIST_QUAD_1ST_POS , DIST_QUAD_1ST_NEG/2 );

    // On recule de 20cm...
    angle_Regu_Off();
    add_dist_consigne(odometrie->mmToUO(-200));
    wait(5);
    
    
    
    
    // On considere qu'on est arrive on reset le Y
    odometrie->resetTheta();
    reset_regu_angle();
    //odometrie->resetX();
    //reset_regu_dist();
    
    
    
    // On remet le regulateur, on avance pour pouvoir tourner et on tourne
    angle_Regu_On();
    add_dist_consigne(odometrie->mmToUO(140));
    wait(3);
    
    int mult = sens==0 ? -1 : 1;
    
    add_angle_consigne(mult * odometrie->degToUO(90));
    wait(3);
    
    
    
    add_dist_consigne(odometrie->mmToUO(-800));
    wait(10);
    reset_regu_dist();
    
    angle_Regu_Off();
    add_dist_consigne(odometrie->mmToUO(30));
    wait(1);
    add_dist_consigne(odometrie->mmToUO(-80));
    wait(3);
    
    odometrie->resetY();
    reset_regu_dist();
    odometrie->resetTheta();
    reset_regu_angle();
    odometrie->resetX();
    
    angle_Regu_On();
    
    add_dist_consigne(odometrie->mmToUO(50));
    
    dist_regu.setDerivee1ier(DIST_QUAD_1ST_POS , DIST_QUAD_1ST_NEG );
*/

    dist_regu.setDerivee1ier(DIST_QUAD_1ST_POS , DIST_QUAD_1ST_NEG/2 );
    
    // On recule de 20cm...
    angle_Regu_Off();
    add_dist_consigne(odometrie->mmToUO(-100));
    wait(5);
    
    odometrie->resetTheta();
    reset_regu_angle();
    odometrie->resetX();
    reset_regu_dist();
    
    angle_Regu_On();
    
    add_dist_consigne(odometrie->mmToUO(550));
    wait(5);
    
    
    int mult = sens==0 ? 1 : -1;
    add_angle_consigne(mult * odometrie->degToUO(90));
    wait(3);
    
    
    
    angle_Regu_Off();
    add_dist_consigne(-odometrie->mmToUO(300));
    wait(5);
    
    odometrie->resetY();
    reset_regu_dist();
    
    
    angle_Regu_On();
    add_dist_consigne(odometrie->mmToUO(140));
    wait(3);
    
    
    
    mult = sens==0 ? -1 : 1;
    add_angle_consigne(mult * odometrie->degToUO(90));
    wait(3);
    
    
    
    add_dist_consigne( -odometrie->mmToUO(500));
    wait(5);
    
    
    
    
    
    
    
    
    dist_regu.setDerivee1ier(DIST_QUAD_1ST_POS , DIST_QUAD_1ST_NEG );

}




extern Serial *pc;

void ConsignController::perform() {
    int64_t dist_output = 0;
    int64_t angle_output = 0;
    
    
    

    if ( angle_regu_on ) {
        angle_output = angle_regu.manage(angle_consigne, odometrie->getDeltaThetaBrut() );
        // printf("angle_cons=%d", angle_output);
    }
    if ( dist_regu_on ) {
        dist_output = dist_regu.manage(dist_consigne, odometrie->getDeltaDist() );
        //printf(" - dist_cons=%d", dist_output);
    }




    //Calcul des vitesses a appliquer
    int64_t VmoteurD = Utils::constrain( dist_output + angle_output , V_MAX_NEG_MOTOR , V_MAX_POS_MOTOR);
    int64_t VmoteurG = Utils::constrain( dist_output - angle_output, V_MAX_NEG_MOTOR , V_MAX_POS_MOTOR);

    motors->vitesseG(VmoteurG);
    motors->vitesseD(VmoteurD);
    
   //printf("%d - %d\n" , angle_output , angle_regu.getErreur()  );

    #ifdef DEBUG
        /* on ajoute les valeurs et on les envoies */
        udp_data_add("v",VmoteurG);
        udp_data_add("V", VmoteurD);
        udp_data_add("Pa", odometrie->UOtoDeg( angle_regu.getPositionPivot() ));
        udp_data_add("Pd", odometrie->UOtoMM(  dist_regu.getPositionPivot()));
        udp_data_add("Ea", angle_regu.getErreur());
        udp_data_add("Ed", dist_regu.getErreur());
    #endif
}


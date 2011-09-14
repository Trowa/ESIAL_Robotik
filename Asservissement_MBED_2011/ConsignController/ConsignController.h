#ifndef CONSIGN_CONTROLLER
#define CONSIGN_CONTROLLER

#include "../Odometrie/Odometrie.h"
#include "../MotorsController/MotorsController.h"
#include "../Regulateur/Regulateur.h"


class ConsignController {
public:
    ConsignController(Odometrie *odo, MotorsController *mot );

    void add_angle_consigne(int64_t delta);
    void add_dist_consigne(int64_t delta);
    
    void reset_consignes();
    
    void set_angle_consigne(int64_t consigne);
    void set_dist_consigne(int64_t consigne);
    
    void perform();

    Odometrie* getOdometrie() {
        return odometrie;
    }
    
    MotorsController* getMotorsController() {
        return motors;
    }

    /*
     * Methodes pour arreter les regulateurs independamment ( par exemple, pour faire un calage bordure .. )
     */
    void angle_Regu_On() { angle_regu_on = true; }
    void dist_Regu_On() { dist_regu_on = true; }
    
    void angle_Regu_Off() { angle_regu_on = false; }
    void dist_Regu_Off() { dist_regu_on = false; }
    
    
    void calage_bordure(int sens);
    
    void reset_regu_dist() 
    { 
        dist_regu.reset_accumulator(); 
        dist_consigne = 0; // TODO C'est moche bodel, dist_regu est un fucking objet, dist_consigne sert carrement a rien !!
    }
    
    void reset_regu_angle() 
    { 
        angle_regu.reset_accumulator(); 
        angle_consigne = 0; // TODO C'est moche bodel, dist_regu est un fucking objet, dist_consigne sert carrement a rien !!
    }

    // Methode permettant au CommandManager de savoir si la derniere consigne est finie ou non.
    bool  areRampsFinished() {
        return angle_regu.isRampFinished() && dist_regu.isRampFinished();
    }
    
    // Methode permettant de connaitre les valeurs accumulees sur les regulateurs, pour que le CommandManager
    // puisse definir des consignes de distance et d'angle
    int64_t getAccuAngle() { return angle_regu.getCurrentAccu(); }
    int64_t getAccuDist() { return dist_regu.getCurrentAccu(); }

//private:
    //Moteurs
    MotorsController *motors;

    //Regulateurs d'angle et de distance
    Regulateur angle_regu;
    bool angle_regu_on;
    
    Regulateur dist_regu;
    bool dist_regu_on;

    //Filtres associees
    /* inutile ?
    QuadRampDerivee filtreConsigne_dist;
    Accumulateur filtreFeedback_dist;
    Pid filtreErreur_dist;

    QuadRampDerivee filtreConsigne_angle;
    Accumulateur filtreFeedback_angle;
    Pid filtreErreur_angle;
    */
    
    // Odometrie
    Odometrie *odometrie;

    //Consignes courantes
    int64_t angle_consigne;
    int64_t dist_consigne;
    
    


};

#endif
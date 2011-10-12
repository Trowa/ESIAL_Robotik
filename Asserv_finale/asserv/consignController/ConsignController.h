#ifndef CONSIGN_CONTROLLER
#define CONSIGN_CONTROLLER

#include "mbed.h"
#include "../odometrie/Odometrie.h"
#include "../motorsController/MotorsController.h"
#include "../regulateur/Regulateur.h"
#include "../Utils/Utils.h"
#include "../config.h"


class ConsignController {
public:
    // Constructeur prenant en param�tre un objet Odometrie et un objet MotorsController cr�er � l'initialisation de l'asserv
    ConsignController(Odometrie *odo, MotorsController *mot );
    
    // Desctructeur
    ~ConsignController();
    
    // On ajoute une valeur � la consigne courante
    // Utile pour une marcher arri�re de quelques cm par exemple, ou pour tourner sur place
    void add_angle_consigne(int64_t delta);
    void add_dist_consigne(int64_t delta);
    
    // On fixe la consigne � atteindre
    void set_angle_consigne(int64_t consigne);
    void set_dist_consigne(int64_t consigne);
    
    // Rafraichie la consigne a realiser et on donne les ordres aux moteurs en cons�quence
    void perform();

    /*
     * Methodes pour arreter les regulateurs independamment ( par exemple, pour faire un calage bordure .. )
     * Nan mais l� c'est tellement gros de pas utiliser un setter que je laisse cette connerie
     */
    void angle_Regu_On() { angle_regu_on = true; }
    void dist_Regu_On() { dist_regu_on = true; }
    
    void angle_Regu_Off() { angle_regu_on = false; }
    void dist_Regu_Off() { dist_regu_on = false; }
    
    // TODO Rien � foutre ici �a !!!!
    void calage_bordure(int sens);
    
    /*
     * Permet d'activer ou de d�sactiver les QuadRampDerivee
     */
     void setQuadRamp_Angle(bool on) {
       angle_regu.setfiltreQuadRampDeriveeON(false);
     }

     void setQuadRamp_Dist(bool on) {
       dist_regu.setfiltreQuadRampDeriveeON(false);
     }

    /*
    * M�thodes permettant de r�initialiser les r�gulateurs. On met donc l'accumulateur
    * � 0 pour r�initialiser la position courante du point de vue du filtre. On met
    * aussi la consigne � atteindre � 0 sinon l'asserv va vouloir atteindre la position
    */
    void reset_regu_dist() { 
      dist_regu.reset_accumulator(); 
      dist_consigne = 0;
    }
    
    void reset_regu_angle() 
    { 
        angle_regu.reset_accumulator(); 
        angle_consigne = 0;
    }

    // Methode permettant au CommandManager de savoir si la QuadRamp est finie ou non.
    bool  areRampsFinished() {
        return angle_regu.isRampFinished() && dist_regu.isRampFinished();
    }
    
    // Methode permettant de connaitre les valeurs accumulees sur les regulateurs, pour que le CommandManager
    // puisse definir des consignes de distance et d'angle
    int64_t getAccuAngle() { return angle_regu.getAccumulateur(); }
    int64_t getAccuDist() { return dist_regu.getAccumulateur(); }

  private:
    
    //Moteurs
    MotorsController *motors;

    //Regulateurs d'angle
    Regulateur angle_regu;
    bool angle_regu_on; // Pour d�sactiver le r�gu
    //Regulateurs de distance
    Regulateur dist_regu;
    bool dist_regu_on; // Pour d�sactiver le r�gu

    // Odometrie
    Odometrie *odometrie;

    // Consignes courantes
    /*
    * xxx_consigne est la consigne en UO que l'on doit avoir atteint pour consid�rer
    * que la position vis�e est atteinte
    */
    int64_t angle_consigne;
    int64_t dist_consigne;
    
};

#endif

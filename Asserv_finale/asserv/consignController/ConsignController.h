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
    // Constructeur prenant en paramètre un objet Odometrie et un objet MotorsController créer à l'initialisation de l'asserv
    ConsignController(Odometrie *odo, MotorsController *mot );
    
    // Desctructeur
    ~ConsignController();
    
    // On ajoute une valeur à la consigne courante
    // Utile pour une marcher arrière de quelques cm par exemple, ou pour tourner sur place
    void add_angle_consigne(int64_t delta);
    void add_dist_consigne(int64_t delta);
    
    // On fixe la consigne à atteindre
    void set_angle_consigne(int64_t consigne);
    void set_dist_consigne(int64_t consigne);
    
    // Rafraichie la consigne a realiser et on donne les ordres aux moteurs en conséquence
    void perform();

    /*
     * Methodes pour arreter les regulateurs independamment ( par exemple, pour faire un calage bordure .. )
     * Nan mais là c'est tellement gros de pas utiliser un setter que je laisse cette connerie
     */
    void angle_Regu_On() { angle_regu_on = true; }
    void dist_Regu_On() { dist_regu_on = true; }
    
    void angle_Regu_Off() { angle_regu_on = false; }
    void dist_Regu_Off() { dist_regu_on = false; }
    
    // TODO Rien à foutre ici ça !!!!
    void calage_bordure(int sens);
    
    /*
     * Permet d'activer ou de désactiver les QuadRampDerivee
     */
     void setQuadRamp_Angle(bool on) {
       angle_regu.setfiltreQuadRampDeriveeON(false);
     }

     void setQuadRamp_Dist(bool on) {
       dist_regu.setfiltreQuadRampDeriveeON(false);
     }

    /*
    * Méthodes permettant de réinitialiser les régulateurs. On met donc l'accumulateur
    * à 0 pour réinitialiser la position courante du point de vue du filtre. On met
    * aussi la consigne à atteindre à 0 sinon l'asserv va vouloir atteindre la position
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
    bool angle_regu_on; // Pour désactiver le régu
    //Regulateurs de distance
    Regulateur dist_regu;
    bool dist_regu_on; // Pour désactiver le régu

    // Odometrie
    Odometrie *odometrie;

    // Consignes courantes
    /*
    * xxx_consigne est la consigne en UO que l'on doit avoir atteint pour considérer
    * que la position visée est atteinte
    */
    int64_t angle_consigne;
    int64_t dist_consigne;
    
};

#endif

#ifndef CONSIGN_CONTROLLER
#define CONSIGN_CONTROLLER

#include "mbed.h"
#include "../odometrie/Odometrie.h"
#include "../motorsController/MotorsController.h"
#include "../regulateur/Regulateur.h"
#include "../Utils/Utils.h"
#include "../config/config.h"


class ConsignController
{
public:
    // Constructeur prenant en paramètre un objet Odometrie et un objet MotorsController créés à l'initialisation de l'asserv
    ConsignController(Odometrie *odo, MotorsController *mot);

    // Desctructeur
    ~ConsignController();

    // On ajoute une valeur à la consigne courante
    // Utile pour une marcher arrière de quelques cm par exemple, ou pour tourner sur place
    void add_angle_consigne(int64_t deltaUO);
    void add_dist_consigne(int64_t deltaUO);

    // On fixe la consigne à atteindre
    void set_angle_consigne(int64_t consigneUO);
    void set_dist_consigne(int64_t consigneUO);

    // Rafraichie la consigne à realiser et on donne les ordres aux moteurs en conséquence
    void perform();

    /*
     * Methodes pour arrêter les régulateurs indépendamment ( par exemple, pour faire un calage bordure .. )
     * Nan mais là c'est tellement gros de pas utiliser un setter que je laisse cette connerie
     */
    void angle_Regu_On(bool enable) {
        angle_regu_on = enable;
    }
    void dist_Regu_On(bool enable) {
        dist_regu_on = enable;
    }

    /*
     * Permet d'activer ou de désactiver les QuadRampDerivee
     */
    void setQuadRamp_Angle(bool enable) {
        angle_regu.setfiltreQuadRampDeriveeON(enable);
    }

    void setQuadRamp_Dist(bool enable) {
        dist_regu.setfiltreQuadRampDeriveeON(enable);
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

    void reset_regu_angle() {
        angle_regu.reset_accumulator();
        angle_consigne = 0;
    }

    // Méthode permettant au CommandManager de savoir si la QuadRamp est finie ou non.
    bool  areRampsFinished() {
        return angle_regu.isRampFinished() && dist_regu.isRampFinished();
    }

    // Méthode permettant de connaître les valeurs accumulées sur les régulateurs, pour que le CommandManager
    // puisse définir des consignes de distance et d'angle
    int64_t getAccuAngle() {
        return angle_regu.getAccumulateur();
    }
    int64_t getAccuDist() {
        return dist_regu.getAccumulateur();
    }

    int64_t getDistConsigne() {
        return dist_consigne;
    }
    int64_t getAngleConsigne() {
        return angle_consigne;
    }


    //Méthode permettant de réduire la vitesse du robot, en cas de calage bordure par exemple
    void setLowSpeed(bool b);

private:

    //Moteurs
    MotorsController *motors;

    //Régulateurs d'angle
    Regulateur angle_regu;
    bool angle_regu_on; // Pour d&#65533;sactiver le r&#65533;gu
    //Régulateurs de distance
    Regulateur dist_regu;
    bool dist_regu_on; // Pour d&#65533;sactiver le r&#65533;gu

    // Odométrie
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

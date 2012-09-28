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
    // Constructeur prenant en param&#65533;tre un objet Odometrie et un objet MotorsController cr&#65533;er &#65533; l'initialisation de l'asserv
    ConsignController(Odometrie *odo, MotorsController *mot );

    // Desctructeur
    ~ConsignController();

    // On ajoute une valeur &#65533; la consigne courante
    // Utile pour une marcher arri&#65533;re de quelques cm par exemple, ou pour tourner sur place
    void add_angle_consigne(int64_t deltaUO);
    void add_dist_consigne(int64_t deltaUO);

    // On fixe la consigne &#65533; atteindre
    void set_angle_consigne(int64_t consigneUO);
    void set_dist_consigne(int64_t consigneUO);

    // Rafraichie la consigne a realiser et on donne les ordres aux moteurs en cons&#65533;quence
    void perform();

    /*
     * Methodes pour arreter les regulateurs independamment ( par exemple, pour faire un calage bordure .. )
     * Nan mais l&#65533; c'est tellement gros de pas utiliser un setter que je laisse cette connerie
     */
    void angle_Regu_On() { angle_regu_on = true; }
    void dist_Regu_On() { dist_regu_on = true; }

    void angle_Regu_Off() { angle_regu_on = false; }
    void dist_Regu_Off() { dist_regu_on = false; }

    // TODO Rien &#65533; foutre ici &#65533;a !!!!
    void calage_bordure(int sens);

    /*
     * Permet d'activer ou de d&#65533;sactiver les QuadRampDerivee
     */
     void setQuadRamp_Angle(bool on) {
       angle_regu.setfiltreQuadRampDeriveeON(false);
     }

     void setQuadRamp_Dist(bool on) {
       dist_regu.setfiltreQuadRampDeriveeON(false);
     }

    /*
    * M&#65533;thodes permettant de r&#65533;initialiser les r&#65533;gulateurs. On met donc l'accumulateur
    * &#65533; 0 pour r&#65533;initialiser la position courante du point de vue du filtre. On met
    * aussi la consigne &#65533; atteindre &#65533; 0 sinon l'asserv va vouloir atteindre la position
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

    int64_t getDistConsigne() { return dist_consigne; }
    int64_t getAngleConsigne() { return angle_consigne; }


    //Methode permettant de reduire la vitesse du robot, en cas de calage bordure par exemple
    void setLowSpeed(bool b);

  private:

    //Moteurs
    MotorsController *motors;

    //Regulateurs d'angle
    Regulateur angle_regu;
    bool angle_regu_on; // Pour d&#65533;sactiver le r&#65533;gu
    //Regulateurs de distance
    Regulateur dist_regu;
    bool dist_regu_on; // Pour d&#65533;sactiver le r&#65533;gu

    // Odometrie
    Odometrie *odometrie;

    // Consignes courantes
    /*
    * xxx_consigne est la consigne en UO que l'on doit avoir atteint pour consid&#65533;rer
    * que la position vis&#65533;e est atteinte
    */
    int64_t angle_consigne;
    int64_t dist_consigne;

};

#endif

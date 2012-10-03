#ifndef CONFIG
#define CONFIG

#include "mbed.h"

  // Retour PC
  extern Serial pc;


  //Débug ou pas ?
//#define DEBUG

/***************************
*       CommandManager     *
****************************/
  extern double angleThreshold; // Fenêtre de l'angle dans lequel on considère que le GoTo peut commencer à avancer
  extern int64_t returnThreshold; // Distance en dessous de laquelle on revient en marche arriere si on dépasse, sinon demi-tour
  extern int64_t enchainThreshold; // Distance au point de consigne à partir de laquelle on va essayer d'enchainer la consigne suivante

/***************************
*       ODOMETRIE          *
****************************/

  #define PI 3.14159265358979

  // Définition des variables pour l'odométrie
  extern double frontParMetreCodeurG; // Nombre de tics codeurs en 1m pour codeur gauche
  extern double frontParMetreCodeurD; // Nombre de tics codeurs en 1m pour codeur droite
  extern int64_t distRoues; // Distance entre les roues codeuses en mm
  extern int64_t uOParFront; // Nombre d'UO pour un tic de codeur

  //Booléens gérant une eventuelle inversion des codeurs
  extern bool inverseCodeurG;
  extern bool inverseCodeurD;
  extern bool swapCodeurs;

/***************************
*         Moteurs          *
****************************/
  extern int64_t V_MAX_POS_MOTOR;   // MD22 : 1 a 127, vitesse maximum positive
  extern int64_t V_MAX_NEG_MOTOR;  // MD22 : -1 a -128, vitesse maximum negative

  // Vitesses à partir desquelles le robot commence à bouger, pour ne pas
  // envoyer de jus dans des moteurs qui tournent pas
  extern int64_t V_MIN_POS_MOTOR;   // MD22 : 1 a 127
  extern int64_t V_MIN_NEG_MOTOR;  // MD22 : -1 a -128

  //Booléens gerant une éventuelle inversion des moteurs
  extern bool inverseMoteurG;
  extern bool inverseMoteurD;
  extern bool swapMoteurs;


/***************************
*           PID            *
****************************/
  // PID en distance
  extern int64_t DIST_KP; // Coeff proportionelle
  extern int64_t DIST_KI; // Coeff intégrale
  extern int64_t DIST_KD; // Coeff dérivée
  extern double DIST_OUT_RATIO; // Coeff permettant de diminuer les valeurs du PID
  extern int64_t DIST_MAX_OUTPUT; // Valeur de sortie maximum pour le moteur
  extern int64_t DIST_MAX_INTEGRAL; // Valeur maximum de l'intégrale (0 = filtre PD)


  extern int64_t ANGLE_KP; // Coeff proportionelle
  extern int64_t ANGLE_KI; // Coeff intégrale
  extern int64_t ANGLE_KD; // Coeff dérivée
  extern double ANGLE_OUT_RATIO; // Coeff permettant de diminuer les valeurs du PID
  extern int64_t ANGLE_MAX_OUTPUT; // Valeur de sortie maximum pour le moteur
  extern int64_t ANGLE_MAX_INTEGRAL; // Valeur maximum de l'intégrale (0 = filtre PD)

  // QUADRAMPDerivee
  extern int64_t DIST_QUAD_1ST_POS; // Vitesse max en marche avant
  extern int64_t DIST_QUAD_1ST_NEG; // Vitesse max en marche arrière
  extern int64_t DIST_QUAD_AV_2ND_ACC; // Accélération max en marche avant
  extern int64_t DIST_QUAD_AV_2ND_DEC; // Décélération max en marche avant
  extern int64_t DIST_QUAD_AV_ANTICIPATION_GAIN_COEF; // Coeff déterminant le début de la rampe de décélération en marche avant
  extern int64_t DIST_QUAD_AR_2ND_ACC; // Accélération max en marche arrière
  extern int64_t DIST_QUAD_AR_2ND_DEC; // Décélération max en marche arrière
  extern int64_t DIST_QUAD_AR_ANTICIPATION_GAIN_COEF; // Coeff déterminant le début de la rampe de décélération en marche arrière
  extern int64_t DIST_TAILLE_FENETRE_ARRIVEE; // Largeur de la zone où l'on considère être arrivé (UO)

  extern int64_t ANGLE_QUAD_1ST_POS; // Vitesse max en rotation
  extern int64_t ANGLE_QUAD_2ND_ACC; // Accélération max en rotation
  extern int64_t ANGLE_QUAD_2ND_DEC; // Décélération max en rotation
  extern int64_t ANGLE_QUAD_ANTICIPATION_GAIN_COEF; // Coeff déterminant le début de la rampe de décélération en rotation
  extern int64_t ANGLE_TAILLE_FENETRE_ARRIVEE; // Largeur de la zone où l'on considère être arrivé (UO)

#endif

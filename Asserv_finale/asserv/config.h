#ifndef CONFIG
#define CONFIG

#include "mbed.h"

  // Retour PC
  static Serial pc(USBTX, USBRX);


  //Débug ou pas ?
//#define DEBUG

/***************************
*       CommandManager     *
****************************/
  static double angleThreshold = 0.1; // Fenêtre de l'angle dans lequel on considère que le GoTo peut commencer à avancer
  static int64_t returnThreshold = 50; // Distance en dessous de laquelle on revient en marche arriere si on dépasse, sinon demi-tour

/***************************
*       ODOMETRIE          *
****************************/

  #define PI 3.14159265358979

  // Définition des variables pour l'odométrie
  static double frontParMetreCodeurG = 25650.333; // Nombre de tics codeurs en 1m pour codeur gauche
  static double frontParMetreCodeurD = 25659.0; // Nombre de tics codeurs en 1m pour codeur droite
  static int64_t distRoues = 285; // Distance entre les roues codeuses en mm
  static int64_t uOParFront = 512; // Nombre d'UO pour un tic de codeur

  //Booléens gérant une eventuelle inversion des codeurs
  static bool inverseCodeurG = true;
  static bool inverseCodeurD = false;
  static bool swapCodeurs = false;

/***************************
*         Moteurs          *
****************************/
  static int64_t V_MAX_POS_MOTOR = 90;   // MD22 : 1 a 127, vitesse maximum positive
  static int64_t V_MAX_NEG_MOTOR = -90;  // MD22 : -1 a -128, vitesse maximum negative

  //Booléens gerant une éventuelle inversion des moteurs
  static bool inverseMoteurG = true;
  static bool inverseMoteurD = true;
  static bool swapMoteurs = false;


/***************************
*           PID            *
****************************/
  // PID en distance
  static int64_t DIST_KP = 70; // Coeff proportionelle
  static int64_t DIST_KI = 0; // Coeff intégrale
  static int64_t DIST_KD = 500; // Coeff dérivée
  static double DIST_OUT_RATIO = 0.00001; // Coeff permettant de diminuer les valeurs du PID
  static int64_t DIST_MAX_OUTPUT = 90; // Valeur de sortie maximum pour le moteur
  static int64_t DIST_MAX_INTEGRAL = 0; // Valeur maximum de l'intégrale (0 = filtre PD)


  static int64_t ANGLE_KP = 150; // Coeff proportionelle
  static int64_t ANGLE_KI = 0; // Coeff intégrale
  static int64_t ANGLE_KD = 950; // Coeff dérivée
  static double ANGLE_OUT_RATIO = 0.000005; // Coeff permettant de diminuer les valeurs du PID
  static int64_t ANGLE_MAX_OUTPUT = 90; // Valeur de sortie maximum pour le moteur
  static int64_t ANGLE_MAX_INTEGRAL = 0; // Valeur maximum de l'intégrale (0 = filtre PD)

  // QUADRAMPDerivee
  static int64_t DIST_QUAD_1ST_POS = 800000; // Vitesse max en marche avant
  static int64_t DIST_QUAD_1ST_NEG = 800000; // Vitesse max en marche arrière
  static int64_t DIST_QUAD_AV_2ND_ACC = 6000; // Accélération max en marche avant
  static int64_t DIST_QUAD_AV_2ND_DEC = 4000; // Décélération max en marche avant
  static int64_t DIST_QUAD_AV_ANTICIPATION_GAIN_COEF = 115; // Coeff déterminant le début de la rampe de décélération en marche avant
  static int64_t DIST_QUAD_AR_2ND_ACC = 3000; // Accélération max en marche arrière
  static int64_t DIST_QUAD_AR_2ND_DEC = 3000; // Décélération max en marche arrière
  static int64_t DIST_QUAD_AR_ANTICIPATION_GAIN_COEF = 95; // Coeff déterminant le début de la rampe de décélération en marche arrière
  static int64_t DIST_TAILLE_FENETRE_ARRIVEE = 200000; // Largeur de la zone où l'on considère être arrivé (UO)

  static int64_t ANGLE_QUAD_1ST_POS = 700000; // Vitesse max en rotation
  static int64_t ANGLE_QUAD_2ND_ACC = 6000; // Accélération max en rotation
  static int64_t ANGLE_QUAD_2ND_DEC = 6000; // Décélération max en rotation
  static int64_t ANGLE_QUAD_ANTICIPATION_GAIN_COEF = 35; // Coeff déterminant le début de la rampe de décélération en rotation
  static int64_t ANGLE_TAILLE_FENETRE_ARRIVEE = 100000; // Largeur de la zone où l'on considère être arrivé (UO)

#endif

#include "mbed.h"

// Retour PC
  Serial pc(USBTX, USBRX);

/***************************
*       CommandManager     *
****************************/
  double angleThreshold = 0.1; // Fenêtre de l'angle dans lequel on considère que le GoTo peut commencer à avancer
  int64_t returnThreshold = 50; // Distance en dessous de laquelle on revient en marche arriere si on dépasse, sinon demi-tour

/***************************
*       ODOMETRIE          *
****************************/

  // Définition des variables pour l'odométrie
  double frontParMetreCodeurG = 25650.333; // Nombre de tics codeurs en 1m pour codeur gauche
  double frontParMetreCodeurD = 25659.0; // Nombre de tics codeurs en 1m pour codeur droite
  int64_t distRoues = 285; // Distance entre les roues codeuses en mm
  int64_t uOParFront = 512; // Nombre d'UO pour un tic de codeur

  //Booléens gérant une eventuelle inversion des codeurs
  bool inverseCodeurG = true;
  bool inverseCodeurD = false;
  bool swapCodeurs = false;

/***************************
*         Moteurs          *
****************************/
  int64_t V_MAX_POS_MOTOR = 90;   // MD22 : 1 a 127, vitesse maximum positive
  int64_t V_MAX_NEG_MOTOR = -90;  // MD22 : -1 a -128, vitesse maximum negative

  // Vitesses à partir desquelles le robot commence à bouger, pour ne pas
  // envoyer de jus dans des moteurs qui tournent pas
  int64_t V_MIN_POS_MOTOR = 10;   // MD22 : 1 a 127
  int64_t V_MIN_NEG_MOTOR = -10;  // MD22 : -1 a -128

  //Booléens gerant une éventuelle inversion des moteurs
  bool inverseMoteurG = true;
  bool inverseMoteurD = true;
  bool swapMoteurs = false;


/***************************
*           PID            *
****************************/
  // PID en distance
  int64_t DIST_KP = 70; // Coeff proportionelle
  int64_t DIST_KI = 0; // Coeff intégrale
  int64_t DIST_KD = 500; // Coeff dérivée
  double DIST_OUT_RATIO = 0.00001; // Coeff permettant de diminuer les valeurs du PID
  int64_t DIST_MAX_OUTPUT = 90; // Valeur de sortie maximum pour le moteur
  int64_t DIST_MAX_INTEGRAL = 0; // Valeur maximum de l'intégrale (0 = filtre PD)


  int64_t ANGLE_KP = 150; // Coeff proportionelle
  int64_t ANGLE_KI = 0; // Coeff intégrale
  int64_t ANGLE_KD = 950; // Coeff dérivée
  double ANGLE_OUT_RATIO = 0.000005; // Coeff permettant de diminuer les valeurs du PID
  int64_t ANGLE_MAX_OUTPUT = 90; // Valeur de sortie maximum pour le moteur
  int64_t ANGLE_MAX_INTEGRAL = 0; // Valeur maximum de l'intégrale (0 = filtre PD)

  // QUADRAMPDerivee
  int64_t DIST_QUAD_1ST_POS = 800000; // Vitesse max en marche avant
  int64_t DIST_QUAD_1ST_NEG = 800000; // Vitesse max en marche arrière
  int64_t DIST_QUAD_AV_2ND_ACC = 6000; // Accélération max en marche avant
  int64_t DIST_QUAD_AV_2ND_DEC = 4000; // Décélération max en marche avant
  int64_t DIST_QUAD_AV_ANTICIPATION_GAIN_COEF = 80; // Coeff déterminant le début de la rampe de décélération en marche avant
  int64_t DIST_QUAD_AR_2ND_ACC = 3000; // Accélération max en marche arrière
  int64_t DIST_QUAD_AR_2ND_DEC = 3000; // Décélération max en marche arrière
  int64_t DIST_QUAD_AR_ANTICIPATION_GAIN_COEF = 75; // Coeff déterminant le début de la rampe de décélération en marche arrière
  int64_t DIST_TAILLE_FENETRE_ARRIVEE = 200000; // Largeur de la zone où l'on considère être arrivé (UO)

  int64_t ANGLE_QUAD_1ST_POS = 700000; // Vitesse max en rotation
  int64_t ANGLE_QUAD_2ND_ACC = 6000; // Accélération max en rotation
  int64_t ANGLE_QUAD_2ND_DEC = 6000; // Décélération max en rotation
  int64_t ANGLE_QUAD_ANTICIPATION_GAIN_COEF = 35; // Coeff déterminant le début de la rampe de décélération en rotation
  int64_t ANGLE_TAILLE_FENETRE_ARRIVEE = 100000; // Largeur de la zone où l'on considère être arrivé (UO)

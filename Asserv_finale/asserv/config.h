#ifndef CONFIG
#define CONFIG

/***************************
*       CommandManager     *
****************************/
  static double angleThreshold = 0; // Fenêtre de l'angle dans lequel on considère que le GoTo peut commencer à avancer

/***************************
*       ODOMETRIE          *
****************************/
  
  #define PI 3.14159265358979

  // Définition des variables pour l'odométrie
  static double frontParMetreCodeurG = 25661.444; // Nombre de tics codeurs en 1m pour codeur gauche
  static double frontParMetreCodeurD = 25703.778; // Nombre de tics codeurs en 1m pour codeur droite
  static int64_t distRoues = 284; // Distance entre les roues codeuses en mm
  static double uOParFront = 512; // Nombre d'UO pour un tic de codeur

  //Booleens gerant une eventuelle inversion des codeurs
  static bool inverseCodeurG = false;
  static bool inverseCodeurD = false;
  static bool swapCodeurs = false;

/***************************
*         Moteurs          *
****************************/
  static int64_t V_MAX_POS_MOTOR = 90;   // MD22 : 1 a 127, vitesse maximum positive
  static int64_t V_MAX_NEG_MOTOR = -90;  // MD22 : -1 a -128, vitesse maximum negative

/***************************
*           PID            *
****************************/
  // PID en distance
  static int64_t DIST_KP = 70; // Coeff proportionelle
  static int64_t DIST_KI = 0; // Coeff intégrale
  static int64_t DIST_KD = 250; // Coeff dérivée
  static double DIST_OUT_RATIO = 0.00001; // Coeff permettant de diminuer les valeurs du PID
  static int64_t DIST_MAX_OUTPUT = 90; // Valeur de sortie maximum pour le moteur
  static int64_t DIST_MAX_INTEGRAL = 0; // Valeur maximum de l'intégrale (0 = filtre PD)


  static int64_t ANGLE_KP = 90; // Coeff proportionelle
  static int64_t ANGLE_KI = 0; // Coeff intégrale
  static int64_t ANGLE_KD = 1250; // Coeff dérivée
  static double ANGLE_OUT_RATIO = 0.000005; // Coeff permettant de diminuer les valeurs du PID
  static int64_t ANGLE_MAX_OUTPUT = 50; // Valeur de sortie maximum pour le moteur
  static int64_t ANGLE_MAX_INTEGRAL = 0; // Valeur maximum de l'intégrale (0 = filtre PD)

  // QUADRAMPDerivee                        
  static int64_t DIST_QUAD_1ST_POS = 400000; // Vitesse max en marche avant
  static int64_t DIST_QUAD_1ST_NEG = 150000; // Vitesse max en marche arrière
  static int64_t DIST_QUAD_AV_2ND_ACC = 1600; // Accélération max en marche avant
  static int64_t DIST_QUAD_AV_2ND_DEC = 8000; // Décélération max en marche avant
  static int64_t DIST_QUAD_AV_ANTICIPATION_GAIN_COEF = 42; // Coeff déterminant le début de la rampe de décélération en marche avant
  static int64_t DIST_QUAD_AR_2ND_ACC = 6000; // Accélération max en marche arrière
  static int64_t DIST_QUAD_AR_2ND_DEC = 1000; // Décélération max en marche arrière
  static int64_t DIST_QUAD_AR_ANTICIPATION_GAIN_COEF = 80; // Coeff déterminant le début de la rampe de décélération en marche arrière
  static int64_t DIST_TAILLE_FENETRE_ARRIVEE = 300000; // Largeur de la zone où l'on considère être arrivé (UO)

  static int64_t ANGLE_QUAD_1ST_POS = 70000; // Vitesse max en rotation
  static int64_t ANGLE_QUAD_2ND_ACC = 600; // Accélération max en rotation
  static int64_t ANGLE_QUAD_2ND_DEC = 400; // Décélération max en rotation
  static int64_t ANGLE_QUAD_ANTICIPATION_GAIN_COEF = 50; // Coeff déterminant le début de la rampe de décélération en rotatio=
  static int64_t ANGLE_TAILLE_FENETRE_ARRIVEE = 100000; // Largeur de la zone où l'on considère être arrivé (UO)

#endif

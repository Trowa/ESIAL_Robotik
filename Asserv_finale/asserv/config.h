#ifndef CONFIG
#define CONFIG

/***************************
*       ODOMETRIE          *
****************************/

  // D�finition des variables pour l'odom�trie
  static double frontParMetreCodeurG = 0; // Nombre de tics codeurs en 1m pour codeur gauche
  static double frontParMetreCodeurD = 0; // Nombre de tics codeurs en 1m pour codeur droite
  static int64_t distRoues = 0; // Distance entre les roues codeuses en mm
  static double uOParFront = 0; // Nombre d'UO pour un tic de codeur

  //Booleens gerant une eventuelle inversion des codeurs
  static bool inverseCodeurG = false;
  static bool inverseCodeurD = false;
  static bool swapCodeurs = false;

/***************************
*           PID            *
****************************/
  // PID en distance
  static int64_t DIST_KP = 70; // Coeff proportionelle
  static int64_t DIST_KI = 0; // Coeff int�grale
  static int64_t DIST_KD = 250; // Coeff d�riv�e
  static double DIST_OUT_RATIO = 0.00001; // Coeff permettant de diminuer les valeurs du PID
  static int64_t DIST_MAX_OUTPUT = 90; // Valeur de sortie maximum pour le moteur
  static int64_t DIST_MAX_INTEGRAL = 0; // Valeur maximum de l'int�grale (0 = filtre PD)


  static int64_t ANGLE_KP = 90; // Coeff proportionelle
  static int64_t ANGLE_KI = 0; // Coeff int�grale
  static int64_t ANGLE_KD = 1250; // Coeff d�riv�e
  static double ANGLE_OUT_RATIO = 0.000005; // Coeff permettant de diminuer les valeurs du PID
  static int64_t ANGLE_MAX_OUTPUT = 50; // Valeur de sortie maximum pour le moteur
  static int64_t ANGLE_MAX_INTEGRAL = 0; // Valeur maximum de l'int�grale (0 = filtre PD)

  // QUADRAMPDerivee                        
  static int64_t DIST_QUAD_1ST_POS = 400000; // Vitesse max en marche avant
  static int64_t DIST_QUAD_1ST_NEG = 150000; // Vitesse max en marche arri�re
  static int64_t DIST_QUAD_AV_2ND_ACC = 1600; // Acc�l�ration max en marche avant
  static int64_t DIST_QUAD_AV_2ND_DEC = 8000; // D�c�l�ration max en marche avant
  static int64_t DIST_QUAD_AV_ANTICIPATION_GAIN_COEF = 42; // Coeff d�terminant le d�but de la rampe de d�c�l�ration en marche avant
  static int64_t DIST_QUAD_AR_2ND_ACC = 6000; // Acc�l�ration max en marche arri�re
  static int64_t DIST_QUAD_AR_2ND_DEC = 1000; // D�c�l�ration max en marche arri�re
  static int64_t DIST_QUAD_AR_ANTICIPATION_GAIN_COEF = 80; // Coeff d�terminant le d�but de la rampe de d�c�l�ration en marche arri�re
  static int64_t DIST_TAILLE_FENETRE_ARRIVEE = 300000; // Largeur de la zone o� l'on consid�re �tre arriv� (UO)

  static int64_t ANGLE_QUAD_1ST_POS = 70000; // Vitesse max en rotation
  static int64_t ANGLE_QUAD_2ND_ACC = 600; // Acc�l�ration max en rotation
  static int64_t ANGLE_QUAD_2ND_DEC = 400; // D�c�l�ration max en rotation
  static int64_t ANGLE_QUAD_ANTICIPATION_GAIN_COEF = 50; // Coeff d�terminant le d�but de la rampe de d�c�l�ration en rotatio=
  static int64_t ANGLE_TAILLE_FENETRE_ARRIVEE = 100000; // Largeur de la zone o� l'on consid�re �tre arriv� (UO)

#endif

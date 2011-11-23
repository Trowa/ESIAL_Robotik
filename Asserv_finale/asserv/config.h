#ifndef CONFIG
#define CONFIG

  // Retour PC
  static Serial pc(USBTX, USBRX);

/***************************
*       CommandManager     *
****************************/
  static double angleThreshold = 0.05; // Fen�tre de l'angle dans lequel on consid�re que le GoTo peut commencer � avancer
  static int64_t returnThreshold = 50; // Distance en dessous de laquelle on revient en marche arriere si on depasse, sinon demi-tour

/***************************
*       ODOMETRIE          *
****************************/
  
  #define PI 3.14159265358979

  // D�finition des variables pour l'odom�trie
  static double frontParMetreCodeurG = 25661.444; // Nombre de tics codeurs en 1m pour codeur gauche
  static double frontParMetreCodeurD = 25703.778; // Nombre de tics codeurs en 1m pour codeur droite
  static int64_t distRoues = 284; // Distance entre les roues codeuses en mm
  static int64_t uOParFront = 512; // Nombre d'UO pour un tic de codeur

  //Booleens gerant une eventuelle inversion des codeurs
  static bool inverseCodeurG = true;
  static bool inverseCodeurD = false;
  static bool swapCodeurs = false;

/***************************
*         Moteurs          *
****************************/
  static int64_t V_MAX_POS_MOTOR = 70;   // MD22 : 1 a 127, vitesse maximum positive
  static int64_t V_MAX_NEG_MOTOR = -70;  // MD22 : -1 a -128, vitesse maximum negative

  //Booleens gerant une eventuelle inversion des codeurs
  static bool inverseMoteurG = true;
  static bool inverseMoteurD = true;
  static bool swapMoteurs = false;

/***************************
*           PID            *
****************************/
  // PID en distance
  static int64_t DIST_KP = 70; // Coeff proportionelle
  static int64_t DIST_KI = 0; // Coeff int�grale
  static int64_t DIST_KD = 242; // Coeff d�riv�e
  static double DIST_OUT_RATIO = 0.00001; // Coeff permettant de diminuer les valeurs du PID
  static int64_t DIST_MAX_OUTPUT = 90; // Valeur de sortie maximum pour le moteur
  static int64_t DIST_MAX_INTEGRAL = 0; // Valeur maximum de l'int�grale (0 = filtre PD)


  static int64_t ANGLE_KP = 250; // Coeff proportionelle
  static int64_t ANGLE_KI = 0; // Coeff int�grale
  static int64_t ANGLE_KD = 300; // Coeff d�riv�e
  static double ANGLE_OUT_RATIO = 0.000005; // Coeff permettant de diminuer les valeurs du PID
  static int64_t ANGLE_MAX_OUTPUT = 50; // Valeur de sortie maximum pour le moteur
  static int64_t ANGLE_MAX_INTEGRAL = 0; // Valeur maximum de l'int�grale (0 = filtre PD)

  // QUADRAMPDerivee                        
  static int64_t DIST_QUAD_1ST_POS = 400000; // Vitesse max en marche avant
  static int64_t DIST_QUAD_1ST_NEG = 150000; // Vitesse max en marche arri�re
  static int64_t DIST_QUAD_AV_2ND_ACC = 1600; // Acc�l�ration max en marche avant
  static int64_t DIST_QUAD_AV_2ND_DEC = 8000; // D�c�l�ration max en marche avant
  static int64_t DIST_QUAD_AV_ANTICIPATION_GAIN_COEF = 20; // Coeff d�terminant le d�but de la rampe de d�c�l�ration en marche avant
  static int64_t DIST_QUAD_AR_2ND_ACC = 6000; // Acc�l�ration max en marche arri�re
  static int64_t DIST_QUAD_AR_2ND_DEC = 1000; // D�c�l�ration max en marche arri�re
  static int64_t DIST_QUAD_AR_ANTICIPATION_GAIN_COEF = 100; // Coeff d�terminant le d�but de la rampe de d�c�l�ration en marche arri�re
  static int64_t DIST_TAILLE_FENETRE_ARRIVEE = 100000; // Largeur de la zone o� l'on consid�re �tre arriv� (UO)

  static int64_t ANGLE_QUAD_1ST_POS = 70000; // Vitesse max en rotation
  static int64_t ANGLE_QUAD_2ND_ACC = 600; // Acc�l�ration max en rotation
  static int64_t ANGLE_QUAD_2ND_DEC = 400; // D�c�l�ration max en rotation
  static int64_t ANGLE_QUAD_ANTICIPATION_GAIN_COEF = 50; // Coeff d�terminant le d�but de la rampe de d�c�l�ration en rotatio=
  static int64_t ANGLE_TAILLE_FENETRE_ARRIVEE = 100000; // Largeur de la zone o� l'on consid�re �tre arriv� (UO)

#endif

/* Ce fichier définit les différents paramètres de l'asservissement.
 *
 * Il est utilisé pour permettre de changer dynamiquement ces options
 * à partir de leur nom (utile pour les fichiers de configuration)
 *
 * Il est inclut à plusieurs endroit du code :
 *  - une première fois pour déclarer (dans la classe Config) un
 *    attribut associé à chaque paramètre (config.h)
 *
 *  - une seconde pour le définir (config.cpp)
 *
 *  - une troisième fois pour construire un tableau d'objets Parameter
 *    permettant d'accéder dynamiquement aux différents paramètres
 *    (association nom <-> pointeur) (config.cpp)
 *
 * Grâce à cet artifice, on évite de dupliquer la liste des paramètres.
 * La syntaxe un peu barbare fait appel à une macro dont la définition
 * est différente selon ce que l'on veut en faire
 *
 * Prenons par exemple la ligne :
 *
 *     PARAM(int64_t, maProperiete)
 *
 * Si avant l'inclusion de ce fichier, on définit la macro PARAM comme suit :
 *
 *     #define PARAM(type, nom) static type nom;
 *
 * Alors cette ligne sera remplacée par :
 *
 *     int64_t maPropriete;
 *
 * De même, si PARAM est définie comme suit :
 *
 * 	   #define PARAM(type, nom) Parameter(#nom, &Config::nom),
 *
 * La ligne sera alors remplacée par
 *
 *     Parameter("maPropriete", &Config::maPropriete),
 *
 * On notera que la syntaxe '#nom' donne "maPropriete" alors que 'nom' seul
 * donnera maPropriete (sans les guillemets).
 * C'est une fonctionnalité bien utile des macros !
 *
 * ----- Définition des paramètres -----
 * Toutes les lignes commençant par // seront incluses lors de la génération
 * du fichier texte de configuration.
 */

//--------------------------//
//         Général          //
//--------------------------//

// Version du fichier ce configuration (date, mois, jour, puis révision entre 01 et 99)
// Exemple : 2013030801 : révision 01 le 08/03/2013
PARAM(int64_t, configVersion)

//--------------------------//
//     Modes de réglage     //
//--------------------------//

// Désactivation du command manager (le robot n'est plus asservi)
PARAM(bool, disableAsserv)

// Réglage des codeurs (on affiche simplement les valeurs des codeurs)
PARAM(bool, reglageCodeurs)

// Désactiver la QuadRampDérivée en distance
PARAM(bool, disableDistanceQuad)

// Désactiver la QuadRampDérivée en angle
PARAM(bool, disableAngleQuad)

// Désactiver le régulateur de distance
PARAM(bool, disableDistanceRegu)

// Désactiver le régulateur d'angle
PARAM(bool, disableAngleRegu)

//--------------------------//
//       CommandManager     //
//--------------------------//

// Fenêtre de l'angle dans lequel on considère que le GoTo peut commencer à avancer
PARAM(double, angleThreshold)

// Distance en dessous de laquelle on revient en marche arriere si on dépasse, sinon demi-tour
PARAM(int64_t, returnThreshold)

// Distance au point de consigne à partir de laquelle on va essayer d'enchainer la consigne suivante
PARAM(int64_t, enchainThreshold)

//--------------------------//
//        Odométrie         //
//--------------------------//

// Distance entre le point asservit (centre des deux roues) et l'arrière du robot pour mettre le (0,0) au coin de la table
PARAM(int64_t, placementOrigineX)
PARAM(int64_t, placementOrigineY)

// Nombre de tics codeurs en 1m pour codeur gauche
// !!! ATTENTION : redémarrer l'asserv pour prendre en compte les changements !!
PARAM(double, frontParMetreCodeurG)

// Nombre de tics codeurs en 1m pour codeur droite
// !!! ATTENTION : redémarrer l'asserv pour prendre en compte les changements !!
PARAM(double, frontParMetreCodeurD)

// Distance entre les roues codeuses en mm
// !!! ATTENTION : redémarrer l'asserv pour prendre en compte les changements !!
PARAM(int64_t, distRoues)

// Nombre d'UO pour un tic de codeur
// !!! ATTENTION : redémarrer l'asserv pour prendre en compte les changements !!
PARAM(int64_t, uOParFront)

// Inversion du sens du codeur gauche
PARAM(bool, inverseCodeurG)

// Inversion du sens du codeur droit
PARAM(bool, inverseCodeurD)

// Échange des codeurs
PARAM(bool, swapCodeurs)

//--------------------------//
//         Moteurs          //
//--------------------------//

// Vitesse maximum en sens positif (MD22 : 1 à 127)
PARAM(int64_t, V_MAX_POS_MOTOR)

// Vitesse maximum en sens négatif (MD22 : 1 à 127)
PARAM(int64_t, V_MAX_NEG_MOTOR)


// Vitesses à partir desquelles le robot commence à bouger, pour ne pas
// envoyer de jus dans des moteurs qui ne tournent pas

// MD22 : 1 a 127
PARAM(int64_t, V_MIN_POS_MOTOR)

// MD22 : -1 a -128
PARAM(int64_t, V_MIN_NEG_MOTOR)

// Inversion du sens du moteur gauche
PARAM(bool, inverseMoteurG)

// Inversion du sens du moteur droit
PARAM(bool, inverseMoteurD)

// Échange des moteurs
PARAM(bool, swapMoteurs)


//--------------------------//
//           PID            //
//--------------------------//


// PID en distance


// Coeff proportionelle
PARAM(int64_t, DIST_KP)

// Coeff intégrale
PARAM(int64_t, DIST_KI)

// Coeff dérivée
PARAM(int64_t, DIST_KD)

// Coeff permettant de diminuer les valeurs du PID
PARAM(double, DIST_OUT_RATIO)

// Valeur de sortie maximum pour le moteur
PARAM(int64_t, DIST_MAX_OUTPUT)

// Valeur maximum de l'intégrale (0 = filtre PD)
PARAM(int64_t, DIST_MAX_INTEGRAL)


// PID en angle


// Coeff proportionelle
PARAM(int64_t, ANGLE_KP)

// Coeff intégrale
PARAM(int64_t, ANGLE_KI)

// Coeff dérivée
PARAM(int64_t, ANGLE_KD)

// Coeff permettant de diminuer les valeurs du PID
PARAM(double, ANGLE_OUT_RATIO)

// Valeur de sortie maximum pour le moteur
PARAM(int64_t, ANGLE_MAX_OUTPUT)

// Valeur maximum de l'intégrale (0 = filtre PD)
PARAM(int64_t, ANGLE_MAX_INTEGRAL)


// QUADRAMPDerivee (distance)

// Vitesse max en marche avant
PARAM(int64_t, DIST_QUAD_1ST_POS)

// Vitesse max en marche arrière
PARAM(int64_t, DIST_QUAD_1ST_NEG)

// Accélération max en marche avant
PARAM(int64_t, DIST_QUAD_AV_2ND_ACC)

// Décélération max en marche avant
PARAM(int64_t, DIST_QUAD_AV_2ND_DEC)

// Coeff déterminant le début de la rampe de décélération en marche avant
PARAM(int64_t, DIST_QUAD_AV_ANTICIPATION_GAIN_COEF)

// Accélération max en marche arrière
PARAM(int64_t, DIST_QUAD_AR_2ND_ACC)

// Décélération max en marche arrière
PARAM(int64_t, DIST_QUAD_AR_2ND_DEC)

// Coeff déterminant le début de la rampe de décélération en marche arrière
PARAM(int64_t, DIST_QUAD_AR_ANTICIPATION_GAIN_COEF)

// Largeur de la zone où l'on considère être arrivé (UO)
PARAM(int64_t, DIST_TAILLE_FENETRE_ARRIVEE)


// QUADRAMPDerivee (angle)


// Vitesse max en rotation
PARAM(int64_t, ANGLE_QUAD_1ST_POS)

// Accélération max en rotation
PARAM(int64_t, ANGLE_QUAD_2ND_ACC)

// Décélération max en rotation
PARAM(int64_t, ANGLE_QUAD_2ND_DEC)

// Coeff déterminant le début de la rampe de décélération en rotation
PARAM(int64_t, ANGLE_QUAD_ANTICIPATION_GAIN_COEF)

// Largeur de la zone où l'on considère être arrivé (UO)
PARAM(int64_t, ANGLE_TAILLE_FENETRE_ARRIVEE)

#undef PARAM

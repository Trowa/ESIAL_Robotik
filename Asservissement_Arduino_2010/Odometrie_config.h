#ifndef ODOMETRIE_CONFIG_H
#define ODOMETRIE_CONFIG_H

/*
 * REGLAGES ODOMETRIE
 */


// Nombre de fronts montant generé par un codeur sur une distance d'un mètre
#define FRONT_PAR_METRE 2546.0



// Distance entre les roues en mètre
#define DIST_ROUES  0.135



/* On définit une unitée odométrique qui sera plus petite que les fronts des codeurs 
 *  D'une part part pour faire les calculs sur des grands nombres pour limiter les erreurs de précisions due aux opérations entres petits nombres
 *  Et d'autre part pour que le PID puisse manipuler des valeurs plus pratiques
 */
#define UO_PAR_FRONT 256


#endif

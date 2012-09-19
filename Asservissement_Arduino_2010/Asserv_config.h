#ifndef ASSERV_CONFIG
#define ASSERV_CONFIG

#include <WProgram.h>


//--------------------------------------------------------
//                       Moteurs 
//--------------------------------------------------------
#define V_MAX_POS_MOTOR 127
#define V_MAX_NEG_MOTOR -127


//--------------------------------------------------------
//                       PID
//--------------------------------------------------------
#define DIST_KP 1
#define DIST_KI 0
#define DIST_KD 0
#define DIST_OUT_RATIO 0.008
#define DIST_MAX_OUTPUT 127
#define DIST_MAX_INTEGRAL 0

#define ANGLE_KP 1
#define ANGLE_KI 0.0
#define ANGLE_KD 0
#define ANGLE_OUT_RATIO 0.01
#define ANGLE_MAX_OUTPUT 127
#define ANGLE_MAX_INTEGRAL 0


//--------------------------------------------------------
//                       QUADRAMPDerivee
//--------------------------------------------------------
#define DIST_QUAD_1ST_POS 5000
#define DIST_QUAD_1ST_NEG 5000
#define DIST_QUAD_2ND_ACC 20
#define DIST_QUAD_2ND_DEC 20
#define DIST_QUAD_ANTICIPATION_GAIN_COEF 100
#define DIST_TAILLE_FENETRE_ARRIVEE 512

#define ANGLE_QUAD_1ST_POS 3000
#define ANGLE_QUAD_1ST_NEG 3000
#define ANGLE_QUAD_2ND_ACC 20
#define ANGLE_QUAD_2ND_DEC 20
#define ANGLE_QUAD_ANTICIPATION_GAIN_COEF 0
#define ANGLE_TAILLE_FENETRE_ARRIVEE 200


#endif

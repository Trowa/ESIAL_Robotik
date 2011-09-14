#ifndef ASSERV_CONFIG
#define ASSERV_CONFIG


//--------------------------------------------------------
//                         PID
//--------------------------------------------------------

//Peuvent etre definis par les setters de la classe Pid
#define DIST_KP 70
#define DIST_KI 0
#define DIST_KD 250//160
#define DIST_OUT_RATIO 0.00001
#define DIST_MAX_OUTPUT 90
#define DIST_MAX_INTEGRAL 0
#define DIST_COEFF_REPARTITION 0.5


#define ANGLE_KP 90
#define ANGLE_KI 0
#define ANGLE_KD 1250
#define ANGLE_OUT_RATIO 0.000005
#define ANGLE_MAX_OUTPUT 50
#define ANGLE_MAX_INTEGRAL 0
#define ANGLE_COEFF_REPARTITION 0.5




//--------------------------------------------------------
//       Rampe avec anticipation de deceleration
//--------------------------------------------------------
#define DIST_MIN_DIST_DECELERATION_FORWARD 100000
#define DIST_MAX_DIST_DECELERATION_FORWARD 1500000
#define DIST_MIN_DIST_DECELERATION_BACKWARD 50000
#define DIST_MAX_DIST_DECELERATION_BACKWARD 1500000
#define DIST_DECELERATION_COEFF_FORWARD 0.6
#define DIST_DECELERATION_COEFF_BACKWARD 1.4
#define DIST_MAX_STEP_FORWARD 0.5
#define DIST_MAX_STEP_BACKWARD 10
#define DIST_MIN_DECELERATION_SPEED_FORWARD 10
#define DIST_MIN_DECELERATION_SPEED_BACKWARD 20

#define ANGLE_MIN_DIST_DECELERATION_FORWARD 4000
#define ANGLE_MAX_DIST_DECELERATION_FORWARD 20000
#define ANGLE_MIN_DIST_DECELERATION_BACKWARD 400000000
#define ANGLE_MAX_DIST_DECELERATION_BACKWARD 20000
#define ANGLE_DECELERATION_COEFF_FORWARD 0.5
#define ANGLE_DECELERATION_COEFF_BACKWARD 0.5
#define ANGLE_MAX_STEP_FORWARD 5
#define ANGLE_MAX_STEP_BACKWARD 5
#define ANGLE_MIN_DECELERATION_SPEED_FORWARD 10
#define ANGLE_MIN_DECELERATION_SPEED_BACKWARD 10

//--------------------------------------------------------
//                       QUADRAMPDerivee
//--------------------------------------------------------
                          
#define DIST_QUAD_1ST_POS 400000
#define DIST_QUAD_1ST_NEG 150000 // 143000 pour vitesse de 50
#define DIST_QUAD_AV_2ND_ACC 1600
#define DIST_QUAD_AV_2ND_DEC 8000
#define DIST_QUAD_AV_ANTICIPATION_GAIN_COEF 42
#define DIST_QUAD_AR_2ND_ACC 6000
#define DIST_QUAD_AR_2ND_DEC 1000
#define DIST_QUAD_AR_ANTICIPATION_GAIN_COEF 80
#define DIST_TAILLE_FENETRE_ARRIVEE 300000

#define ANGLE_QUAD_1ST_POS 70000
#define ANGLE_QUAD_1ST_NEG 70000
#define ANGLE_QUAD_2ND_ACC 600
#define ANGLE_QUAD_2ND_DEC 400
#define ANGLE_QUAD_ANTICIPATION_GAIN_COEF 50
#define ANGLE_TAILLE_FENETRE_ARRIVEE 100000//100000000


#endif
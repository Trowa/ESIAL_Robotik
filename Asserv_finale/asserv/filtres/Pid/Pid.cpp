#include "Pid.h"

Pid::Pid(bool isDistance)
{
  integrale = 0;

  if (isDistance) {
    kp = DIST_KP;
    ki = DIST_KI;
    kd = DIST_KD;
    outRatio = DIST_OUT_RATIO;
    maxIntegral = DIST_MAX_INTEGRAL;
    maxOutput = DIST_MAX_OUTPUT;
  } else {
    kp = ANGLE_KP;
    ki = ANGLE_KI;
    kd = ANGLE_KD;
    outRatio = ANGLE_OUT_RATIO;
    maxIntegral = ANGLE_MAX_INTEGRAL;
    maxOutput = ANGLE_MAX_OUTPUT;
  }
}

Pid::~Pid() {}

int64_t Pid::filtre(int64_t erreur , int64_t feedback_odometrie , int64_t value3)
{
    // Calcul de la Proportionnelle
    int64_t P = erreur * kp;
  
    //Calcul de l'Integrale
    integrale += erreur; 
    integrale = Utils::constrain(integrale, -maxIntegral , maxIntegral);
    int64_t I = integrale * ki;
  

    // Calcul de la Derivee
    int64_t D = feedback_odometrie * kd;
  
    //On calcul la Sortie qui est la somme des trois valeurs multipliée par le ratio de sortie
    int64_t sortie = P + I + D;
    sortie = sortie * outRatio;
  
    // Saturation de la sortie
    sortie = Utils::constrain(sortie , -maxOutput , maxOutput);
  
    return sortie;
}

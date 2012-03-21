#include "Pid.h"

// Constructeur
Pid::Pid(bool isDistance) {
  // L'int�grale est nulle au d�part
  integrale = 0;
  
  // En fonction du type de PID on ne r�cup�re pas les m�me param�tres
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

// Destructeur
Pid::~Pid() {}

// On filtre l'erreur pour calculer la sortie � donner aux moteurs
int64_t Pid::filtre(int64_t erreur , int64_t feedback_odometrie , int64_t value3)
{
    // Calcul de la Proportionnelle
    int64_t P = erreur * kp;
	
	//pc.printf("P=%d ", P);
	  
    //Calcul de l'Integrale que l'on oublie pas de borner
    integrale += erreur; 
    integrale = Utils::constrain(integrale, -maxIntegral , maxIntegral);
    int64_t I = integrale * ki;
  

    // Calcul de la Derivee
    int64_t D = feedback_odometrie * kd;
  
	//pc.printf("D=%d ", D);

    //On calcul la Sortie qui est la somme des trois valeurs multipli�e par le ratio de sortie
    int64_t sortie = P + I + D;
    sortie = sortie * outRatio;
  
    // Saturation de la sortie pour ne pas allez trop vite non plus
    sortie = Utils::constrain(sortie , -maxOutput , maxOutput);
  
    return sortie;
}

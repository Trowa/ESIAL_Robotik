#include "Pid.h"
#include "../Utils/Utils.h"

int64_t Pid::filtre(int64_t erreur , int64_t value2 , int64_t value3)
{
    //Proportionnel
    P = erreur * kp;
  
    //Integrale
    integral += erreur; 
    integral = Utils::constrain(integral, -maxI , maxI);
    I = integral * ki;
  

    // Derivee
    derivee = erreur - prev_value;
    D = derivee * kd;
    prev_value = erreur;
  
    //Sortie
    sortie = P + I + D;
    sortie = sortie * ratioSortie;
  
    //Saturation de la sortie
    sortie = Utils::constrain(sortie , -maxSortie , maxSortie);
  
    return sortie;
}
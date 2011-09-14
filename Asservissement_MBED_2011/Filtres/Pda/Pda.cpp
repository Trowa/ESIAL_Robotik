#include "Pda.h"
#include "../Utils/Utils.h"

int64_t Pda::filtre(int64_t erreur , int64_t value2 , int64_t value3)
{
    //Proportionnel
    P = erreur * kp;
    
    // Derivee
    derivee = erreur - prev_value;
    D = derivee * kd;
    
    // Acceleration
    acceleration = derivee - prev_value + prev_prev_value;
    A = acceleration * ka;
    
    prev_prev_value = prev_value;
    prev_value = erreur;
  
    //Sortie
    sortie = P + D - A;
    sortie = sortie * ratioSortie;
  
    //Saturation de la sortie
    sortie = Utils::constrain(sortie , -maxSortie , maxSortie);
  
    //printf("a=%d - p=%d - d=%d - s=%d\n", A, P, D, sortie);
  
    return sortie;
}
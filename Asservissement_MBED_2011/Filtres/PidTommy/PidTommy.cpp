#include "PidTommy.h"
#include "../Utils/Utils.h"

int64_t PidTommy::filtre(int64_t erreur , int64_t feedback, int64_t value3)
{
    //Proportionnel
    P = erreur * kp;
    

    /*  
    //Integrale
    integral += (consigne - accuFeedback); 
    integral = Utils::constrain(integral, -maxI , maxI);
    I = integral * ki;
     */

    // Derivee
    D = feedback * kd;
  
  
    //Sortie
    sortie = P  - D;
    sortie = (P * kRepartition) - ((1 - kRepartition) * D);
    //printf("P=%ld\tP*D=%ld\tP*kr=%ld\tP*D*(1-kr)=%ld\n", P, P*D, (int64_t) P * kRepartition, (int64_t) P * (1 - kRepartition) * D);
    sortie = sortie * ratioSortie;
  
    //Saturation de la sortie
    sortie = Utils::constrain(sortie , -maxSortie , maxSortie);
  
    
   // printf("Erreur = %d feedback = %d sortie = %d \n", erreur, feedback, sortie);
    return sortie;
}
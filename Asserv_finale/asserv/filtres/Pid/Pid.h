#ifndef PID
#define PID

#include "../Filtre.h"
#include "../../Utils/Utils.h"

// PID(Proportionnel-Integrale-Derivee) - Filtre sur l'erreur
 
class Pid : public Filtre
{
    public:
   
        Pid(bool isDistance);
        ~Pid();
     
        int64_t filtre(int64_t erreur , int64_t value2 = 0 , int64_t value3 = 0);
     
    private:
        // Valeur précédente de l'erreur
        int64_t prev_value;
        // Somme des erreurs
        int64_t integrale;

   
        //Coeffs
        int64_t kp, ki, kd; // Coeff venant de config.h
   
        //Ratios & Saturation
        /* Coeffs venant de config.h */
        double outRatio;
        int64_t maxIntegral;
        int64_t maxOutput;
 
 };
 
 #endif

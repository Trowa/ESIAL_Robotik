#ifndef PDA
#define PDA

#include "../Filtre.h"

// PDA(Proportionnel-Derivee-Acceleration) - Filtre sur l'erreur
// derivee sur le feedback
 
class Pda : public Filtre
{
    public:
   
        Pda()
        {
            prev_value = 0;
            prev_prev_value = 0;
        }
     
        // set coefficients
        void setKP(double newKP) { kp = newKP; }
        void setKD(double newKD) { kd = newKD; }
        void setKA(double newKA) { ka = newKA; }
     
        void setRatioSortie(double newRatio){ratioSortie = newRatio;}
        void setSortieMax(int64_t newMax){maxSortie = newMax;}
        
        
        // get coefficients
        double getKP() { return kp; }
        double getKD() { return kd; }
        double getKA() { return ka; }
        
        double getRatioSortie() { return ratioSortie; }
        int64_t getSortieMax() { return maxSortie; }
     
        int64_t filtre(int64_t erreur , int64_t value2 = 0 , int64_t value3 = 0);
     
    private:
   
        //Coeffs
        double kp, kd, ka;
   
        //Ratios & Saturation
        double ratioSortie;
        int64_t maxSortie;
        
        //Calculs
        int64_t P;
   
        int64_t prev_value;
        int64_t prev_prev_value;
        
        int64_t A;
        int64_t acceleration;
   
        int64_t D;
        int64_t derivee;
     
        int64_t sortie;
 
 };
 
 #endif
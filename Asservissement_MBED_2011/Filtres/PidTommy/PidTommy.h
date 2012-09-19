#ifndef PIDTOMMY
#define PIDTOMMY

#include "../Filtre.h"

// PID(Proportionnel-Integrale-Derivee) - Filtre sur l'erreur
// amelioration sur une proposition de M. TOMCZAK
 
class PidTommy : public Filtre
{
    public:
   
        PidTommy()
        {
            prev_value = 0;
            integral = 0;
        }
     
        // set coefficients
        void setKP(double newKP) { kp = newKP; }
        void setKI(double newKI) { ki = newKI; }
        void setKD(double newKD) { kd = newKD; }
        void setKRepartition(double newKRepartition) { kRepartition = newKRepartition; }
     
        void setMaxIntegral(int64_t newMaxI){maxI = newMaxI;}
        void setRatioSortie(double newRatio){ratioSortie = newRatio;}
        void setSortieMax(int64_t newMax){maxSortie = newMax;}
        
        
        // get coefficients
        double getKP() { return kp; }
        double getKI() { return ki; }
        double getKD() { return kd; }
        double getKRepartition() { return kRepartition; }
        
        int64_t getMaxIntegral() { return maxI; }
        double getRatioSortie() { return ratioSortie; }
        int64_t getSortieMax() { return maxSortie; }
     
        int64_t filtre(int64_t erreur, int64_t feedback, int64_t value3=0);
     
    private:
   
        //Coeffs
        double kp, ki, kd, kRepartition;
   
        //Ratios & Saturation
        double ratioSortie;
        int64_t maxI;
        int64_t maxSortie;
        
        //Calculs
        int64_t P;
   
        int64_t I;
        int64_t integral;
   
        int64_t D;
        int64_t prev_value;
        int64_t derivee;
     
        int64_t sortie;
 
 };
 
 #endif
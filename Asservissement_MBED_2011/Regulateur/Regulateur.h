#ifndef REGULATEUR
#define REGULATEUR

#include "../Filtres/Accumulateur/Accumulateur.h"
#include "../Filtres/Pid/Pid.h"
#include "../Filtres/QuadRampDerivee/QuadRampDerivee.h"
#include "../Filtres/Pda/Pda.h"
#include "../Filtres/PidTommy/PidTommy.h"
#include "../Filtres/Rampe/Rampe.h"

class Regulateur
{
   public:
   
     Regulateur() 
     {
        filtreConsigneON = true;
     
      }
     
     int64_t manage(int64_t consigne, int64_t feedback); 
     
     
     
     bool isRampFinished() { return filtreConsigne.isRampFinished(); }
     
     //parametrage des filtres
     void setKP(double newKP) { filtreErreur.setKP(newKP); }
     void setKI(double newKI) { filtreErreur.setKI(newKI); } //PID
     void setKD(double newKD) { filtreErreur.setKD(newKD); }
//     void setKA(double newKA) { filtreErreur.setKA(newKA); } //PDA
//     void setMaxIntegral(int64_t newMaxI) { filtreErreur.setMaxIntegral(newMaxI);} //PID

     void setKRepartition(double newKRepartition) { filtreErreur.setKRepartition(newKRepartition); } //PIDTommy
    
     void setRatioSortie(double newRatio) { filtreErreur.setRatioSortie(newRatio); }
     void setSortieMax(int64_t newMax) { filtreErreur.setSortieMax(newMax); }
    
    
     void setMaxStepForward(double newStepForward) { filtreSortie.setMaxStepForward(newStepForward); }
     void setMaxStepBackward(double newStepBackward) { filtreSortie.setMaxStepBackward(newStepBackward); }
     
     void setDecelerationCoeffForward(double newCoeff) { filtreSortie.setDecelerationCoeffForward(newCoeff); }
     void setDecelerationCoeffBackward(double newCoeff) { filtreSortie.setDecelerationCoeffBackward(newCoeff); }
         
     void setMinDecelerationDistForward(int64_t newDist) { filtreSortie.setMinDecelerationDistForward(newDist); }
     void setMaxDecelerationDistForward(int64_t newDist) {  filtreSortie.setMaxDecelerationDistForward(newDist); }
     void setMinDecelerationDistBackward(int64_t newDist) { filtreSortie.setMinDecelerationDistBackward(newDist); }
     void setMaxDecelerationDistBackward(int64_t newDist) { filtreSortie.setMaxDecelerationDistBackward(newDist); }
     
     void setMinDecelerationSpeedForward(int64_t newMinSpeed) { filtreSortie.setMinDecelerationSpeedForward(newMinSpeed); }
     void setMinDecelerationSpeedBackward(int64_t newMinSpeed) { filtreSortie.setMinDecelerationSpeedBackward(newMinSpeed); }
     
     
     void setDerivee1ier( int64_t positive, int64_t negative ) { filtreConsigne.setDerivee1ier( positive , negative ); }
     void setDerivee2ndAv( int64_t positive, int64_t negative ) { filtreConsigne.setDerivee2ndAv( positive, negative ); }
     void setDerivee2ndAr( int64_t positive, int64_t negative ) { filtreConsigne.setDerivee2ndAr( positive, negative ); }
     void setCoeffGainAnticipationAv( float value) { filtreConsigne.setCoeffGainAnticipationAv( value ); }
     void setCoeffGainAnticipationAr( float value) { filtreConsigne.setCoeffGainAnticipationAr( value ); }
     void setTailleFenetreArrivee(int64_t taille) { filtreConsigne.setTailleFenetreArrivee( taille ); }
     
     // recuperation des parametres des filtres
     double getKP() { return filtreErreur.getKP(); }
     double getKI() { return filtreErreur.getKI(); }
     double getKD() { return filtreErreur.getKD(); }
//     double getKA() { return filtreErreur.getKA(); }
     
//     int64_t getMaxIntegral() { return filtreErreur.getMaxIntegral(); }
     double getRatioSortie() { return filtreErreur.getRatioSortie(); }
     int64_t getSortieMax() { return filtreErreur.getSortieMax(); }
     
     
     double getMaxStepForward() { return filtreSortie.getMaxStepForward(); }
     double getMaxStepBackward() { return filtreSortie.getMaxStepBackward(); }
     
     double getDecelerationCoeffForward() { return filtreSortie.getDecelerationCoeffForward(); }
     double getDecelerationCoeffBackward() { return filtreSortie.getDecelerationCoeffBackward(); }
         
     int64_t getMinDecelerationDistForward() { return filtreSortie.getMinDecelerationDistForward(); }
     int64_t getMaxDecelerationDistForward() { return filtreSortie.getMaxDecelerationDistForward(); }
     int64_t getMinDecelerationDistBackward() { return filtreSortie.getMinDecelerationDistBackward(); }
     int64_t getMaxDecelerationDistBackward() { return filtreSortie.getMaxDecelerationDistBackward(); }
     
     int64_t getMinDecelerationSpeedForward() { return filtreSortie.getMinDecelerationSpeedForward(); }
     int64_t getMinDecelerationSpeedBackward() { return filtreSortie.getMinDecelerationSpeedBackward(); }
     
     
     int64_t getDerivee1ierPos() { return filtreConsigne.getDerivee1ierPos(); }
     int64_t getDerivee1ierNeg() { return filtreConsigne.getDerivee1ierNeg(); }    
     int64_t getDerivee2ndPosAv() { return filtreConsigne.getDerivee2ndPosAv(); }
     int64_t getDerivee2ndNegAv() { return filtreConsigne.getDerivee2ndNegAv(); }
     double getCoeffGainAnticipationAv() { return filtreConsigne.getCoeffGainAnticipationAv(); }
     int64_t getDerivee2ndPosAr() { return filtreConsigne.getDerivee2ndPosAr(); }
     int64_t getDerivee2ndNegAr() { return filtreConsigne.getDerivee2ndNegAr(); }
     double getCoeffGainAnticipationAr() { return filtreConsigne.getCoeffGainAnticipationAr(); }
     int getTailleFenetreArrivee() { return filtreConsigne.getTailleFenetreArrivee(); }
     int64_t getPositionPivot() { return filtreConsigne.getPositionPivot(); }
     
     int64_t getErreur(){ return erreur; }
     
     int64_t getCurrentAccu() { return feedbackFiltre; }
     
     void setConsigneOnOff(bool val) {filtreConsigneON = val;}
     
     void reset_accumulator()
     {
        filtreFeedback.reset();
        feedbackFiltre = 0; // Toujours aussi moche !!
     }
     
   private:
     // Filtres utilises 
     QuadRampDerivee filtreConsigne;
     Accumulateur filtreFeedback;
     //Pda filtreErreur;
     //Pid filtreErreur;
     PidTommy filtreErreur;
     Rampe filtreSortie;
     
     //Valeurs retournees
     int64_t consigneFiltree;
     int64_t feedbackFiltre;
     int64_t erreur, erreurFiltree, sortie;
     
     bool filtreConsigneON;
  
};


#endif
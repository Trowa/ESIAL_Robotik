#ifndef QUADRAMPDERIVEE
#define QUADRAMPDERIVEE

#include "../Filtre.h"

class QuadRampDerivee : public Filtre
 {
   public:
   
     QuadRampDerivee()
     {
       diviseurFrequence = 0;
       divisdeurFrequenceCompteur = diviseurFrequence;
       prevPosition = 0;
       prevConsigneVitesse = 0;
       prevConsignePosition = 0;
       tailleFenetreArrivee = 0;
       arrivee=true;
       gainAnticipation_av = 0;
       gainAnticipation_ar = 0;
     }
     
     
     // configuration
     void setDerivee1ier( int64_t positive, int64_t negative )
     {
         derivee_1ier_pos = positive;
         derivee_1ier_neg = negative;
     }
     
     int64_t getDerivee1ierPos() { return derivee_1ier_pos; }
     int64_t getDerivee1ierNeg() { return derivee_1ier_neg; }
     
     

     void setDerivee2ndAv( int64_t positive, int64_t negative )
     {
         derivee_2nd_pos_av = positive;
         derivee_2nd_neg_av = negative;
     }
     
     void setDerivee2ndAr( int64_t positive, int64_t negative )
     {
         derivee_2nd_pos_ar = positive;
         derivee_2nd_neg_ar = negative;
     }
     
     int64_t getDerivee2ndPosAv() { return derivee_2nd_pos_av; }
     int64_t getDerivee2ndNegAv() { return derivee_2nd_neg_av; }
     int64_t getDerivee2ndPosAr() { return derivee_2nd_pos_ar; }
     int64_t getDerivee2ndNegAr() { return derivee_2nd_neg_ar; }
     
     
     void setCoeffGainAnticipationAv( double value) { gainAnticipation_av = value; }
     double getCoeffGainAnticipationAv() { return gainAnticipation_av; }
     void setCoeffGainAnticipationAr( double value) { gainAnticipation_ar = value; }
     double getCoeffGainAnticipationAr() { return gainAnticipation_ar; }
     
     void setTailleFenetreArrivee(int64_t taille) { tailleFenetreArrivee = taille; }
     int getTailleFenetreArrivee() { return tailleFenetreArrivee; }
     
     
     bool isRampFinished() { return arrivee; }

     int64_t filtre(int64_t consigne, int64_t position_actuelle , int64_t vitesse);
     
     int64_t getPositionPivot(){ return position_pivot;}
     
     
 
   private:
   
     // Diviseur ( pour ne pas appliquer une consigne differente a chaque iteration )
     int diviseurFrequence;
     int divisdeurFrequenceCompteur;
     int64_t position_pivot;
     
     // Pour calcul de la vitesse ( derivee )
     int64_t prevPosition;
     
     // Derivees premieres de la consigne ( dans notre cas, la vitesse )
     int64_t derivee_1ier_pos; //En avant ( positif )
     int64_t derivee_1ier_neg; // En arriere (negatif )

    // Derivees secondes de la consigne ( dans notre cas, l'acceleration ) 
     int64_t derivee_2nd_pos_av; //En avant ( positif )
     int64_t derivee_2nd_neg_av; // En arriere (negatif - deceleration)
     
     //la meme chose pour le deplacement vers l'arriere
     int64_t derivee_2nd_pos_ar;
     int64_t derivee_2nd_neg_ar;
     
     //On ajoute de l' "anticipation " pour eviter que la vitesse ne chute brutalement a zero quand le point d'arrivee arrive....( bien pourri ca dis donc -_-" )
     double gainAnticipation_av;
     double gainAnticipation_ar;

     // Sauvegarde des valeurs precedentes
     int64_t prevConsigneVitesse;
     int64_t prevConsignePosition;
     
     //Pour eviter les vibrations a l'arrivee
     int64_t tailleFenetreArrivee;
     
     // Permet de connaitre l'etat de la consigne actuelle ( on est arrivee ou non... )
     bool arrivee;

     
 };
 
 #endif
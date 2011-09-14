#ifndef FILTRE
#define FILTRE

#include <WProgram.h>
#import "Odometrie.h"

/*
 *  Protocole ( au sens java du terme , dsl -_-' ) représentant les filtres
 */

class Filtre
{
    public:
      virtual long filtre(long value) = 0;
};




/*
 * Accumulateur - Filtre sur le feedback
 */
 
 class Accumulateur : public Filtre
 {
   public:
     Accumulateur() : total(0){} 
 
     long filtre( long value)
     {
       total += value;
       return total;
     }
 
   private:
     float total;
 };
 


/*
 * PID - Filtre sur l'erreur
 */ 
 
 class Pid : public Filtre
 {
   public:
   
     Pid()
     {
         prev_value = 0;
         integral = 0;
     }
     
     void setCoeffs( float kp ,float ki ,float kd )
     {
         Kp = kp; Ki = ki; Kd = kd;
     }
     
     void setMaxIntegral(long maxi){maxI = maxi;}
     void setRatioSortie(float ratio){ratioSortie = ratio;}
     void setSortieMax(long maxi){maxSortie=maxi;}
     
     long filtre(long erreur);
     
   private:
   
   //Coeffs
   float Kp, Ki, Kd;
   
   //Ratios & Saturation
   float ratioSortie;
   long maxI;
   long maxSortie;
   
   //Calculs
     long P;
   
     long I;
     long integral;
   
     long D;
     long prev_value;
     long derivee;
     
     long sortie;
 
 };
 
 
 
 
 /*
 * QUADRAMP- Filtre sur la consigne
 */ 
 
 class QuadRampDerivee
 {
   public:
   
     QuadRampDerivee()
     {
       diviseurFrequence = 1;
       divisdeurFrequenceCompteur = diviseurFrequence;
       prevPosition = 0;
       prevConsigneVitesse = 0;
       prevConsignePosition = 0;
       tailleFenetreArrivee = 0;
       arrivee=false;
       gainAnticipation = 0;
       
       //Debug
       disabled = true;
     }
     
     void setDerivee1ier( long positive, long negative )
     {
         derivee_1ier_pos = positive;
         derivee_1ier_neg = negative;
     }

     void setDerivee2nd( long positive, long negative )
     {
         derivee_2nd_pos = positive;
         derivee_2nd_neg = negative;
     }
     
     void setCoeffGainAnticipation( float value)
     {
        gainAnticipation = value; 
     }
     
     void setTailleFenetreArrivee(int taille)
     {
        tailleFenetreArrivee = taille;  
     }
     
     boolean isRampFinished()
     {
       return arrivee;
     }

     long filtre(long consigne, long position_actuelle , long vitesse);
     
     
     //Debug
     boolean disabled;
     
     
 
   private:
   
     // Diviseur ( pour ne pas appliquer une consigne différente à chaque itération )
     int diviseurFrequence;
     int divisdeurFrequenceCompteur;
     
     // Pour calcul de la vitesse ( dérivée )
     long prevPosition;
     
     // Derivées premières de la consigne ( dans notre cas, la vitesse )
     long derivee_1ier_pos; //En avant ( positif )
     long derivee_1ier_neg; // En arrière (négatif )

    // Dérivées secondes de la consigne ( dans notre cas, l'accélération ) 
     long derivee_2nd_pos; //En avant ( positif )
     long derivee_2nd_neg; // En arrière (négatif )     
     
     // Sauvegarde des valeurs precedentes
     long prevConsigneVitesse;
     long prevConsignePosition;
     
     //Pour éviter les vibrations à l'arrivée
     int tailleFenetreArrivee;
     
     //On ajoute de l' "anticipation " pour éviter que la vitesse ne chute brutalement à zero quand le point d'arrivée arrive....( bien pourri ça dis donc -_-" )
     float gainAnticipation;
     
     // Permet de connaitre l'etat de la consigne actuelle ( on est arrivée ou non... )
     boolean arrivee;

     
 };
#endif


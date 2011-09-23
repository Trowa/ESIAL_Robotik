#include "QuadRampDerivee.h"
#include "../Utils/Utils.h"

//--------------------------------------------------------
//                      QUADRAMPm
//-------------------------------------------------------
int64_t QuadRampDerivee::filtre(int64_t consigne, int64_t position_actuelle , int64_t vitesse)
{
    /*
     *  Diviseur permettant de ne pas changer de consigne a chaque iteration
     *    Et donc de laisser du temps au PID pour arriver a la position voulue
     */
    if( diviseurFrequence > 1 )
    {
      if( --divisdeurFrequenceCompteur > 0 ) 
      {
        return prevConsignePosition;
      } 
      divisdeurFrequenceCompteur = diviseurFrequence;
    }
    
    // Reset de certains "flag" qui seront determines au cours de la fonction
   arrivee = false;


   //Calcul de la position du pivot
   
    char sens = ( consigne - position_actuelle >= 0 ) ? 1 : -1;
    if(sens == 1) {
        position_pivot = consigne + ( ( vitesse >= 0 ) ? -1 : 1) * ( ((vitesse * vitesse )/(2*derivee_2nd_neg_av)) + abs(vitesse) * gainAnticipation_av );
    } else {
        position_pivot = consigne + ( ( vitesse >= 0 ) ? -1 : 1) * ( ((vitesse * vitesse )/(2*derivee_2nd_neg_ar)) + abs(vitesse) * gainAnticipation_ar );
    }
    
    

   //Calcul de la consigne d'acceleration  
   int64_t acceleration_consign;
   
   if( position_pivot >= position_actuelle )
     acceleration_consign = (sens == 1) ? derivee_2nd_pos_av : derivee_2nd_neg_ar;
   else
     acceleration_consign = (sens == 1) ? -derivee_2nd_neg_av : -derivee_2nd_pos_ar;
  

/* Pkoi dans le calcul de la position du pivot tu utilise la vitesse ??? &#65533;a serait pas plus logiquer d'utiliser le sens qu'on vient de calculer? 
    D'ailleurs &#65533;a peut se simplifier vachement ou bien?? * /
   
   
   //EXEMPLE : 
    char sens = ( consigne - position_actuelle >= 0 ) ? -1 : 1;
    int64_t d2_neg = ( sens == -1 ) ? derivee_2nd_neg_av : derivee_2nd_neg_ar;
    double gainAnticipation = ( sens == -1 ) ? gainAnticipation_av : gainAnticipation_ar;
    position_pivot = consigne + sens * ( ((vitesse * vitesse )/(2*d2_neg)) + abs(vitesse) * gainAnticipation );
    
   int64_t acceleration_consign;
   
   if( position_pivot >= position_actuelle )
     acceleration_consign = (sens == -1) ? derivee_2nd_pos_av : derivee_2nd_neg_ar;
   else
     acceleration_consign = (sens == -1) ? -derivee_2nd_neg_av : -derivee_2nd_pos_ar;
    
*/

  
  
   //Calcul de la consigne de vitesse  
   int64_t consigneVitesse = prevConsigneVitesse + acceleration_consign;
   consigneVitesse = Utils::constrain(consigneVitesse,-derivee_1ier_neg, derivee_1ier_pos);
      prevConsigneVitesse = consigneVitesse;
      
      
    
   // On verifie si on est dans la fenetre d'arrivee
   if ( abs(consigne-position_actuelle) < tailleFenetreArrivee )
   {
      prevConsigneVitesse = 0; // On reset la consigne precedente 
     arrivee = true;
     return consigne;
   }
   
  
   //Calcul de la consigne de position
   prevConsignePosition = position_actuelle+consigneVitesse;
 
   return prevConsignePosition; 
}
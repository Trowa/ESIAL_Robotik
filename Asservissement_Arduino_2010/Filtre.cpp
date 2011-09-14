#import "Filtre.h"


//--------------------------------------------------------
//                       PID
//--------------------------------------------------------

long Pid::filtre(long erreur)
{
 
   //Proportionnel
   P = erreur * Kp;
  
  //Intégral
  integral += erreur; 
  integral = constrain(integral, -maxI,maxI);
  I = integral * Ki;
  

  // Dérivée
  derivee = erreur - prev_value;
  D = derivee * Kd;
  prev_value = erreur;
  
  //Sortie
  sortie = P + I + D;
  sortie = sortie * ratioSortie;
  
  //Saturation de la sortie
  sortie = constrain(sortie , -maxSortie , maxSortie);
  
   return sortie;
}


//--------------------------------------------------------
//                      QUADRAMPm
//-------------------------------------------------------
long QuadRampDerivee::filtre(long consigne, long position_actuelle , long vitesse)
{
  //DEBUG
  if( disabled)
  {
   if ( abs(consigne-position_actuelle) < tailleFenetreArrivee )
   {
     arrivee = true;
   }
    
    return consigne;
  }
    
   
    /*
     *  Diviseur permettant de ne pas changer de consigne à chaque itération
     *    Et donc de laisser du temps au PID pour arriver à la position voulue
     */
    if( diviseurFrequence > 1 )
    {
      if( --divisdeurFrequenceCompteur > 0 ) 
      {
        return prevConsignePosition;
      } 
      divisdeurFrequenceCompteur = diviseurFrequence;
    }
    
    // Reset de certains "flag" qui seront determinés au cours de la fonction
   arrivee = false;
    
   //Choix de la dérivée seconde ( accélération ou décéllération) en fonction 
   long derivee2nd = (vitesse >= 0 ) ? derivee_2nd_pos : derivee_2nd_neg;


   //Calcul de la position du pivot
   long sens = ( vitesse >= 0 ) ? -1 : 1;
   long position_pivot = consigne +  sens * ( ((vitesse * vitesse )/(2*derivee_2nd_neg)) + abs(vitesse) * gainAnticipation );
   
   
   //Calcul de la consigne d'accéleration  
   long acceleration_consign;
   if( position_pivot >= position_actuelle )
     acceleration_consign = derivee_2nd_pos;
   else
     acceleration_consign = -derivee_2nd_neg;
     
   //Calcul de la consigne de vitesse  
   long consigneVitesse = prevConsigneVitesse + acceleration_consign;
   consigneVitesse = constrain(consigneVitesse,-derivee_1ier_neg, derivee_1ier_pos);
      prevConsigneVitesse = consigneVitesse;
   
   
   // On vérifie si on est dans la fenetre d'arrivée
   if ( abs(consigne-position_actuelle) < tailleFenetreArrivee )
   {
     arrivee = true;
     return consigne;
   }
   
   //Calcul de la consigne de position
   prevConsignePosition = position_actuelle+consigneVitesse;
   
 
  
   return prevConsignePosition; 
}

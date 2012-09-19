#ifndef CONSIGN_CONTROLLER
#define CONSIGN_CONTROLLER

#include <WProgram.h>
#include "Asserv_config.h"
#include "Filtre.h"
#include "Motors.h"
#include "Odometrie.h"

/*
 *    Regulateur : 
 *    
 *    Classe régulant un ordre de grandeur ( orientation ou distance )
 *      en faisant appel aux filtres.
 *       
 */


class Regulateur
{
   public:
     Regulateur()
     {
     }
  
     void setFiltres( QuadRampDerivee* filtre_consigne, Filtre* filtre_feedback, Filtre* filtre_erreur )
     {
         filtreConsigne = filtre_consigne; // QuadRamp
         filtreFeedback = filtre_feedback; // Accumulateur
         filtreErreur = filtre_erreur;     // PID 
     }
     long manage(long consigne, long feedback); 
     
     
     boolean isRampFinished()
     {
       return filtreConsigne->isRampFinished();
     }
     
   private:
     // Filtres utilisée 
     QuadRampDerivee *filtreConsigne;
     Filtre *filtreFeedback;
     Filtre *filtreErreur;
     
     //Valeurs retournées
     long consigneFiltree;
     long feedbackFiltree;
     long erreur, erreurFiltree;
  
};



/*
 *    ConsignController : 
 *    
 *    Classe appelant les régulateurs sur l'angle et la distance,
 *      puis calcul la puissance à demander aux moteurs
 *       
 *
 *    Cette classe fait le lien entre l'odométrie, les régulateurs d'angle/distance, et la carte moteur.
 */

class ConsignController
{
    public:
        ConsignController(Odometrie *odo, Motors *mot );
       
        void add_angle_consigne(long delta);
        void add_dist_consigne(long delta);       
         
        void perform();
        
        Odometrie* getOdometrie()
        {
          return odometrie;
        }
        
        
        /*
         * Méthodes pour arreter les régulateurs indépendamment ( par exemple, pour faire un calage bordure .. )
         */
        void angle_Regu_OnOff()
        {
            angle_regu_on = !angle_regu_on;
        }
        
        void dist_Regu_OnOff()
        {
            dist_regu_on = !dist_regu_on;
        }
        
        // Méthode permettant au CommandManager de savoir si la dernière consigne est finie ou non.
        boolean  areRampsFinished()
        {
          return angle_regu.isRampFinished() && dist_regu.isRampFinished();
        }

  
    private:
        //Moteurs
        Motors *motors;
        
        //Régu d'angle et de distance
        Regulateur angle_regu; boolean angle_regu_on;
        Regulateur dist_regu; boolean dist_regu_on;
        
        //Filtres associées
          QuadRampDerivee filtreConsigne_dist;
          Accumulateur filtreFeedback_dist;
          Pid filtreErreur_dist;
          
          QuadRampDerivee filtreConsigne_angle;
          Accumulateur filtreFeedback_angle;
          Pid filtreErreur_angle;
          
        // Odométrie
        Odometrie *odometrie;
        
        //Consignes courantes
        long angle_consigne;
        long dist_consigne;        
        
        
};



#endif

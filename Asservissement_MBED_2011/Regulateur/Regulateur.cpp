#include "Regulateur.h"


int64_t Regulateur::manage(int64_t consigne, int64_t feedback)
{
 
    feedbackFiltre = filtreFeedback.filtre(feedback); 

    
    
    
    consigneFiltree = filtreConsigne.filtre(consigne, feedbackFiltre, feedback); 
      
    if ( filtreConsigneON )
    {
        erreur = consigneFiltree - feedbackFiltre;
    }
    else
        erreur = consigne - feedbackFiltre;  
    
    erreurFiltree = filtreErreur.filtre(erreur, feedback); //PID Tommy
    
    
    //erreurFiltree = filtreErreur.filtre(erreur); //PID
    
    sortie = erreurFiltree;
    //sortie = filtreSortie.filtre(erreurFiltree, consigne, feedbackFiltre);
//     printf("erreur filtree = %d", erreurFiltree);
     //printf("erreur = %d sortie = %d \n", erreur,sortie );
     return sortie;
}
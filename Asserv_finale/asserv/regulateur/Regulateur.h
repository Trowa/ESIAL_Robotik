#ifndef REGULATEUR
#define REGULATEUR

#include "../filtres/Pid/Pid.h"
#include "../filtres/QuadRampDerivee/QuadRampDerivee.h"
#include "../config.h"

class Regulateur {
  public:
   
    // Constructeur et Destructeur
    /*
    * isDistance est un bool�en pass� en param�tre pour d�terminer si le r�gulateur s'occupe de la distance ou de l'angle
    * En fonction du cas, la QuadRampDerivee et le PID � cr�er ne sont pas les m�mes, on passe donc cet argument � leur constructeur pour
    * les diff�rencier
    */
    Regulateur(bool isDistance);
    ~Regulateur();
    
    // Permet de calculer l'erreur par rapport � la consigne gr�ce aux filtres et � la position fournit par l'odom�trie
    int64_t manage(int64_t consigne, int64_t feedback_odometrie); 
    
    // Permet d'activer ou de d�sactiver la QuadRampDerivee
    void setfiltreQuadRampDeriveeON(bool val) {
      filtreQuadRampDeriveeON = val;
    }
     
    // R�initialisation de l'accumulateur (distance en UO parcouru depuis l'origine)
    void reset_accumulator() {
      accumulateur = 0;
    }

    int64_t getAccumulateur() {
      return accumulateur;
    }

    // D�termine si la QuandRampDerivee est termin�e et donc si le robot � atteint sa cible
    bool isRampFinished() {
      return filtreQuadRampDerivee.isRampFinished();
    }

    // Permet de modifier dynamiquement la vitesse de la marche arri�re, pour le callage bordure notamment
    void setVitesseMarcheArriere(int64_t vitesse) {
      filtreQuadRampDerivee.setVitesseMarcheArriere(vitesse);
    }
     
  private:
    // Filtres utilises 
    QuadRampDerivee filtreQuadRampDerivee;
    Pid filtrePid;
    // Accumulateur des UO parcourues depuis le point de d�part
    int64_t accumulateur;
     
    // Permet de d�sactiver la QuadRampDerivee
    bool filtreQuadRampDeriveeON;
  	
	#ifdef DEBUG
  		bool isDistance; //permet de savoir si cette instance de r�gulateur est utilis�e pour la r�gulation de distance
  	#endif
	
};


#endif

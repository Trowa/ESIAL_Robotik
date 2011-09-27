#ifndef QUADRAMPDERIVEE
#define QUADRAMPDERIVEE

#include "../Filtre.h"
#include "../../Utils/Utils.h"

class QuadRampDerivee : public Filtre {
  public:
    
    // Constructeur avec un bool�en d�terminant si la QuadRamp concerne la distance ou l'angle
    QuadRampDerivee(bool isDistance);
    // Destructeur
    ~QuadRampDerivee();
    // Renvoie le bool�en arrivee qui passe � frais quand la rampe est termin�e
    bool isRampFinished() { return arrivee; }

    // Permet de changer la vitesse maximum de la marche arri�re, utile pour le callage bordure
    void setVitesseMarcheArriere(int64_t vitesse) {
      derivee_1ier_neg = vitesse;
    }

    // Filtre la consigne pour tenir compte des rampes d'acc�l�ration et de d�c�l�ration
    int64_t filtre(int64_t consigne, int64_t position_actuelle , int64_t vitesse);     
 
  private:
     
    // Derivees premieres de la consigne ( dans notre cas, la vitesse )
    int64_t derivee_1ier_pos; //En avant ( positif ) // Vient de config.h
    int64_t derivee_1ier_neg; // En arriere (negatif )  // Vient de config.h

    // Derivees secondes de la consigne ( dans notre cas, l'acceleration ) 
    int64_t derivee_2nd_pos_av; //En avant ( positif )  // Vient de config.h
    int64_t derivee_2nd_neg_av; // En arriere (negatif - deceleration) // Vient de config.h
     
    //la meme chose pour le deplacement vers l'arriere
    int64_t derivee_2nd_pos_ar; // Vient de config.h
    int64_t derivee_2nd_neg_ar; // Vient de config.h
     
    //On ajoute de l' "anticipation " pour eviter que la vitesse ne chute brutalement a zero quand on atteint le point d'arriv�e
    double gainAnticipation_av; // En avant // Vient de config.h
    double gainAnticipation_ar; // En arri�re // Vient de config.h

    // Sauvegarde de la vitesse precedente
    int64_t prevConsigneVitesse; // Stocke la vitesse de l'it�ration pr�c�dente
     
    // Taille de la fen�tre dans laquelle on se consid�re comme arriv� puisque la consigne exact est g�n�ralement innateignable
    int64_t tailleFenetreArrivee; // Vient de config.h
     
    // Permet de connaitre l'etat de la consigne actuelle ( on est arrivee ou non... )
    bool arrivee;

 };
 
 #endif

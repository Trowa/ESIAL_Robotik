#ifndef QUADRAMPDERIVEE
#define QUADRAMPDERIVEE

#include "../Filtre.h"
#include "../../Utils/Utils.h"

class QuadRampDerivee : public Filtre
 {
  public:
    
    QuadRampDerivee(bool isDistance);
    ~QuadRampDerivee();
    bool isRampFinished() { return arrivee; }

    void setVitesseMarcheArriere(int64_t vitesse) {
      derivee_1ier_neg = vitesse;
    }

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
     
    //On ajoute de l' "anticipation " pour eviter que la vitesse ne chute brutalement a zero quand le point d'arrivee arrive....( bien pourri ca dis donc -_-" )
    double gainAnticipation_av; // Vient de config.h
    double gainAnticipation_ar; // Vient de config.h

    // Sauvegarde de la vitesse precedente
    int64_t prevConsigneVitesse; // Stocke la vitesse de l'itération précédente
     
    //Pour eviter les vibrations a l'arrivee
    int64_t tailleFenetreArrivee; // Vient de config.h
     
    // Permet de connaitre l'etat de la consigne actuelle ( on est arrivee ou non... )
    bool arrivee;

 };
 
 #endif

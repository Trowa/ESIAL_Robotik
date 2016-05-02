#ifndef QUADRAMPDERIVEE
#define QUADRAMPDERIVEE

#include "../Filtre.h"
#include "../../Utils/Utils.h"
#include "../config/config.h"


class QuadRampDerivee : public Filtre
{
public:

    // Constructeur avec un booléen déterminant si la QuadRamp concerne la distance ou l'angle
    QuadRampDerivee(bool isDistance);
    // Destructeur
    ~QuadRampDerivee();
    // Renvoie le booléen arrivee qui passe à frais quand la rampe est terminée
    bool isRampFinished() {
        return arrivee;
    }

    // Permet de changer la vitesse maximum de la marche arrière, utile pour le calage bordure
    void setVitesseMarcheArriere(int64_t vitesse) {
        derivee_1ier_neg = vitesse;
    }

    void setVitesseMarcheAvant(int64_t vitesse) {
        derivee_1ier_pos = vitesse;
    }

    // Filtre la consigne pour tenir compte des rampes d'accélération et de décélération
    int64_t filtre(int64_t consigne, int64_t position_actuelle , int64_t vitesse);

private:

    // Dérivées premières de la consigne ( dans notre cas, la vitesse )
    int64_t derivee_1ier_pos; //En avant ( positif ) // Vient de config.h
    int64_t derivee_1ier_neg; // En arrière (negatif )  // Vient de config.h

    // Derivées secondes de la consigne ( dans notre cas, l'accélération )
    int64_t derivee_2nd_pos_av; //En avant ( positif )  // Vient de config.h
    int64_t derivee_2nd_neg_av; // En arrière (negatif - décélération) // Vient de config.h

    //la même chose pour le deplacement vers l'arrière
    int64_t derivee_2nd_pos_ar; // Vient de config.h
    int64_t derivee_2nd_neg_ar; // Vient de config.h

    //On ajoute de l' "anticipation " pour eviter que la vitesse ne chute brutalement à zéro quand on atteint le point d'arrivée
    double gainAnticipation_av; // En avant // Vient de config.h
    double gainAnticipation_ar; // En arrière // Vient de config.h

    // Sauvegarde de la vitesse précédente
    int64_t prevConsigneVitesse; // Stocke la vitesse de l'itération précédente

    // Taille de la fenêtre dans laquelle on se considère comme arrivé puisque la consigne exacte est généralement inatteignable
    int64_t tailleFenetreArrivee; // Vient de config.h

    // Permet de connaitre l'état de la consigne actuelle ( on est arrivée ou non... )
    bool arrivee;

#ifdef DEBUG
    bool isDistance; //permet de savoir si cette instance de la quadramp est utilisée pour la régulation de distance
#endif
};

#endif

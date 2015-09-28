#ifndef REGULATEUR
#define REGULATEUR

#include "../filtres/Pid/Pid.h"
#include "../filtres/QuadRampDerivee/QuadRampDerivee.h"
#include "../config/config.h"

class Regulateur
{
public:

    // Constructeur et Destructeur
    /*
    * isDistance est un booléen passé en paramètre pour déterminer si le régulateur s'occupe de la distance ou de l'angle
    * En fonction du cas, la QuadRampDerivee et le PID à créer ne sont pas les mêmes, on passe donc cet argument à leur constructeur pour
    * les différencier
    */
    Regulateur(bool isDistance);
    ~Regulateur();

    // Permet de calculer l'erreur par rapport à la consigne grâce aux filtres et à la position fournie par l'odométrie
    int64_t manage(int64_t consigne, int64_t feedback_odometrie);

    // Permet d'activer ou de désactiver la QuadRampDerivee
    void setfiltreQuadRampDeriveeON(bool val) {
        filtreQuadRampDeriveeON = val;
    }

    // Réinitialisation de l'accumulateur (distance en UO parcourue depuis l'origine)
    void reset_accumulator() {
        accumulateur = 0;
    }

    int64_t getAccumulateur() {
        return accumulateur;
    }

    // Détermine si la QuandRampDerivee est terminée et donc si le robot a atteint sa cible
    bool isRampFinished() {
        return filtreQuadRampDerivee.isRampFinished();
    }

    // Permet de modifier dynamiquement la vitesse de la marche arrière, pour le calage bordure notamment
    void setVitesseMarcheArriere(int64_t vitesse) {
        filtreQuadRampDerivee.setVitesseMarcheArriere(vitesse);
    }

private:
    // Filtres utilisés
    QuadRampDerivee filtreQuadRampDerivee;
    Pid filtrePid;
    // Accumulateur des UO parcourues depuis le point de départ
    int64_t accumulateur;

    // Permet de désactiver la QuadRampDerivee
    bool filtreQuadRampDeriveeON;

    bool isDistance; //permet de savoir si cette instance de régulateur est utilisée pour la régulation de distance
};


#endif

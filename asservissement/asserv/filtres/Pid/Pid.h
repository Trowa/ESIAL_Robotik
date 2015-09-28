#ifndef PID
#define PID

#include "../Filtre.h"
#include "../../Utils/Utils.h"

// PID(Proportionnel-Integrale-Derivee) - Filtre sur l'erreur

class Pid : public Filtre
{
public:
    // Constructeur avec un booléen pour le type de PID (distance ou angle)
    Pid(bool isDistance);
    // Destructeur
    ~Pid();

    // On filtre l'erreur pour déterminer comment la corriger
    int64_t filtre(int64_t erreur , int64_t feedback_odometrie , int64_t value3 = 0);

private:
    // Somme des erreurs
    int64_t integrale;

    //Coeffs d'asservissement
    int64_t kp, ki, kd; // Coeff venant de config.h

    //Ratios & Saturation
    /* Coeffs venant de config.h */
    double outRatio; // Ratio de sortie
    int64_t maxIntegral; // Valeur maximale de l'intégrale
    int64_t maxOutput; // Valeur maximale de la sortie

};

#endif

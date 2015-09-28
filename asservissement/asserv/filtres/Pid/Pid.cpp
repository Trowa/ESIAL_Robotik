#include "Pid.h"

// Constructeur
Pid::Pid(bool isDistance)
{
    // L'intégrale est nulle au départ
    integrale = 0;

    // En fonction du type de PID on ne récupère pas les même paramètres
    if (isDistance) {
        kp = Config::DIST_KP;
        ki = Config::DIST_KI;
        kd = Config::DIST_KD;
        outRatio = Config::DIST_OUT_RATIO;
        maxIntegral = Config::DIST_MAX_INTEGRAL;
        maxOutput = Config::DIST_MAX_OUTPUT;
    } else {
        kp = Config::ANGLE_KP;
        ki = Config::ANGLE_KI;
        kd = Config::ANGLE_KD;
        outRatio = Config::ANGLE_OUT_RATIO;
        maxIntegral = Config::ANGLE_MAX_INTEGRAL;
        maxOutput = Config::ANGLE_MAX_OUTPUT;
    }
}

// Destructeur
Pid::~Pid() {}

// On filtre l'erreur pour calculer la sortie à donner aux moteurs
int64_t Pid::filtre(int64_t erreur , int64_t feedback_odometrie , int64_t value3)
{
    // Calcul de la Proportionnelle
    int64_t P = erreur * kp;

    //printf("P=%d ", P);

    //Calcul de l'Intégrale que l'on oublie pas de borner
    integrale += erreur;
    integrale = Utils::constrain(integrale, -maxIntegral , maxIntegral);
    int64_t I = integrale * ki;


    // Calcul de la Dérivée
    int64_t D = feedback_odometrie * kd;

    //printf("D=%d ", D);

    //On calcul la Sortie qui est la somme des trois valeurs multipliée par le ratio de sortie
    int64_t sortie = P + I - D;
    sortie = sortie * outRatio;

    // Saturation de la sortie pour ne pas allez trop vite non plus
    sortie = Utils::constrain(sortie , -maxOutput , maxOutput);

    return sortie;
}

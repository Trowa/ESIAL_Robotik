#include "mbed.h"
#include "Odometrie.h"
#include "../codeurs/CodeursDirects.h"
//#include "../codeurs/CodeursAVR.h"
#include "../Utils/Utils.h"

#ifdef DEBUG
#include "../debug/DebugUDP.h"
extern DebugUDP *debugUdp;
#endif

// Constructeur
/*
* Lors de la création de l'objet, on calcul la distance entre les roues en UO et le nombre d'UO par front
* Les infos nécessaires au calcul sont dans config.h
*/
Odometrie::Odometrie()
{

    //Instanciation des codeurs
    codeurs = new CodeursDirects(p25, p26, p15, p16); //Avec des codeurs branchés directement sur la Mbed
    //codeurs = new CodeursAVR(p5, p6, p7, p8); //OU avec des codeurs branchés sur un AVR avec lequel on communique en SPI

    // Initialisation des compteurs
    compteurG = 0;
    compteurD = 0;

    // Initialisation de la position
    x = 0;
    y = 0;
    theta = 0;

    // Calcul de frontParMetre et de ratioCodeur
    if (Config::frontParMetreCodeurG != Config::frontParMetreCodeurD) {
        double min, max;

        if (Config::frontParMetreCodeurG > Config::frontParMetreCodeurD) {
            min = Config::frontParMetreCodeurD;
            max = Config::frontParMetreCodeurG;
            applyRatioOnG = false;
        } else {
            min = Config::frontParMetreCodeurG;
            max = Config::frontParMetreCodeurD;
            applyRatioOnG = true;
        }

        ratioCodeurs = max / min;
        frontParMetre = max;

    } else {
        ratioCodeurs = 1;
        frontParMetre = Config::frontParMetreCodeurD;
    }

    // Calcul de la distance entre les roues en UO
    distanceRouesUO = Config::distRoues * frontParMetre * Config::uOParFront / 1000;

}

// Destructeur
Odometrie::~Odometrie()
{
    delete codeurs;
}

void Odometrie::resetX(float xval) {
    x = Utils::mmToUO(this, xval);
}

void Odometrie::resetY(float yval) {
    y = Utils::mmToUO(this, yval);
}

void Odometrie::resetTheta() {
    theta = 0;
}       

// Mise à jour de la position du robot
void Odometrie::refresh()
{
    if (!Config::reglageCodeurs) { // Fonctionnement normal
        //Récupération des comptes des codeurs
        codeurs->getCounts(&compteurG, &compteurD);

        //printf("CG=%lld CD=%lld\n", compteurG, compteurD);
        //printf(compteurD==0 ? "***\n" : ".\n");

        //On transforme ces valeurs en Unites Odometrique
        compteurD = compteurD * Config::uOParFront;
        compteurG = compteurG * Config::uOParFront;

        // On applique le ratio pour prendre en compte la différence entre les codeurs
        if (applyRatioOnG) {
            compteurG = compteurG * ratioCodeurs;
        } else {
            compteurD = compteurD * ratioCodeurs;
        }
    } else { // Réglage des codeurs
        // Bout de code permettant de mesurer le nombre de tics codeurs par mètre.
        // Ce code ne sert qu'à ça, ne pas tester d'autre utilisation, il n'y en a pas.
        // Si ce code n'est PAS commenté, l'asservissement fera vraiment N'IMPORTE QUOI ! Et violemment ! Vous êtes prévenu !
        // En cas d'utilisation de ce bout de code dans son cadre prévu, NE PAS METTRE DE COURANT DANS LES MOTEURS !
        // Le robot va violemment osciller et ça risque d'abîmer le matos.
        // On peut l'éviter, alors on le fait, hein, ça évite de donner des cheveux blancs aux trésoriers...

        //Déclaration temporaire pour les comptes des codeurs
        int64_t tempCompteG;
        int64_t tempCompteD;
        //Récupération des comptes des codeurs
        codeurs->getCounts(&tempCompteG, &tempCompteD);

        //On rajoute les comptes récupérés aux totaux
        compteurD += tempCompteD;
        compteurG += tempCompteG;

        //renvoi des résultats sur la série
        printf("CG=%lld \tCD=%lld\n", compteurG, compteurD);
    }

    /*
    * deltaDist = la distance parcourue par le robot pendant l'itération = moyenne des distances des codeurs
    * deltaTheta = la variation de l'angle pendant l'itération = rapport de la différence des distances codeurs sur la
    *               distance entre les roues
    */
    deltaDist = (compteurG + compteurD) / 2; // En UO
    int64_t diffCount = compteurD - compteurG; // On conserve la différence entre les comptes en UO
    deltaTheta = (double)diffCount / (double)distanceRouesUO; // En radian

    if (llabs(diffCount) < 1) {   // On considère le mouvement comme une ligne droite
        // Mise à jour de la position
        x += deltaDist * cos(theta);
        y += deltaDist * sin(theta);
    } else { //On approxime en considérant que le robot suit un arc de cercle
        // On calcule le rayon de courbure du cercle
        double R = deltaDist / deltaTheta;
        //Mise à jour de la position
        x += R * (-sin(theta) + sin(theta + deltaTheta));
        y += R * (cos(theta) - cos(theta + deltaTheta));
        // Mise à jour du cap
        theta += deltaTheta;

        // On limite le cap à +/- PI afin de ne pouvoir tourner dans les deux sens et pas dans un seul
        if (theta > PI) {
            theta -= 2 * PI ;
        } else if (theta <= -PI) {
            theta += 2 * PI ;
        }
    }

    //Si le débug est en route
#ifdef DEBUG

    if (debugUdp->getDebugSend()) {
        /* on ajoute les valeurs et on les envoie */
        //uint64_t XMM =  this->getXmm();
        //uint64_t YMM =  this->getYmm();
        debugUdp->addData("X", (double) x);
        debugUdp->addData("Y", (double) y);
        debugUdp->addData("Theta", (double) theta);
        debugUdp->addData("deltaDist", (double) deltaDist);
        debugUdp->addData("deltaTheta", (double) deltaTheta);
        /*    udp_data_add("X",XMM);
                udp_data_add("Y", YMM);
                udp_data_add("T", this->getThetaDeg() );
                udp_data_add("Vl", deltaDist/(frontParMetre*Config::uOParFront)*1000 );
                udp_data_add("Va", deltaTheta*180/PI );
         */
    }

#endif


    //printf("x=%lld-y=%lld\n", x, y);
    /*
    printf("y=%d  ",y);
    printf("theta=%f\n",theta);
    */
}

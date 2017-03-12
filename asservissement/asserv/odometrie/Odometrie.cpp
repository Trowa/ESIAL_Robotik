#include "mbed.h"
#include "Odometrie.h"
#include "../codeurs/CodeursDirects.h"
//#include "../codeurs/CodeursAVR.h"
#include "../Utils/Utils.h"

#ifdef DEBUG_UDP
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
	//codeurs = new CodeursDirects(p25, p26, p15, p16); //Avec des codeurs branchés directement sur la Mbed
	//PMX
	codeurs = new CodeursDirects(p22, p21, p17, p18); //Avec des codeurs branchés directement sur la Mbed

	// Initialisation des compteurs
	compteurG = 0;
	compteurD = 0;

	// Initialisation de la position
	x = 0;
	y = 0;
	theta = 0;

	// Calcul de frontParMetre et de ratioCodeur
	if (Config::frontParMetreCodeurG != Config::frontParMetreCodeurD)
	{
		double min, max;

		if (Config::frontParMetreCodeurG > Config::frontParMetreCodeurD)
		{
			min = Config::frontParMetreCodeurD;
			max = Config::frontParMetreCodeurG;
			applyRatioOnG = false;
		}
		else
		{
			min = Config::frontParMetreCodeurG;
			max = Config::frontParMetreCodeurD;
			applyRatioOnG = true;
		}

		ratioCodeurs = max / min;
		frontParMetre = max;

	}
	else
	{
		ratioCodeurs = 1;
		frontParMetre = Config::frontParMetreCodeurD;
	}

	// Calcul de la distance entre les roues en UO
	distanceRouesUO = Config::distRoues * frontParMetre * Config::uOParFront
			/ 1000;

	//TODO a initiliser
	//deltaDist = 0;
	//deltaTheta = 0;

}

// Destructeur
Odometrie::~Odometrie()
{
	delete codeurs;
}

void Odometrie::resetX(float xval)
{
	x = Utils::mmToUO(this, xval);
}

void Odometrie::resetY(float yval)
{
	y = Utils::mmToUO(this, yval);
}

void Odometrie::resetTheta()
{
	theta = 0;
}

// Mise à jour de la position du robot
void Odometrie::refresh()
{
	//Récupération des comptes des codeurs
	codeurs->getCounts(&compteurG, &compteurD);

	if (Config::reglageCodeurs)
	{
		printf("CG=%ld \t\tCD=%ld\r\n", compteurG, compteurD);
	}
	//On transforme ces valeurs en Unites Odometrique
	compteurD = compteurD * Config::uOParFront;
	compteurG = compteurG * Config::uOParFront;

	// On applique le ratio pour prendre en compte la différence entre les codeurs
	if (applyRatioOnG)
	{
		compteurG = compteurG * ratioCodeurs;
	}
	else
	{
		compteurD = compteurD * ratioCodeurs;
	}

	/*
	 * deltaDist = la distance parcourue par le robot pendant l'itération = moyenne des distances des codeurs
	 * deltaTheta = la variation de l'angle pendant l'itération = rapport de la différence des distances codeurs sur la
	 *               distance entre les roues
	 */
	deltaDist = (compteurG + compteurD) / 2; // En UO
	int32_t diffCount = compteurD - compteurG; // On conserve la différence entre les comptes en UO
	deltaTheta = (double) diffCount / (double) distanceRouesUO; // En radian

	if (llabs(diffCount) < 1)
	{   // On considère le mouvement comme une ligne droite
		// Mise à jour de la position
		x += deltaDist * cos(theta);
		y += deltaDist * sin(theta);
	}
	else
	{ //On approxime en considérant que le robot suit un arc de cercle
		// On calcule le rayon de courbure du cercle
		double R = deltaDist / deltaTheta;
		//Mise à jour de la position
		x += R * (-sin(theta) + sin(theta + deltaTheta));
		y += R * (cos(theta) - cos(theta + deltaTheta));
		// Mise à jour du cap
		theta += deltaTheta;

		// On limite le cap à +/- PI afin de ne pouvoir tourner dans les deux sens et pas dans un seul
		if (theta > PI)
		{
			theta -= 2 * PI;
		}
		else if (theta <= -PI)
		{
			theta += 2 * PI;
		}
	}

	//Si le débug est en route
#ifdef DEBUG_UDP

	if (debugUdp->getDebugSend())
	{
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

}

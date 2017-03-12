#include "Regulateur.h"

#ifdef DEBUG_UDP
#include "../debug/DebugUDP.h"
extern DebugUDP *debugUdp;
#endif

// Constructeur
Regulateur::Regulateur(bool isDistance) :
		filtreQuadRampDerivee(isDistance), filtrePid(isDistance)
{
	filtreQuadRampDeriveeON = true;
	accumulateur = 0;
	this->isDistance = isDistance;
}

// Destructeur
Regulateur::~Regulateur()
{
}

// Calcul de l'erreur
int64_t Regulateur::manage(int64_t consigne, int64_t feedback_odometrie)
{

	// On ajoute dans l'accumulateur la variation de déplacement mesuré par l'odométrie
	accumulateur += feedback_odometrie;

	// On filtre la consigne à l'aide de la QuadRampeDerivee pour savoir si l'on est en phase d'accélération, constante ou de décélération
	// Cette étape permet d'avoir un comportement linéaire au lieu de binaire (= soit à l'arrêt, soit à fond) qui provoque des secouses et peut
	// renverser le robot. A la place, on accélère et déccélère tranquillement et ça, c'est beau :p
	int64_t consigneFiltree = filtreQuadRampDerivee.filtre(consigne,
			accumulateur, feedback_odometrie);

	// On calcul l'erreur, c'est à dire la différence entre la consigne à suivre et la position réelle stockée dans l'accumulateur
	int64_t erreur;
	bool enabled = filtreQuadRampDeriveeON;

	if (isDistance)
		enabled &= !Config::disableDistanceQuad;
	else
		enabled &= !Config::disableAngleQuad;

	if (enabled)
	{
		erreur = consigneFiltree - accumulateur;
	}
	else
	{
		erreur = consigne - accumulateur;
	}

	// On envoie l'erreur au PID qui va la filtrer par rapport aux paramètres du robot et aux coefficients d'asservissement
	// On obtient alors la vitesse à envoyer aux moteurs pour corriger l'erreur courante
	int64_t erreurFiltree = filtrePid.filtre(erreur, feedback_odometrie);

	//débug
#ifdef DEBUG_UDP

	char name[32];

	strcpy(name, "consigne");
	name[8] = (isDistance ? 'D' : 'A');
	name[9] = 0;
	debugUdp->addData(name, (double) consigne);

	strcpy(name, "accu");
	name[4] = (isDistance ? 'D' : 'A');
	name[5] = 0;
	debugUdp->addData(name, (double) accumulateur);

	strcpy(name, "consigneFiltree");
	name[15] = (isDistance ? 'D' : 'A');
	name[16] = 0;
	debugUdp->addData(name, (double) consigneFiltree);

	strcpy(name, "erreurFiltree");
	name[13] = (isDistance ? 'D' : 'A');
	name[14] = 0;
	debugUdp->addData(name, (double) erreurFiltree);

#endif

	return erreurFiltree;
}

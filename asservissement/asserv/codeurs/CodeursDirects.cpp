#include "CodeursDirects.h"

CodeursDirects::CodeursDirects(PinName pinChanA_G, PinName pinChanB_G,
		PinName pinChanA_D, PinName pinChanB_D) :
		codeurG(pinChanA_G, pinChanB_G), codeurD(pinChanA_D, pinChanB_D)
{
	// On change la priorité de l'interruption timer pour qu'elle soit plus basse que celle des ticks codeurs
	NVIC_SetPriority(TIMER3_IRQn, 1);
	lastCountD_ = 0;
	lastCountG_ = 0;
}

CodeursDirects::~CodeursDirects()
{
}

void CodeursDirects::getCounts(int32_t* countG, int32_t* countD)
{

#ifdef CODEUR_AVEC_RESET
	__disable_irq(); //On désactive les interruptions
	*countD = codeurD.getCount();//On récupère le compte des codeurs
	*countG = codeurG.getCount();
	codeurD.reset();//On reset les comptes
	codeurG.reset();
	__enable_irq();//on réactive les interruptions
#endif
#ifndef CODEUR_AVEC_RESET
	int32_t tempD_ = codeurD.getCount();
	*countD = tempD_ - lastCountD_; //On récupère le compte des codeurs
	lastCountD_ = tempD_;

	int32_t tempG_ = codeurG.getCount();
	*countG = tempG_ - lastCountG_;
	lastCountG_ = tempG_;

#endif

	// On ajuste le sens des codeurs en cas de problème de cablage
	if (Config::swapCodeurs)
	{ //inversion des deux codeurs
		int32_t temp = *countG;
		*countG = *countD;
		*countD = temp;
	}

	if (Config::inverseCodeurD)
	{ //Changement de sens de rotation du codeur droit
		*countD = -(*countD);
	}

	if (Config::inverseCodeurG)
	{ //Changement de sens de rotation du codeur gauche
		*countG = -(*countG);
	}

}

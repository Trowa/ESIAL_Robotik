#include "CodeursDirects.h"

CodeursDirects::CodeursDirects(PinName pinChanA_G, PinName pinChanB_G, PinName pinChanA_D, PinName pinChanB_D) :
	codeurG(pinChanA_G , pinChanB_G ),
	codeurD(pinChanA_D , pinChanB_D )
{	
	// On change la priorit� de l'interruption timer pour quelle soit plus basse que celle des ticks codeurs
  	NVIC_SetPriority(TIMER3_IRQn, 1);
}

CodeursDirects::~CodeursDirects() { }

void CodeursDirects::getCounts(int64_t* countG, int64_t* countD) {
	__disable_irq(); //On d�sactive les interruptions
  	*countD = codeurD.getCount(); //On r�cup�re le compte des codeurs
  	*countG = codeurG.getCount();
  	codeurD.reset(); //On reset les comptes
  	codeurG.reset();
  	__enable_irq(); //on r�active les interruptions
	
	// On ajuste le sens des codeurs en cas de probl�me de cablage
  	if(swapCodeurs) { //inversion des deux codeurs
	    int64_t temp = *countG;
	    *countG = *countD;
	    *countD = temp;
  	}
	
	if(inverseCodeurD) { //Changement de sens de rotation du codeur droit
    	*countD = -(*countD);
  	}
  	if(inverseCodeurG) { //Changement de sens de rotation du codeur gauche
    	*countG = -(*countG);
  	}
	
}

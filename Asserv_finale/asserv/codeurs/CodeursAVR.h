#ifndef CODEUR_AVR
#define CODEUR_AVR

#include "mbed.h"
#include "../config.h"
#include "CodeursInterface.h"
#include "Codeur.h"

//Gestion des codeurs branch�s sur un AVR
//L'AVR se charge de compter les tics (et donc des interruptions)
//La Mbed commuique en SPI pour r�cup�rer les comptes des codeurs
/*
class CodeursAVR : public CodeursInterface {

	public:
		CodeursAVR(PinName mosi, PinName miso, PinName sck, PinName ss);
		~CodeursAVR();
		virtual void getCounts(int64_t* countG, int64_t* countD);
	
	private:
		SPI spiToAVR;
		DigitalOut slaveSelect;
};
*/
#endif

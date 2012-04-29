#include "CodeursAVR.h"

CodeursAVR::CodeursAVR(PinName mosi, PinName miso, PinName sck, PinName ss) :
	spiToAVR(mosi, miso, sck),
	slaveSelect(ss)
{
	slaveSelect = 1;
	spiToAVR.format(8,3);
	spiToAVR.frequency(1000000);
}

CodeursAVR::~CodeursAVR() { }

void CodeursAVR::getCounts(int64_t* countG, int64_t* countD) {
	slaveSelect = 0; //On passe le slave select � 0, ce qui d�clenche le gestionnaire du SPI sur l'AVR
	int8_t answerG = spiToAVR.write(0x00); //On r�cup�re le compte gauche
	wait_us(8); //On attend un peu, que l'AVR ait le temps de charger le registre avec le compte codeur droit
	int8_t answerD = spiToAVR.write(0x00); //On r�cup�re le compte droit
	slaveSelect = 1; //On reset le Slave Select
	
	 //On r�gle d'�ventuels probl�mes de signe
	//answerG = (answerG>128?answerG-256:answerG);
    //answerD = (answerD>128?answerD-256:answerD);
	
	//Et on affecte
	*countG = answerG;
	*countD = answerD;
	
	//Debug
	//pc.printf("Cg:%lld - Cd:%lld\n", *countG, *countD);
	
}

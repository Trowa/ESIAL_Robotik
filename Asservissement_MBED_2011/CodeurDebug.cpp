/*

#include "Codeur.h"
#include "Accumulateur.h"

Codeur codeurG(PinChanACoderG , PinChanBCoderG );
Codeur codeurD(PinChanACoderD , PinChanBCoderD );

Serial pc(USBTX, USBRX); // tx, rx

Accumulateur countDAccu , countGAccu;
int64_t countG , countD;

Ticker infos;

void dispInfos() {
    pc.printf("countG = %d\ncountD = %d\n", countG , countD);
}



int main()
{

    infos.attach(&dispInfos , 1.0);
    
    while(1)
    {
        wait(0.05);
        countG = countGAccu.filtre(codeurG.getCount());
        countD = countDAccu.filtre(-codeurD.getCount());
        codeurG.reset();
        codeurD.reset();
    }
}

*/
/*

#include "Codeur.h"
#include "Accumulateur.h"


Codeur codeurG(PinChanACoderG , PinChanBCoderG );
Codeur codeurD(PinChanACoderD , PinChanBCoderD );

Serial pc(USBTX, USBRX); // tx, rx

//Accumulateur countDAccu , countGAccu;
int64_t countG , countD, deltaDist , dist=0;
double deltaTheta, theta=0 , diffCount;

double distanceRouesFront = DIST_ROUES * FRONT_PAR_METRE;
double distanceRouesUO = distanceRouesFront * UO_PAR_FRONT;

Accumulateur distAccu;

Ticker infos;

void dispInfos() {
    pc.printf("\n\ncountG = %d\ncountD = %d\ndistance = %d\nangle = %f\n", countG , countD , dist , theta);
}



int main()
{

    //infos.attach(&dispInfos , 1.0);
    
    while(1)
    {
        wait(0.5);
//        countG = countGAccu.filtre(codeurG.getCount());
//        countD = countDAccu.filtre(-codeurD.getCount());
        countG = codeurG.getCount();
        countD = -codeurD.getCount();
        codeurG.reset();
        codeurD.reset();
        
        
        
        countD = countD * UO_PAR_FRONT;
        countG = countG * UO_PAR_FRONT;
        
        
        
        deltaDist = (countG + countD)/2; // En UO
        
        
   
        diffCount = countD - countG; // On conserve la difference entre les compte en UO
        deltaTheta = diffCount / distanceRouesUO; // En radian
        
        pc.printf("deltaDist= %d\ diffCount= %lf deltaTheta= %lf\n" , deltaDist , diffCount , deltaTheta);
        
        dist = distAccu.filtre(deltaDist);
        theta += deltaTheta;
    
    }
}

*/
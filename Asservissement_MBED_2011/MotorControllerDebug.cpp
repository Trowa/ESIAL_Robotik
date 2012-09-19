/*

#include "Qik.h"
#include "Md22.h"

Md22 motorController(p28 , p27 );

Serial pc(USBTX, USBRX);


int main()
{

    //motorController.vitesseG(127);

    while(true)
    {
        
        pc.printf("deceleration moteur gauche\n");
        for(int vitG = -128 ; vitG<120 ; vitG+= 8)
        {
            motorController.vitesseG(vitG);
            wait(0.1);
        }
        
        pc.printf("deceleration moteur droit\n");
        for(int vitD = -128 ; vitD<120 ; vitD+= 8)
        {
            motorController.vitesseD(vitD);
            wait(0.1);
        }
        
        pc.printf("acceleration moteur gauche\n");
        for(int vitG = 127 ; vitG>-120 ; vitG-= 8)
        {
            motorController.vitesseG(vitG);
            wait(0.1);
        }
        
        pc.printf("acceleration moteur droit\n");
        for(int vitD = 127 ; vitD>-120 ; vitD-= 8)
        {
            motorController.vitesseD(vitD);
            wait(0.1);
        }
     
    }
    
    
    
}

*/
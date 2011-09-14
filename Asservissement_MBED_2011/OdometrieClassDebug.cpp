/*

#include "Odometrie.h"

Odometrie odo(p9,p10,p23,p24);

//Serial pc(USBTX, USBRX); // tx, rx

int main()
{
    odo.setFrontParMetre(5092.96);
    odo.setDistRouesMetre(0.117); 
    odo.setUOParFront(256);
    
    while(1)
    {
        
        odo.refresh();
        
//        pc.printf("nx = %d , y = %d , theta = %lf\n" , odo.getXmm() , odo.getYmm() , odo.getThetaDeg() );
        printf("nx = %d , y = %d , theta = %lf\n" , odo.getXmm() , odo.getYmm() , odo.getThetaDeg() );
        
        wait(0.5);
        
    }

}

*/
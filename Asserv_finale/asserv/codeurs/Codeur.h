#ifndef CODEUR
#define CODEUR

#include "mbed.h"
#include "../config.h"

//Un codeur branché directement sur la Mbed

class Codeur
{

    public:
    
        Codeur(PinName pinChanA , PinName pinChanB );
        int getCount();
        void reset();
                
    private:
    
        void handleInterruptA();
        void handleInterruptB();
    
        volatile int count;
        InterruptIn pinChannelA;
        InterruptIn pinChannelB;
        
       
};

#endif

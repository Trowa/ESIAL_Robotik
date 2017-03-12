#ifndef CODEUR
#define CODEUR

#include "mbed.h"
#include "../config/config.h"

//Un codeur branché directement sur la Mbed

class Codeur
{

public:

    Codeur(PinName pinChanA , PinName pinChanB);
    ~Codeur();
    long getCount();
    void reset();

private:

    void handleInterruptA();
    void handleInterruptB();

    volatile long count;
    InterruptIn pinChannelA;
    InterruptIn pinChannelB;


};

#endif

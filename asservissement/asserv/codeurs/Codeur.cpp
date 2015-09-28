#include "Codeur.h"

Codeur::Codeur(PinName pinChanA , PinName pinChanB)

    : pinChannelA(pinChanA) , pinChannelB(pinChanB)//debug

{
    count = 0;

    pinChannelA.rise(this, &Codeur::handleInterruptA);
    pinChannelA.fall(this, &Codeur::handleInterruptA);
    pinChannelB.rise(this, &Codeur::handleInterruptB);
    pinChannelB.fall(this, &Codeur::handleInterruptB);
}

Codeur::~Codeur() { }

void Codeur::reset(void)
{
    count = 0;
}


int Codeur::getCount(void)
{
    return count;
}


void Codeur::handleInterruptA(void)
{

    if (pinChannelA) {
        if (pinChannelB) {
            count++;
        } else {
            count--;
        }
    } else {
        if (pinChannelB) {
            count--;
        } else {
            count++;
        }
    }
}

void Codeur::handleInterruptB(void)
{

    if (pinChannelB) {
        if (pinChannelA) {
            count--;
        } else {
            count++;
        }
    } else {
        if (pinChannelA) {
            count++;
        } else {
            count--;
        }
    }
}

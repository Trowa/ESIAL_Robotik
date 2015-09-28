#ifndef CODEUR_DIRECT
#define CODEUR_DIRECT

#include "mbed.h"
#include "../config/config.h"
#include "CodeursInterface.h"
#include "Codeur.h"

//Deux codeurs, branchés directement sur la Mbed

class CodeursDirects : public CodeursInterface
{

public:
    CodeursDirects(PinName pinChanA_G, PinName pinChanB_G, PinName pinChanA_D, PinName pinChanB_D);
    ~CodeursDirects();
    virtual void getCounts(int64_t* countG, int64_t* countD);

private:
    Codeur codeurG;
    Codeur codeurD;

};

#endif

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
    virtual void getCounts(int32_t* countG, int32_t* countD);

private:
    Codeur codeurG;
    Codeur codeurD;

    int32_t lastCountD_;
    int32_t lastCountG_;

};

#endif

#ifndef CODEURS_INTERFACE
#define CODEURS_INTERFACE

#include "mbed.h"

//Interface pour la gestion de deux codeurs
class CodeursInterface
{

public:
    virtual ~CodeursInterface() { }
    virtual void getCounts(int32_t* countG, int32_t* countD) = 0;

};

#endif

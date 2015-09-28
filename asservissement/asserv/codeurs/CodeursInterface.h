#ifndef CODEURS_INTERFACE
#define CODEURS_INTERFACE

#include "mbed.h"

//Interface pour la gestion de deux codeurs
class CodeursInterface
{

public:
    virtual ~CodeursInterface() { }
    virtual void getCounts(int64_t* countG, int64_t* countD) = 0;

};

#endif

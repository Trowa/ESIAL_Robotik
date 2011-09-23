#ifndef FILTRE
#define FILTRE

#include "mbed.h"
#include "../config.h"


// "interface" representant les filtres

class Filtre
{
  public:
    int64_t filtre(int64_t value1 , int64_t value2 = 0 , int64_t value3 = 0);
};

#endif

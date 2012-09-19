#ifndef UTILS
#define UTILS
#include "../mbed.h"
class Utils
{
    public :
    //Limitation d'une valeur a un intervalle [valeurMin , valeurMax]
    int64_t static constrain(int64_t value , int64_t min , int64_t max );
    
};

#endif
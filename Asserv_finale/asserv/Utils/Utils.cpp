#include "Utils.h"

//Limitation d'une valeur a un intervalle [valeurMin , valeurMax]
int64_t Utils::constrain(int64_t value , int64_t valeurMin , int64_t valeurMax)
{
    if(value < valeurMin)
        return valeurMin;
    
    if(value > valeurMax)
        return valeurMax;
        
    return value;
}

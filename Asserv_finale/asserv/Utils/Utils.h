#ifndef UTILS
#define UTILS

#include "mbed.h"
#include "../odometrie/Odometrie.h"

class Utils
{
  public :
    
    //Limitation d'une valeur a un intervalle [valeurMin , valeurMax]
    int64_t static constrain(int64_t value , int64_t min , int64_t max );

    // Conversions
    int64_t static mmToUO(Odometrie *odo, int64_t valeur);
    int64_t static degToUO(Odometrie *odo, int64_t valeur);
    int64_t static radToUO(Odometrie *odo, int64_t valeur);
    
};

#endif

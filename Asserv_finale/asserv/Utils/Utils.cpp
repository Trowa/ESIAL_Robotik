#include "Utils.h"

//Limitation d'une valeur a un intervalle [valeurMin , valeurMax]
int64_t Utils::constrain(int64_t value , int64_t valeurMin , int64_t valeurMax) {
    if(value < valeurMin)
        return valeurMin;
    
    if(value > valeurMax)
        return valeurMax;
        
    return value;
}

// Conversion mm vers UO
int64_t Utils::mmToUO(Odometrie *odo, int64_t valeur) {
  return valeur/1000*(odo->getFrontParMetre())*uOParFront;
}

// Conversion degres en UO
int64_t Utils::degToUO(Odometrie *odo, int64_t valeur) {
  return (PI * valeur/180) * odo->getDistanceRouesUO();
}

// Conversion radians en UO
int64_t Utils::radToUO(Odometrie *odo, double valeur) {
  return valeur * odo->getDistanceRouesUO();
}

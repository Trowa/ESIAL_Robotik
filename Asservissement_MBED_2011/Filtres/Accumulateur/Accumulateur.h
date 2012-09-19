#ifndef ACCUMULATEUR
#define ACCUMULATEUR

#include "../Filtre.h"

// Accumulateur - Filtre sur le feedback

class Accumulateur : public Filtre
 {
   public:
     Accumulateur(); 
 
     int64_t filtre(int64_t value1 , int64_t value2 = 0 , int64_t value3 = 0);
     
     void reset();
 
   private:
     float total;
 };
 
 #endif
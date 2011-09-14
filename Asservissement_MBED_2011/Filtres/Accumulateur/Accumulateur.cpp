#include "Accumulateur.h"

Accumulateur::Accumulateur()
{
    total = 0;
}


void Accumulateur::reset()
{
    total = 0;
//    printf("RESET Accu\n");
}

int64_t Accumulateur::filtre(int64_t value1 , int64_t value2 , int64_t value3 )
{
    total += value1;
    return total;
}

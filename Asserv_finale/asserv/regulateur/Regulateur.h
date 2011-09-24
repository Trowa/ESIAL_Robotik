#ifndef REGULATEUR
#define REGULATEUR

#include "../filtres/Pid/Pid.h"
#include "../filtres/QuadRampDerivee/QuadRampDerivee.h"

class Regulateur
{
   public:
   
    Regulateur(bool isDistance);
    ~Regulateur();
     
    int64_t manage(int64_t consigne, int64_t feedback_odometrie); 
     
    void setfiltreQuadRampDeriveeON(bool val) {
      filtreQuadRampDeriveeON = val;
    }
     
    void reset_accumulator() {
      accumulateur = 0;
    }

    int64_t getAccumulateur() {
      return accumulateur;
    }

    bool isRampFinished() {
      return filtreQuadRampDerivee.isRampFinished();
    }
     
  private:
    // Filtres utilises 
    QuadRampDerivee filtreQuadRampDerivee;
    Pid filtrePid;
    int64_t accumulateur;
     
    // Permet de désactiver la QuadRampDerivee
    bool filtreQuadRampDeriveeON;
  
};


#endif

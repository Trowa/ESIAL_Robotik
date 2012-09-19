#ifndef ODOMETRIE
#define ODOMETRIE

#include <WProgram.h>
#include "Odometrie_config.h"

class Coder
{
  public:
    Coder(char PinChanA, char pinChanB , char interruptPinNumber );
     
     
     	inline void inc(){count++;}
	inline void dec(){count--;}
	inline void reset(){ count=0; }
	inline long get_count(){return count;}

   private:
	volatile char count;
        char PinChannelA;
        char PinChannelB;
  
        void calcul_dep();  
};



class Odometrie
{
  public:
    Odometrie();
    
    // Mise à jour ...
    void majOdometrie( );
    
    //Position en unitée arbritraires ( Unitée Odométrie [UO] pour les distances, et rad pour l'angle)
    inline long getX(){ return x; }
    inline long getY(){ return y; }
    inline float getTheta(){ return theta; }
    
    // Position en unitée 'humaine'
    inline long getXmm(){ return x/(FRONT_PAR_METRE*UO_PAR_FRONT)*1000; }
    inline long getYmm(){ return y/(FRONT_PAR_METRE*UO_PAR_FRONT)*1000; }
    inline float getThetaDeg(){ return theta*180/PI; }
    
    
    // Variation de la position depuis la dernière mise à jour
    inline long getDeltaThetaBrut(){ return diffCount; }
    inline float getDeltaThetaDeg(){ return deltaTheta; }
    inline float getDeltaDist(){ return deltaDist; }
    
    
    /*
     * Calcul de position et/ou d'angle pratiques
     */
     
    long degToUO(float deg)
    {
       return (PI * deg/180) * distanceRouesUO;
    }
    
    long mmToUO(float mm)
    {
      return mm/1000*FRONT_PAR_METRE*UO_PAR_FRONT;
    }
    
    
    
    //Pour debug..
    void printInfos();
    
  private:
    float distanceRouesFront;
    float distanceRouesUO;
    float distanceRouesMetre;
    
    
    // Position actuelle 
    long x, y; //En UO
    float theta; //En radian
    
    // Variation pour la mise à jour en cours
    float deltaDist; // En UO
    long diffCount; // EN Brut
    float deltaTheta; //En radian
};



#endif

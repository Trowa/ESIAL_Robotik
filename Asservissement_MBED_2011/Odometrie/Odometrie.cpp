#include "../mbed.h"
#include "Odometrie.h"

//#define DEBUG
#ifdef DEBUG
    #include "../UDPData.h"
#endif
extern Serial pc;

Odometrie::Odometrie(PinName PinChanACoderG , PinName PinChanBCoderG , PinName PinChanACoderD , PinName PinChanBCoderD)

        : codeurG(PinChanACoderG , PinChanBCoderG) ,
        codeurD(PinChanACoderD , PinChanBCoderD)

{

    countG = 0;
    countD = 0;

    x = 0;
    y = 0;
    theta = 0;
    
    inverseCodeurG = false;
    inverseCodeurD = false;
    swapCodeurs = false;
    
    ratioCodeurs = 1;
    applyRatioOnG = false;
    
}

void Odometrie::refresh() {
    //On commence par recuperer le compte des codeurs, et a les reseters en desactivant les interruptions pendant ce temps
    __disable_irq();
    countG = codeurG.getCount();
    countD = - codeurD.getCount(); //On met un signe - pour l'un des codeurs puisqu'ils ne sont pas dans le meme sens dans le robot
    
   codeurG.reset();
   codeurD.reset();
    __enable_irq();
    
   
    if(swapCodeurs) { //inversion des deux codeurs
        int64_t temp = countG;
        countG = countD;
        countD = temp;
    }
    
    if(inverseCodeurD) { //Changement de sens de rotation du codeur droit
        countD = -countD;
    }
    
    if(inverseCodeurG) { //Changement de sens de rotation du codeur gauche
        countG = -countG;
    }
    
 // printf("cG = %d , cD = %d \r\n" ,countG , countD);
    
    //On transforme ces valeurs en Unites Odometrique
    countD = countD * uOParFront;
    countG = countG * uOParFront;
    
    
    if(applyRatioOnG) {
        countG = countG * ratioCodeurs;
    } else {
        countD = countD * ratioCodeurs;
    }
    
    /* Calcul de deltaDist la distance parcourue par le robot
    * et de deltaTheta la variation de l'angle pendant l'iteration
    */
    deltaDist = (countG + countD)/2; // En UO

    diffCount = countD - countG; // On conserve la difference entre les comptes en UO
    deltaTheta = diffCount / distanceRouesUO; // En radian

    //printf("delD = %ld\tdelT = %ld\n" , deltaDist , deltaTheta );

    if ( abs(diffCount) < 1 ) { // On considere le mouvement comme une ligne droite
        // Mise a jour de la position
        x += deltaDist * cos(theta);
        y += deltaDist * sin(theta);
    } else { //On approxime en considerant que le robot suit un arc de cercle
        // On calcule le rayon de courbure du cercle
        R = deltaDist/deltaTheta;
        //Mise a jour de la position
        x += R * (-sin(theta) + sin(theta + deltaTheta) );
        y += R * (cos(theta) - cos(theta + deltaTheta) );

        // Mise a jour du cap
        theta += deltaTheta;


        // On limite le cap a +/- PI
        if ( theta > PI )
            theta -= 2 * PI ;
        else if ( theta <= -PI )
            theta += 2 * PI ;
    }
  
  
  #ifdef DEBUG 
   /* on ajoute les valeurs et on les envoies */
        int XMM =  this->getXmm();
        int YMM =  this->getYmm();
       udp_data_add("X",XMM);
       udp_data_add("Y", YMM);
       udp_data_add("T", this->getThetaDeg() );
       udp_data_add("Vl", deltaDist/(frontParMetre*uOParFront)*1000 );
       udp_data_add("Va", deltaTheta*180/PI );
    #endif
    
    //printf("x=%d,y=%d\n", XMM, YMM);
    
}



// parametres physiques des codeurs
void Odometrie::setFrontParMetre(double codeurGFPM, double codeurDFPM) {
    if(codeurGFPM != codeurDFPM) {
        double min,max;
        if(codeurGFPM>codeurDFPM) {
            min = codeurDFPM;
            max = codeurGFPM;
            applyRatioOnG = false;
        } else {
            min = codeurGFPM;
            max = codeurDFPM;
            applyRatioOnG = true;
        }
        
        ratioCodeurs = max/min;
        frontParMetre = max;
       
    
    } else {
        ratioCodeurs = 1;
        frontParMetre = codeurDFPM;
    }
    //frontParMetre = newFrontParMetre;
    distanceRouesFront = distanceRouesMetre * frontParMetre;
    distanceRouesUO = distanceRouesFront * uOParFront;
}

// distance par rapport au centre
void Odometrie::setDistRouesMetre(double distRoues) {
    distanceRouesMetre = distRoues;
    distanceRouesFront = distanceRouesMetre * frontParMetre;
    distanceRouesUO = distanceRouesFront * uOParFront;
}

void Odometrie::setUOParFront(double newUOParFront) {
    uOParFront = newUOParFront;
    distanceRouesUO = distanceRouesFront * uOParFront;
}




// debug - recuperation du compte des codeurs en UO
/*
int64_t Odometrie::getCountG(void) {
    return countG;
}

int64_t Odometrie::getCountD(void) {
    return countD;
}
*/
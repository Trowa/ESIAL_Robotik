#include "Rampe.h"

        
int64_t Rampe::filtre(int64_t erreurFiltree, int64_t consigne, int64_t feedbackFiltre) {
    
    int64_t distance = abs(consigne - feedbackFiltre);
    char sens = (consigne - feedbackFiltre)>0;
        
    if( (consigne - feedbackFiltre)>0) {
        if(distance > minDecelerationDistForward && distance < maxDecelerationDistForward) {
            
            erreurFiltree *= (1 -  decelerationCoeffForward * (maxDecelerationDistForward - distance)/(maxDecelerationDistForward - minDecelerationDistForward) );
            if(abs(erreurFiltree) < minDecelerationSpeedForward)
                erreurFiltree = ( sens ? minDecelerationSpeedForward : -minDecelerationSpeedForward);
        }
    
    } else {
        if(distance > minDecelerationDistBackward && distance < maxDecelerationDistBackward) {
            erreurFiltree *= (1 - decelerationCoeffBackward * (maxDecelerationDistBackward - distance)/(maxDecelerationDistBackward - minDecelerationDistBackward) );
            if(abs(erreurFiltree) < minDecelerationSpeedBackward)
                erreurFiltree = ( sens ? minDecelerationSpeedBackward : -minDecelerationSpeedBackward);
        }
    }   
    
    
    if(erreurFiltree > prevValue) {
        if(erreurFiltree > (prevValue + maxStepForward) ) {
            prevValue += maxStepForward;
            return (int64_t) prevValue;
        } else {
            prevValue = (double) erreurFiltree;
            return erreurFiltree;
        }
    } else {
        if(erreurFiltree < (prevValue - maxStepBackward) ) {
            prevValue -= maxStepBackward;
            return (int64_t) prevValue;
        } else {
            prevValue = (double )erreurFiltree;
            return erreurFiltree;
        }
    }
    
}
       
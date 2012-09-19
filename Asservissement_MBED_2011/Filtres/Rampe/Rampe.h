#ifndef RAMPE
#define RAMPE

#include "../Filtre.h"

class Rampe : public Filtre
{
    public:
   
        Rampe()
        {
            prevValue = 0;
        }
        
        void setMaxStepForward(double newStepForward) { maxStepForward = newStepForward; }
        void setMaxStepBackward(double newStepBackward) { maxStepBackward = newStepBackward; }
        
        void setDecelerationCoeffForward(double newCoeff) { decelerationCoeffForward = newCoeff; }
        void setDecelerationCoeffBackward(double newCoeff) { decelerationCoeffBackward = newCoeff; }
         
        void setMinDecelerationDistForward(int64_t newDist) { minDecelerationDistForward = newDist; }
        void setMaxDecelerationDistForward(int64_t newDist) { maxDecelerationDistForward = newDist; }
        void setMinDecelerationDistBackward(int64_t newDist) { minDecelerationDistBackward = newDist; }
        void setMaxDecelerationDistBackward(int64_t newDist) { maxDecelerationDistBackward = newDist; }
        
        void setMinDecelerationSpeedForward(int64_t newMinSpeed) { minDecelerationSpeedForward = newMinSpeed; }
        void setMinDecelerationSpeedBackward(int64_t newMinSpeed) { minDecelerationSpeedBackward = newMinSpeed; }
        
        
        double getMaxStepForward() { return maxStepForward; }
        double getMaxStepBackward() { return maxStepBackward; }
        
        double getDecelerationCoeffForward() { return decelerationCoeffForward; }
        double getDecelerationCoeffBackward() { return decelerationCoeffBackward; }
         
        int64_t getMinDecelerationDistForward() { return minDecelerationDistForward; }
        int64_t getMaxDecelerationDistForward() { return maxDecelerationDistForward; }
        int64_t getMinDecelerationDistBackward() { return minDecelerationDistBackward; }
        int64_t getMaxDecelerationDistBackward() { return maxDecelerationDistBackward; }
        
        int64_t getMinDecelerationSpeedForward() { return minDecelerationSpeedForward; }
        int64_t getMinDecelerationSpeedBackward() { return minDecelerationSpeedBackward; }
        
        int64_t filtre(int64_t erreurFiltree, int64_t consigne, int64_t feedbackFiltree);
        
    private:
    
        double prevValue;
        double maxStepForward;
        double maxStepBackward;
        
        double decelerationCoeffForward;
        double decelerationCoeffBackward;
        
        int64_t minDecelerationDistForward;
        int64_t maxDecelerationDistForward;
        int64_t minDecelerationDistBackward;
        int64_t maxDecelerationDistBackward;
        
        int64_t minDecelerationSpeedForward;
        int64_t minDecelerationSpeedBackward;
        
};

#endif
#ifndef COMMAND_MANAGER
#define COMMAND_MANAGER

#include "mbed.h"
#include "../ConsignController/ConsignController.h"
#include "CMDList/CMDList.h"

class CommandManager {

    public:
        CommandManager(int capacity , ConsignController *ctrlr);
  
        bool addStraightLine(int64_t valueInmm);
        bool addTurn(int64_t angleInDeg);
        bool addArcCircle(int64_t angleInDeg, int64_t radiusInmm);
        bool addGoTo(int64_t posXInmm, int64_t posYInmm);
        bool addGoToAngle(int64_t posXInmm, int64_t posYInmm);
        void perform();
        
        void setGoToAngleThreshold( float newAngleThreshold ) { angleThreshold = newAngleThreshold; }
        void setGoToDistThreshold( int newDistThreshold ) { distThreshold = newDistThreshold; }
        void setGoToReturnThreshold( float newReturnThreshold ) { returnThreshold = newReturnThreshold; }
        
        //Gestion d'un &#65533;ventuel arret d'urgence
        void setEmergencyStop();
        void resetEmergencyStop();
        
        void computeGoTo();
        void computeGoToAngle();
        
    private:
        CMDList liste; //File d'attente des commandes
        ConsignController *cnsgCtrl;
        CMD currCMD; //commande courante
        CMD nextCMD; //commande suivante
        
        // pour les consignes GOTO :
        float angleThreshold; // si la difference angleCible - angleActuel est inferieure a cette valeur, le robot avance
        float distThreshold; // si la distance du robot au point d'arrivee prevu est inferieure a cette valeur,
                              // on passe a la consigne GOTO suivante, s'il y en a une
        
        float returnThreshold; // mm;,  distance au dela de laquelle le robot fait un 180 pour aller a la position cible dans un GOTO
        
        bool emergencyStop; //arret d'urgence : si le robot adverse est dans les parages
        
        bool currentConsignFinished; //signale si la consigne courante est terminee. Reste a vrai tant qu'une nouvelle consigne n'est pas arrivee.
};

#endif
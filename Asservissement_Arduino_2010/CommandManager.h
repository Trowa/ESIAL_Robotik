#ifndef COMMAND_MANAGER
#define COMMAND_MANAGER

#include <WProgram.h>
#include "ConsigController.h"

    // Internal Structures
    enum typeCMD
    {
        CMD_NULL, // Erreur....
    	CMD_GO,
    	CMD_TURN,
    	CMD_WAIT,
    	CMD_STOP,
        CMD_CIRCLE,
        CMD_GOTO
    };
    
    typedef struct CMD_struct
    {
    	typeCMD type;
    	long value;
        long secValue;
    } CMD;
    
    
    //A conveniant class to manage a command list
    class CMDList
    {
      public:
        CMDList(int capacity );
        boolean enqueue(typeCMD cmd , long val, long val2);
        CMD dequeue();
        int size();
        
    
       private:
        CMD *list;
        int capa;
        int nextFreePos;
        int headPos;
        bool full;
    };




/*
 *    CommandManager : 
 *    
 *    Classe de haut niveau permettant à l'utilisateur de donner une liste de mouvement à effectuer 
 *      qui s'enchaineront les unes à la suite des autres...
 *       
 */


class CommandManager
{
   public:
     CommandManager(CMDList *lst , ConsignController *ctrlr)
     {
       liste = lst;
       cnsgCtrl = ctrlr;
     }
  
     boolean addStraightLine(int valueInmm);
     boolean addTurn(float angleInDeg);
     boolean addArcCircle(float angleInDeg, float radiusInmm);
     void perform();
     
   private:
     CMDList *liste;
     ConsignController *cnsgCtrl;
     CMD currCMD;  
};


#endif

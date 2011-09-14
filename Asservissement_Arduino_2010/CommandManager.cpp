#include "CommandManager.h"


//--------------------------------------------------------
//                       COMMAND MANAGER
//--------------------------------------------------------


/*
 * Fonctions de "commandes"  externes, tout droit, tourner, arc de cercle..
 */

boolean CommandManager::addStraightLine(int valueInmm)
{
   return liste->enqueue( CMD_GO , cnsgCtrl->getOdometrie()->mmToUO(valueInmm), 0 );
}

boolean CommandManager::addTurn(float angleInDeg)
{
  return liste->enqueue( CMD_TURN , cnsgCtrl->getOdometrie()->degToUO(angleInDeg) , 0);
}

boolean CommandManager::addArcCircle(float angleInDeg, float radiusInmm)
{
   return  liste->enqueue( CMD_CIRCLE , angleInDeg ,radiusInmm ); // cnsgCtrl->getOdometrie()->degToUO(angleInDeg) ,  cnsgCtrl->getOdometrie()->mmToUO(radiusInmm) );
}




/*
 * Fonction de mise à jour...
 */
void CommandManager::perform()
{
  if ( !cnsgCtrl->areRampsFinished()  ) //la consigne courante n'est pas encore finie...
  {
      if (currCMD.type == CMD_GO || currCMD.type == CMD_TURN ) 
          return; //Dans ce cas, on attend simplement d'être arrivé :)
     
      else if ( currCMD.type == CMD_CIRCLE )
      {
          cnsgCtrl->add_angle_consigne( cnsgCtrl->getOdometrie()->mmToUO(1) );
          cnsgCtrl->add_dist_consigne(  (PI*1/180) * cnsgCtrl->getOdometrie()->mmToUO( currCMD.secValue )  );
      }
      
      else if ( currCMD.type == CMD_GOTO )
      {
        
      }
  }
  else 
  {
      currCMD = liste->dequeue(); // On essaye de récuperer la prochaine consigne
      if( currCMD.type == CMD_NULL  )
      {
          return; // Il n'y en a pas...
      } 
        
      if( currCMD.type == CMD_GO  )
      {
          cnsgCtrl->add_dist_consigne(currCMD.value);
      }
      else if ( currCMD.type == CMD_TURN )
      {
          cnsgCtrl->add_angle_consigne(currCMD.value);
      }
      else if ( currCMD.type == CMD_CIRCLE )
      {
          cnsgCtrl->add_angle_consigne( cnsgCtrl->getOdometrie()->mmToUO(1) );
          cnsgCtrl->add_dist_consigne(  (PI*1/180) * cnsgCtrl->getOdometrie()->mmToUO( currCMD.secValue )  );
      }   
  }
}



//--------------------------------------------------------
//                       CMDLIST
//--------------------------------------------------------

CMDList::CMDList(int capacity )
{
    list = (CMD*) malloc( sizeof(CMD) * capacity );
    nextFreePos=0;
    headPos = 0;
    full = false;
    capa = capacity;
}

boolean CMDList::enqueue(typeCMD cmd , long val, long val2)
{
    if( full )
       return false;
    else
   {
      list[nextFreePos].type = cmd;
      list[nextFreePos].value = val;
      list[nextFreePos].secValue = val2;
      nextFreePos = (nextFreePos + 1 ) % capa;
      
      if ( nextFreePos == headPos )
        full = true;
        
      return true;
   } 
}


CMD CMDList::dequeue()
{
   CMD res;
   res.type = CMD_NULL;
  
   if (!full && nextFreePos == headPos )  
     return res; // List if empty !
   
   if( nextFreePos == headPos )
     full = false;
     
   int returnIndex = headPos;  
   headPos = (headPos + 1 ) % capa;
   
   res.type = list[returnIndex].type;
   res.value = list[returnIndex].value;
   return res;
}

int CMDList::size()
{
  if( full)
    return capa;

  return nextFreePos - headPos;
}

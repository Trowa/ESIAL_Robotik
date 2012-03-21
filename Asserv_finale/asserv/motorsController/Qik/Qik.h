#ifndef QIK2S9V1
#define QIK

#include "Qik_config.h"
#include "../MotorsController.h"

// Adress definition
#define VitDposAdress 0x88
#define VitDnegAdress 0x8A
#define VitGposAdress 0x8C
#define VitGnegAdress 0x8E


// instance of a Qik2s9v1
class Qik : public MotorsController
{

    public :
        Qik(PinName txPinToQik, PinName rxPinToQik);
        
        virtual void vitesseG(int vitMoteurG);
        virtual void vitesseD(int vitMoteurD);
        
    private :
    
        Serial qikSerial;
        
};

#endif



#include "Qik.h"

Qik::Qik(PinName txPinToQik, PinName rxPinToQik)

:qikSerial(txPinToQik,rxPinToQik)

{
    qikSerial.baud(38400);
    qikSerial.format( 8 , Serial::None , 1 );
    
    // qikSerial.putc(0xAA); //if baud rate jumper is not set
    
    this->vitesseD(0);
    this->vitesseG(0);
    
    
}


void Qik::vitesseD(int vitMoteurD)
{
    if(inverseMoteurD)
        vitMoteurD=-vitMoteurD;
         
    if(vitMoteurD>V_MAX_POS_MOTOR)
        vitMoteurD=V_MAX_POS_MOTOR;
    if(vitMoteurD<V_MAX_NEG_MOTOR)
        vitMoteurD=V_MAX_NEG_MOTOR;
        
    if(vitMoteurD>=0)
    {
        if(!swapMoteurs) {
            qikSerial.putc(VitDposAdress);
        } else {
            qikSerial.putc(VitGposAdress);
        }
        qikSerial.putc(vitMoteurD);
    }
    else
    {
        if(!swapMoteurs) {
            qikSerial.putc(VitDnegAdress);
        } else {
            qikSerial.putc(VitGnegAdress);
        }
        qikSerial.putc(-vitMoteurD);
    }   
}


void Qik::vitesseG(int vitMoteurG)
{
    if(inverseMoteurG)
        vitMoteurG=-vitMoteurG;
        
    if(vitMoteurG>V_MAX_POS_MOTOR)
        vitMoteurG=V_MAX_POS_MOTOR;
    if(vitMoteurG<V_MAX_NEG_MOTOR)
        vitMoteurG=V_MAX_NEG_MOTOR;
 
    if(vitMoteurG>=0)
    {
        if(!swapMoteurs) {
            qikSerial.putc(VitGposAdress);
        } else {
            qikSerial.putc(VitDposAdress);
        }
        qikSerial.putc(vitMoteurG);
    }
    else
    {
        if(!swapMoteurs) {
            qikSerial.putc(VitGnegAdress);
        } else {
            qikSerial.putc(VitDnegAdress);
        }
        qikSerial.putc(-vitMoteurG);
    }
}



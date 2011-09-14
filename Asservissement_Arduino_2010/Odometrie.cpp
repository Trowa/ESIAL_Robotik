#include "Odometrie.h"
#include "Codeurs_config.h"

Coder::Coder(char PinChanA, char pinChanB , char interruptPinNumber )
{

    count = 0;
    
    PinChannelA = PinChanA;
    PinChannelB = pinChanB;
    
    pinMode(PinChanA , INPUT); 
    pinMode(pinChanB, INPUT); 
    
    // On allume la résistance interne de ces pins
    //   voir http://www.arduino.cc/en/Tutorial/DigitalPins
    digitalWrite(PinChanA, HIGH);
    digitalWrite(pinChanB, HIGH); 
}

Odometrie::Odometrie()
{
  attachInterrupt(interruptNumCoderD, do_coderD, CHANGE);
  attachInterrupt(interruptNumCoderG, do_coderG, CHANGE);
  
  
  distanceRouesFront = DIST_ROUES * FRONT_PAR_METRE;
  distanceRouesUO = distanceRouesFront * UO_PAR_FRONT;
 
  x = 0;
  y = 0;
  theta = 0;
}

void Odometrie::printInfos()
{
  Serial.print("countD = ");
  Serial.print(coderD.get_count());
  Serial.print(" countG = ");  
  Serial.println(coderG.get_count());
}


void Odometrie::majOdometrie()
{
  //On commence par recuperer le compte des codeurs, et à les reseters en désactivant les interruptions pendant ce temps
  cli();
  long countD = coderD.get_count();
  long countG = coderG.get_count();
  coderD.reset();
  coderG.reset();
  sei();
  
  //On transforme ces valeurs en Unitée Odométrique
  countD = countD * UO_PAR_FRONT;
  countG = countG * UO_PAR_FRONT;
  
  /* Calcul de deltaDist la distance parcourue par le robot
   * et de deltaTheta la variation de l'angle pendant l'itération
   */
   deltaDist = (countG + countD)/2; // En UO
   
   diffCount = countD - countG; // On conserve la difference entre les compte en UO
   deltaTheta = diffCount / distanceRouesUO; // En radian
   

  
  if ( abs(diffCount) < 2 ) // On considère le mouvement comme une ligne droite 
  {
    // Mise à jour de la position 
    x += deltaDist * cos(theta);
    y += deltaDist * sin(theta);
  
  } 
  else  //On approxime en considèrant que le robot suit une courbe
  {
    
    // On calcule le rayon de courbure du cercle
    float R = deltaDist/deltaTheta;

   //Mise à jour de la position
   x += R * (-sin(theta) + sin(theta + deltaTheta) );
   y += R * (cos(theta) - cos(theta + deltaTheta) );
  
   
   // Mise à jour du cap
   theta += deltaTheta;
  
 
  
   // On limite le cap à +/- PI
    if ( theta > PI )
      theta -= TWO_PI ;
    else if ( theta <= -PI )
      theta += TWO_PI ;
  }
}


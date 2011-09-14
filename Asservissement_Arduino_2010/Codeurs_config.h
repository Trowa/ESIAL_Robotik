#ifndef CODEURS_CONFIG
#define CODEURS_CONFIG

#include <WProgram.h>

/*
 * CODEURS - Configuration
 *
 * Permet de changer les pins associées au intéruptions
 * Ainsi que les fonctions liées à ces inté ruptions.
 */


// Pins des codeurs droite et gauche
#define PinChanACoderG 2
#define PinChanBCoderG 4

#define PinChanACoderD 3
#define PinChanBCoderD 5

// Numéro des pins où les interruptions se passent
  // Sur arduino  -> 0 Correspond à la pin 2, 1 à la pin 3
#define interruptNumCoderD 1
#define interruptNumCoderG 0


// On cree les instances des codeurs droites et gauches
Coder coderD(PinChanACoderD , PinChanBCoderD,interruptNumCoderD );
Coder coderG(PinChanACoderG , PinChanBCoderG,interruptNumCoderG );

/* 
 *  On créée les fonctions pour gerer les intérruptions qui vont bien
 *  Qui seront attachées à l'init de l'odométrie
 */
void do_coderD()
{
  if (digitalRead(PinChanACoderD) == HIGH) 
  {
      if (digitalRead(PinChanBCoderD) == LOW) 
      {
        coderD.dec();
      } 
      else 
      {
        coderD.inc();
      }
  }
  else
  { 
      if (digitalRead(PinChanBCoderD) == LOW) 
      {
        coderD.inc();
      } 
      else 
      {
          coderD.dec();
      }
  }
}

void do_coderG()
{
  if (digitalRead(PinChanACoderG) == HIGH) 
  {
      if (digitalRead(PinChanBCoderG) == LOW) 
      {
         coderG.inc();

      } 
      else 
      {
        coderG.dec();       
      }
  }
  else
  { 
      if (digitalRead(PinChanBCoderG) == LOW) 
      {
          coderG.dec();
      } 
      else 
      {
        coderG.inc();        

      }
  }
}

#endif

#include "Motors.h"
#include "ConsigController.h"
#include "CommandManager.h"
#include <Wire.h>
#include <CompactQik2s9v1.h>
#include <NewSoftSerial.h>



// En 1ier lieu d'odométrie, brique de base du système
Odometrie odometrie;

// Connection avec les moteurs
#define rxPin 9
#define txPin 8
#define rstPin 10

NewSoftSerial qikSerial =  NewSoftSerial(rxPin, txPin);
CompactQik2s9v1 motor = CompactQik2s9v1(&qikSerial,rstPin);
Motors motors(&motor);


ConsignController consignController(&odometrie, &motors);

// Liste de commandes pour le gestionnaire de commande ( ex: tout droit, tourner, arc de cercle... )
CMDList  lst(5);
CommandManager cmdManager(&lst, &consignController);


// Système pour diviser la frequence de la boucle, dans notre cas ça permet de lancer le commandManager une fois sur 'diviseurFrequBoucle' par rapport à la frequence d'asservissement
int diviseurFrequBoucle = 10;
int compteurDiviseur = diviseurFrequBoucle;


unsigned char control_timer_begining;
#define TIMER2_CLOCK_FREQ 15625.0 //Fréquence à laquelle tourne l'horloge du timer 2 ( TROUVER d'OU VIENT STE VAL )


void init_timer(float frequency)
{
  //Calculate the timer load value
  control_timer_begining  = (int)((256.0-(TIMER2_CLOCK_FREQ/frequency))+0.5);
  
  // Timer2 setup
  TCCR2A = 0;         // normal mode
  TCCR2B = 1<<CS22 | 1<<CS21 | 1<<CS20;  // 1 1 1 sets the prescaler to /1024, cf ATmega168 datasheet
}


void control_start()
{
  // Timer2 overflow interrupt enable;
  TIMSK2 = 1<<TOIE2;
  //load the timer for its first cycle
  TCNT2 = control_timer_begining; 
  
}


ISR(TIMER2_OVF_vect)
{
  TCNT2 = control_timer_begining;   // reload timer
  sei(); // Réactivation des interruptions pour que les fronts des codeurs ne soient pas perdus.
  
 
 // On commence par mettre à jour l'Odométrie, puis on fait les calculs d'asservissement
  odometrie.majOdometrie();
 
  consignController.perform();
  /*
 //Ensuite tout les __diviseurFrequBoucle__ on fait les calculs au niveau des commandes de haut niveau.
   if( --compteurDiviseur == 0 ) 
   {
     compteurDiviseur = diviseurFrequBoucle;
     cmdManager.perform();
   } 
   */
}





void setup() 
{
  Serial.begin(115200); 
  
  // Commandes supplémentaires à effectuer pour initialiser la carte qik
  qikSerial.begin(9600);
  motor.begin();
  motor.stopBothMotors();
  motors.init();
  init_timer(200);
  control_start();
}

void loop() 
{
  if ( Serial.available() > 0 )
  {
    char var = Serial.read();
    
    if (var == 'g' )
    {
   //cmdManager.addArcCircle(90,100);
  //cmdManager.addTurn(180);
   cmdManager.addStraightLine(25);
 // float pos = 0.3 * FRONT_PAR_METRE * UO_PAR_FRONT;
   //  consignController.add_dist_consigne(pos);
    }
    if (var == 'G' )
    {
          cmdManager.addTurn(1);
       cmdManager.addStraightLine( (PI*1/180) * 100);
    }
    if( var == 'p')
    {
      
      Serial.print("X= ");
      Serial.print(odometrie.getX() ); Serial.print(" UO - "); Serial.print(odometrie.getXmm() );
      Serial.print(" mm  \nY= ");
      Serial.print(odometrie.getY() ); Serial.print(" UO - "); Serial.print(odometrie.getYmm() );
      Serial.print(" mm  \nthetaDeg= ");
      Serial.print(odometrie.getTheta(), 5 ); Serial.print(" rad - "); Serial.print(odometrie.getThetaDeg(), 2 );
      Serial.println(" deg\n");
      odometrie.printInfos();
    }
     if( var == 'P')
    {
      odometrie.printInfos();
    }
  }
}

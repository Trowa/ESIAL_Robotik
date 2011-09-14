#include "mbed.h"
#include "MotorsController/Md22/Md22.h"
#include "CommandManager/CommandManager.h"


//#define DEBUG

#ifdef DEBUG
#include "UDPData.h"
#endif

// Ticker pour refresh asserv
Ticker Live;
void Live_isr(void);

// Debug tool
Serial pc(USBTX, USBRX); 
Timer t;
Timer temps;

// Asservissement
Md22 motors( p9 , p10);
Odometrie odometrie(p25,p26,p15,p16);
ConsignController consignController(&odometrie, &motors);
CommandManager cmdManager(30 , &consignController );

//Communication avec l'IA
//Serial iaCom( p28, p27);

//char divBoucleCmdManager;



/* fonction de callback */
void Live_isr() 
{   
    odometrie.refresh();

    //if(--divBoucleCmdManager>0)
        cmdManager.perform();
    //else
      //  divBoucleCmdManager=5;
        
    consignController.perform();
    
    #ifdef DEBUG
        udp_data_add("t", temps.read_ms());
    #endif
}



DigitalOut myled(LED1);
Ticker flipper;


void stop_match()
{
    consignController.angle_Regu_Off();
    consignController.dist_Regu_Off();
    
    pc.printf(" TIMER FINI ! \n");
}

void stop()
{
     cmdManager.setEmergencyStop();
}

void unstop()
{
     cmdManager.resetEmergencyStop();
}


// Tirette
DigitalIn tiretteIN(p27);
DigitalOut tiretteOUT(p28);


// Couleur
DigitalIn couleurIN(p29);
DigitalOut couleureOUT(p30);

// Interruptions capteurs
DigitalIn capteurs(p7);


int checkCapteurs()
{
    for( int i=0; i<5; i++)
    {
        if( capteurs.read() == 0 )
            return 0;
        wait(0.005);
    }
    return 1;
}




int main() 
{ 
    NVIC_SetPriority(TIMER3_IRQn, 1);
    pc.baud(115200);
    pc.printf("GOGO\n");
    
    tiretteOUT.write(1);
    couleureOUT.write(1);

    
    
    //iaCom.baud(9600);
    /*
    while(1)
    {
            if(iaCom.readable()) 
            {
        
                char c = 0;
                c = iaCom.getc();
                pc.printf("%d\n", c);
                iaCom.printf("%d\n", c+1);
       
                
          
                char c = iaCom.getc();
                
                
                switch(c) {
                
                    case 'c' :
                        int sens = 0;
                        iaCom.scanf("%d", &sens);
                        consignController.calage_bordure(sens);
                        printf("fin calage bordure\n");
                        iaCom.printf("y\n");
                        break;
                        
            
                    case 'o' :
                        printf("Match commence !\n");
                        break;
                    
            
                }
                
            }
    }
    */
    
    // init UDPData
    
    #ifdef DEBUG
        udp_data_init(&consignController, &odometrie);
        udp_data_load(); // chargement des coeffs a partir du fichier coeffs.conf
    #endif
    
    temps.start();
    /*
    *   Mesures empiriques sur 1 metre :
    
        cG = 25653 , cD = 25697
        
        cG = 25686 , cD = 25710
        
        cG = 25658 , cD = 25682
 
        cG = 25645 , cD = 25734
        
        cG = 25664 , cD = 25685
        
        cG = 25660 , cD = 25717
        
        cG = 25669 , cD = 25698
        
        cG = 25665 , cD = 25724
        
        cG = 25653 , cD = 25687
        
        cG = 25645 , cD = 25734
        
        cG = 25667 , cD = 25700
 
    */
    //odometrie.setFrontParMetre(25464.8);
    odometrie.setFrontParMetre(25661.444, 25703.778); 
    odometrie.setDistRouesMetre(0.284);
    odometrie.setUOParFront(512);
    motors.inverseMoteurGauche();
    motors.inverseMoteurDroit();
    odometrie.inverseCodeurDroit();
    odometrie.inverseCodeurGauche();
    
    cmdManager.setGoToAngleThreshold(0.05);
    cmdManager.setGoToDistThreshold(50);
    cmdManager.setGoToReturnThreshold(50); // mm

   //consignController.angle_Regu_Off();
  //consignController.dist_Regu_Off();
  
    
    
    Live.attach(Live_isr, 0.005);
    
    
    // On attend que la tirette sorte
    while( tiretteIN.read() == 0 ){ pc.printf("Attente fin tirette  \n"); }
    
    int sens = couleurIN.read();
    int mult = sens==0 ? 1 : -1;
    
    consignController.calage_bordure(sens);
    
    // On attend qu'on remette la tirette, puis qu'elle vire
    while( tiretteIN.read() == 1 ) { pc.printf("Attente tirette  \n"); }
    while( tiretteIN.read() == 0 ) { pc.printf("Attente fin tirette  \n"); }
    
    
    //On met en marche le ticker de 90 secondes pour le match
    flipper.attach(&stop_match, 88);
    
    // On lance le  parcours
    
    cmdManager.addGoTo(1046, mult * 140 );
    pc.printf("Goto1 \n");
    wait(0.05);
    while( !consignController.areRampsFinished() )
    {
        if ( checkCapteurs() )
        {
            stop();
            pc.printf("Stop \n");
            while( checkCapteurs() );
            
            unstop();
            pc.printf("UNStop \n");
            cmdManager.addGoTo(1046, mult * 140 );
            wait(0.05);
        }
    }
    
    
    
    cmdManager.addGoTo(1396, mult * 996 );
    pc.printf("Goto2 \n");
    wait(0.05);
    while( !consignController.areRampsFinished() )
    {
        if ( checkCapteurs() )
        {
            stop();
            pc.printf("Stop \n");
            while( checkCapteurs() );
            
            unstop();
             pc.printf("UNStop \n");
            cmdManager.addGoTo(1396, mult * 996 );
            wait(0.05);
        }
    }
    
    
    /*
    cmdManager.addGoToAngle(346, mult * 2046 );
    wait(50);
    while( !consignController.areRampsFinished() )
    {
        if ( capteurs.read() == 1 )
        {
            stop();
            while( capteurs.read() == 1 );
            
            unstop();
            cmdManager.addGoToAngle(346, mult * 2046 );
        }
    }
    
    
    
    
    cmdManager.addStraightLine(-200);
    wait(50);
     while( !consignController.areRampsFinished() )
    {
        if ( capteurs.read() == 1 )
        {
            stop();
            while( capteurs.read() == 1 );
            
            unstop();
            cmdManager.addStraightLine(-200);
        }
    }
    */
    
    
    
    cmdManager.addGoTo(1221, mult * 1621 );
    pc.printf("Goto3 \n");
    wait(0.05);
     while( !consignController.areRampsFinished() )
    {
        if ( checkCapteurs() )
        {
            stop();
            pc.printf("Stop \n");
            while( checkCapteurs() );
            
            unstop();
             pc.printf("UNStop \n");
            cmdManager.addGoTo(1221, mult * 1621 );
            wait(0.05);
        }
    }
    
    while(1);
    
    
    
    /*
    while( 1 )
    {
        int value0 = tiretteIN.read();
        int value = couleurIN.read();
        pc.printf("%d %d \n", value0, value);
    }
    */
    
    
    /*
    
    int dist;
    int angle;
    int x;
    int y;  
    int sens = 0;
    
    
    bool stop = false;
    bool go = true;
    while(go) 
    {
    
        // Tant que le signal est � bas on attend...
        pc.printf("En attente de lecture \n");
        while ( !bitSignal)
        {
            ;
        }
        
        //Le signal passe � haut, on lit une fois !
        
        int cmd = readCmd();
        pc.printf("Lecture de %d \n", cmd);
        
        
        
        switch(cmd) 
        {
        
            case 0 :
                go = false;
                flipper.attach(&stop_match, 88);
               // cmdManager.addGoTo(1046, mult * 140 );
            break;
            
            case 1 :
                //cmdManager.addGoTo(1396, mult * 996 );
            break;
            
            case 2 :
                //cmdManager.addGoToAngle(346, mult * 2046 );
            break;
            
            case 3 :
                //cmdManager.addStraightLine(-200);
            break;
            
            case 4 :
                //cmdManager.addGoTo(696, mult * 140 );
            break;
            
            case 5 :
            / *
                if( !stop ) 
                {
                    pc.printf("\tSTOP \n");
                    cmdManager.setEmergencyStop();
                    stop = true;
                }
                else
                {
                    pc.printf("\tReset_STOP \n");
                    cmdManager.resetEmergencyStop();
                    stop = false;
                }* /
            break;
    
         / *  case 6 :
                pc.printf("Reset_STOP \n");
                cmdManager.resetEmergencyStop();
            break;
        * /
        
            case 6 :
                
                sens = 0;
                printf("Debut callage sens : %d \n", sens);
                consignController.calage_bordure(sens);
                printf("fin calage bordure\n");
                
            break;
            
            case 7 :
                
                sens = 1;
                printf("Debut callage sens : %d \n", sens);
                consignController.calage_bordure(sens);
                printf("fin calage bordure\n");
                
            break;
        }
        
           // pc.printf("\tAttente que le bit de lecture passe  bas \n");
            // On attend que le bit de lecture soit � bas pour boucler
            //pc.printf("\tOn boucle ! \n");
            myled = !myled;    
    }
    
    
    while(1)
    {
        int mult = sens==0 ? 1 : -1;
        cmdManager.addGoTo(1046, mult * 140 );
        wait(50);
        while( !consignController.areRampsFinished() )
        {
            ;
        }
        
        cmdManager.addGoTo(1396, mult * 996 );
        wait(50);
        while( !consignController.areRampsFinished() )
        {
            ;
        }
        
        cmdManager.addGoToAngle(346, mult * 2046 );
        wait(50);
        while( !consignController.areRampsFinished() )
        {
            ;
        }
        
        cmdManager.addStraightLine(-200);
        wait(50);
        while( !consignController.areRampsFinished() )
        {
            ;
        }
        
        cmdManager.addGoTo(696, mult * 140 );
        wait(50);
        while( !consignController.areRampsFinished() )
        {
            ;
        }
    }
    
     / *   
        if(pc.readable()>0) 
        {
            char c = pc.getc();
            
            if(c=='z') 
            {
                pc.printf("Avance de 500\n");
                consignController.add_dist_consigne(odometrie.mmToUO(100));
            }
            else if(c=='Z') 
            {
                pc.printf("Avance de 500\n");
                consignController.add_dist_consigne(odometrie.mmToUO(1000));
            }
             else if(c=='q') 
            {
                pc.printf("Tourne de %d\n", odometrie.degToUO(90));
                consignController.add_angle_consigne(odometrie.degToUO(90));
            }
            else if(c=='s') 
            {
                pc.printf("Avance de 500\n");
                consignController.add_dist_consigne(odometrie.mmToUO(-100));
            } else if(c=='S') 
            {
                pc.printf("Avance de 500\n");
                consignController.add_dist_consigne(odometrie.mmToUO(-1000));
            }
             else if(c=='d') 
            {
                pc.printf("Tourne de %d\n", odometrie.degToUO(-45));
                consignController.add_angle_consigne(odometrie.degToUO(-45));
            }
             else if(c=='p') 
            {
                pc.printf("X=%lld  Y=%lld  Cap=%f \n", odometrie.getXmm() , odometrie.getYmm(), odometrie.getTheta() );
            }
            else if(c=='e')
            {
                cmdManager.setEmergencyStop();
            }
            else if(c=='r')
            {
                cmdManager.resetEmergencyStop();
            }
            else if(c=='c')
            {
                printf("Callage\n");
                int sens = 0;
                pc.scanf("%d", &sens);
                printf("Sens : %d\n", sens);
                consignController.calage_bordure(sens);
            }
            
            
            
            else if(c=='l')
            {
                 cmdManager.addGoTo(0, 0);
            }
            else if(c=='i')
            {
                 cmdManager.addGoTo(900, 900);
            }
            else if(c=='k')
            {
                 cmdManager.addGoTo(0, 900);
            }
            else if(c=='o')
            {
                 cmdManager.addGoTo(900, 0);
            }
            
        }
        
        if(iaCom.readable()) {
        
            char c = 0;
            c = iaCom.getc();
            pc.printf("Recu : %c\n", c);
            //iaCom.printf("%d\n", c+1);
                        
            
        
            //char c = iaCom.getc();
            
            int mult = sens==0 ? 1 : -1;
            switch(c) 
            {
            
                case 'a' :
                    cmdManager.addGoTo(1046, mult * 140 );
                break;
                
                case 'b' :
                    cmdManager.addGoTo(1396, mult * 996 );
                break;
                
                case 'd' :
                    cmdManager.addGoToAngle(346, mult * 2046 );
                break;
                
                case 'e' :
                    cmdManager.addStraightLine(-200);
                break;
                
                case 'f' :
                    cmdManager.addGoTo(696, mult * 140 );
                break;
                    
                    
                case 'h' :
                    consignController.angle_Regu_Off();
                    consignController.dist_Regu_Off();
                break;
                
                case 's' :
                    pc.printf("STOP \n");
                    cmdManager.setEmergencyStop();
                break;
        
               case 'g' :
                    pc.printf("Reset_STOP \n");
                    cmdManager.resetEmergencyStop();
                break;
            
            
                case 'c' :
                    
                    sens = 0;
                    iaCom.scanf("%d", &sens);
                    printf("Debut callage sens : %d \n", sens);
                    consignController.calage_bordure(sens);
                    printf("fin calage bordure\n");
                    iaCom.printf("y\n");
                    //iaCom.putc('y');
                break;
            / *        
                case 'g' :
                    x=0;
                    y=0;
                    iaCom.scanf("%d %d", &x, &y);
                    //iaCom.flush();
                    //iaCom.scanf("%d", &y);
                    //iaCom.flush();
                    pc.printf("G => (%d , %d) \n", x, y);
                    
                    cmdManager.addGoTo(x, y);
                    break;
                    
                case 'a' :
                    x=0;
                    y=0;
                    iaCom.scanf("%d %d", &x, &y);
                    //iaCom.flush();
                    //iaCom.scanf("%d", &y);
                    //iaCom.flush();
                
                    pc.printf("a => (%d , %d) \n", x, y);
                    
                    cmdManager.addGoToAngle(x, y);
                    break;
                    
                case 'z' :
                    dist = 0;
                    iaCom.scanf("%d", &dist);
                    pc.printf("z => %d \n", dist);
                    cmdManager.addStraightLine(dist);
                    break;
                    
                case 's' :
                    dist = 0;
                    iaCom.scanf("%d", &dist);
                    pc.printf("s => %d \n", dist);
                    cmdManager.addStraightLine(-dist);
                    break;
                    
                case 'd' :
                    angle = 0;
                    iaCom.scanf("%d", &angle);
                    pc.printf("d => %d \n", -angle);
                    cmdManager.addTurn(-angle);
                    break;
                    
                case 'q' :
                    angle = 0;
                    pc.printf("q => %d \n", angle);
                    iaCom.scanf("%d", &angle);
                    cmdManager.addTurn(angle);
                    break;
                    
                case 'e' :
                    pc.printf("STOP \n");
                    cmdManager.setEmergencyStop();
                    break;
                    
                case 'r' :
                    pc.printf("Reset_STOP \n");
                    cmdManager.resetEmergencyStop();
                    break;
        
                case 'o' :
                    printf("Match commence !\n");
                    break;
                    
                case 'h' :
                    consignController.angle_Regu_Off();
                    consignController.dist_Regu_Off();
                    break;
                
                / *
                case 'x' :
                    iaCom.printf("%lld\r\n", odometrie.getXmm());
                    break;
                    
                case 'y' :
                    iaCom.printf("%lld\r\n", odometrie.getYmm());
                    break;
                    
                case 'a' :
                    iaCom.printf("%f\r\n", odometrie.getTheta());
                    break;
                * /  
                
        
            }
            
            
        }
        
        #ifdef DEBUG
            udp_data_send();
        #endif
        
       // printf("x=%lld,y=%lld\n", odometrie.getXmm(),  odometrie.getYmm());
        
        wait(0.01);
        myled = !myled;    
    }
    */
}


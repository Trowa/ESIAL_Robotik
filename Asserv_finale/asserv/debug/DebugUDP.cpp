#include "DebugUDP.h"

#ifdef DEBUG

#include <string>
#include <list>

DebugUDP::DebugUDP(CommandManager* c, Odometrie* o) {
	
	// Odometrie et ConsignController de l'asserv
	this->commandManager = c;
    this->odometrie = o;
	
	// Configuration IP
	eth = new EthernetNetIf(IpAddr(192,168,0,2), //IP Address
                            IpAddr(255,255,255,0), //Network Mask
                            IpAddr(192,168,0,1), //Gateway
                            IpAddr(192,168,0,1)  //DNS
                           );
	EthernetErr ethErr = eth->setup();
	
	debuggerHost = new Host(IpAddr(192,168,0,1), 12340);
	
	udp.setOnEvent(&onUDPSocketEvent); // ecouteur sur la socket UDP
	udp.bind(*debuggerHost);

	sendDebugData = false; //pour l'instant, on n'envoie rien
}

void DebugUDP::setDebugSend(bool b) {
	sendDebugData = b;
	dropCurrentData();
}

bool DebugUDP::getDebugSend() {
	return sendDebugData;
}

void DebugUDP::addData(char* name, double value) {
    sprintf(trame, "%s%s=%lf#",trame, name, value);
}

void DebugUDP::addData(char* name, int64_t value) {
    sprintf(trame, "%s%s=%lld#",trame, name, value);
}

void DebugUDP::sendData() {
   udp.sendto( trame, strlen(trame), debuggerHost);
   trame[0] = '\0';  
}

void DebugUDP::dropCurrentData() {
   trame[0] = '\0';
}

void onUDPSocketEvent(UDPSocketEvent e) {
	
	char* buffer = NULL;
	int bufferSize = 0;

    switch(e) {
        case UDPSOCKET_READABLE: //The only event for now
            Host host = *(debugUdp->getDebuggerHost());
            char name;
            receiveLed = !receiveLed;
            /* lecture */
			bufferSize = 1024;
			buffer = (char*) malloc(bufferSize);
			memset(buffer, 0, bufferSize);
			//Net::poll();
            int len = udp.recvfrom( buffer, bufferSize-1, &host );

			while(buffer[0]!=0) {

				name = buffer[0];          
	
	            switch(name) {    /*todo: ajouter appel setters*/
	            
					// Definition des variables pour l'odometrie
					
					case 'a': // Nombre de tics codeurs en 1m pour codeur gauche
						sscanf(buffer, "%c=%lf", &name, &frontParMetreCodeurG);
						break;
						
					case 'b': // Nombre de tics codeurs en 1m pour codeur droite
						sscanf(buffer, "%c=%lf", &name, &frontParMetreCodeurD);
						break;
	
					case 'c': // Distance entre les roues codeuses en mm
						sscanf(buffer, "%c=%lld", &name, &distRoues);
						break;
	
					case 'd':  // Nombre d'UO pour un tic de codeur
						sscanf(buffer, "%c=%lld", &name, &uOParFront);
						break;                
	               
	               // Moteurs
	               
					case 'e':  // MD22 : 1 a 127, vitesse maximum positive
						sscanf(buffer, "%c=%lld", &name, &V_MAX_POS_MOTOR);
						break;  
						
					case 'f':  // MD22 : -1 a -128, vitesse maximum negative
						sscanf(buffer, "%c=%lld", &name, &V_MAX_NEG_MOTOR);
						break;
	               
					// PID en distance
					
					case 'g': // Coeff proportionelle
						sscanf(buffer, "%c=%lld", &name, &DIST_KP);
						break;
						
					case 'h':  // Coeff integrale
						sscanf(buffer, "%c=%lld", &name, &DIST_KI);
						break;
					
					case 'i': // Coeff derivee
						sscanf(buffer, "%c=%lld", &name, &DIST_KD);
						break;
					
					case 'j': // Coeff permettant de diminuer les valeurs du PID
						sscanf(buffer, "%c=%lf", &name, &DIST_OUT_RATIO);
						break;
						
					case 'k': // Valeur de sortie maximum pour le moteur
						sscanf(buffer, "%c=%lld", &name, &DIST_MAX_OUTPUT);
						break;
	
					case 'l': // Valeur maximum de l'integrale (0 = filtre PD)
						sscanf(buffer, "%c=%lld", &name, &DIST_MAX_INTEGRAL);
						break;
					
					
					
					// PID en angle
					
					case 'm': // Coeff proportionelle
						sscanf(buffer, "%c=%lld", &name, &ANGLE_KP);
						break;
						
					case 'n':  // Coeff intégrale
						sscanf(buffer, "%c=%lld", &name, &ANGLE_KI);
						break;
					
					case 'o': // Coeff derivee
						sscanf(buffer, "%c=%lld", &name, &ANGLE_KD);
						break;
					
					case 'p': // Coeff permettant de diminuer les valeurs du PID
						sscanf(buffer, "%c=%lf", &name, &ANGLE_OUT_RATIO);
						break;
						
					case 'q': // Valeur de sortie maximum pour le moteur
						sscanf(buffer, "%c=%lld", &name, &ANGLE_MAX_OUTPUT);
						break;
	
					case 'r': // Valeur maximum de l'integrale (0 = filtre PD)
						sscanf(buffer, "%c=%lld", &name, &ANGLE_MAX_INTEGRAL);
						break;
						
					case 's': // Fenetre de l'angle dans lequel on considere que le GoTo peut commencer a avancer
						sscanf(buffer, "%c=%lf", &name, &angleThreshold);
						break;
	
					
					// QUADRAMPDERIVEE Distance				
					case 'A': // Vitesse max en marche avant
						sscanf(buffer, "%c=%lld", &name, &DIST_QUAD_1ST_POS);
						break;
					
					case 'B': // Vitesse max en marche arriÃ¨re
						sscanf(buffer, "%c=%lld", &name, &DIST_QUAD_1ST_NEG);
						break;
						
					case 'C': // Acceleration max en marche avant
						sscanf(buffer, "%c=%lld", &name, &DIST_QUAD_AV_2ND_ACC);
						break;
						
					case 'D': // Deceleration max en marche avant
						sscanf(buffer, "%c=%lld", &name, &DIST_QUAD_AV_2ND_DEC);
						break;
						
					case 'E': // Coeff determinant le debut de la rampe de deceleration en marche avant
						sscanf(buffer, "%c=%lld", &name, &DIST_QUAD_AV_ANTICIPATION_GAIN_COEF);
						break;
						
					case 'F': // Acceleration max en marche arriere
						sscanf(buffer, "%c=%lld", &name, &DIST_QUAD_AR_2ND_ACC);
						break;
						
					case 'G': // Deceleration max en marche arriere
						sscanf(buffer, "%c=%lld", &name, &DIST_QUAD_AR_2ND_DEC);
						break;
						
					case 'H': // Coeff determinant le debut de la rampe de deceleration en marche arriere
						sscanf(buffer, "%c=%lld", &name, &DIST_QUAD_AR_ANTICIPATION_GAIN_COEF);
						break;
						
					case 'I': // Largeur de la zone ou l'on considere etre arrive (UO)
						sscanf(buffer, "%c=%lld", &name, &DIST_TAILLE_FENETRE_ARRIVEE);
						break;
						
					// QUADRAMPDERIVEE Angle
					case 'J': // Vitesse max en rotation
						sscanf(buffer, "%c=%lld", &name, &ANGLE_QUAD_1ST_POS);
						break;
						
					case 'K': // Acceleration max en rotation
						sscanf(buffer, "%c=%lld", &name, &ANGLE_QUAD_2ND_ACC);
						break;
						
					case 'L': // Deceleration max en rotation
						sscanf(buffer, "%c=%lld", &name, &ANGLE_QUAD_2ND_DEC);
						break;
						
					case 'M': // Coeff determinant le debut de la rampe de deceleration en rotation
						sscanf(buffer, "%c=%lld", &name, &ANGLE_QUAD_ANTICIPATION_GAIN_COEF);
						break;
						
					case 'N': // Largeur de la zone ou l'on considere etre arrive (UO)
						sscanf(buffer, "%c=%lld", &name, &ANGLE_TAILLE_FENETRE_ARRIVEE);
						break;
							
	               
	               /* ------------------ CONSIGNES  ------------------------ */  
	                case '0': {  /* Ajout GOTO, */
						int64_t posXInmm;
						int64_t posYInmm;
						sscanf(buffer, "%c=%lld;%lld", &name, &posXInmm, &posYInmm);
	                    commandManager->addGoTo(posXInmm, posYInmm);
	                    break;
					}
	                    
	                case '1': {  /* Ajout GOTOANGLE */
	                    int64_t posXInmm;
						int64_t posYInmm;
						sscanf(buffer, "%c=%lld;%lld", &name, &posXInmm, &posYInmm);
	                    commandManager->addGoToAngle(posXInmm, posYInmm);           
					}	break;
	                    
	                case '2': {   /* Ajout GO */
						int64_t distanceInmm;
						sscanf(buffer, "%c=%lld", &name, &distanceInmm);
	                    commandManager->addStraightLine(distanceInmm);
					}	break;
	                    
	                case '3': {   /* Ajout TURN */
						int64_t angleDegre;
						sscanf(buffer, "%c=%lld", &name, &angleDegre);
	                    commandManager->addTurn(angleDegre);
	                    break;
					}
	
					/* Demande de données */
	
					case 'O': //On doit envoyer les constantes
						debugUdp->dropCurrentData(); //on efface tout pour le moment
						//debugUdp->addData("frontParMetreCodeurG", (double)frontParMetreCodeurG);
						//debugUdp->addData("frontParMetreCodeurD", (double)frontParMetreCodeurD);
						//debugUdp->addData("distRoues", (double)distRoues);
						debugUdp->addData("d", (double)uOParFront);
						debugUdp->addData("e", (double)V_MAX_POS_MOTOR);
						debugUdp->addData("f", (double)V_MAX_NEG_MOTOR);
						debugUdp->addData("g", (double)DIST_KP);
						debugUdp->addData("h", (double)DIST_KI);
						debugUdp->addData("i", (double)DIST_KD);
						debugUdp->addData("j", (double)DIST_OUT_RATIO);
						debugUdp->addData("k", (double)DIST_MAX_OUTPUT);
						debugUdp->addData("l", (double)DIST_MAX_INTEGRAL);
						debugUdp->addData("m", (double)ANGLE_KP);
						debugUdp->addData("n", (double)ANGLE_KI);
						debugUdp->addData("o", (double)ANGLE_KD);
						debugUdp->addData("p", (double)ANGLE_OUT_RATIO);
						debugUdp->addData("q", (double)ANGLE_MAX_OUTPUT);
						debugUdp->addData("r", (double)ANGLE_MAX_INTEGRAL);
						debugUdp->addData("s", (double)angleThreshold);
						debugUdp->addData("A", (double)DIST_QUAD_1ST_POS);
						debugUdp->addData("B", (double)DIST_QUAD_1ST_NEG);
						debugUdp->addData("C", (double)DIST_QUAD_AV_2ND_ACC);
						debugUdp->addData("D", (double)DIST_QUAD_AV_2ND_DEC);
						debugUdp->addData("E", (double)DIST_QUAD_AV_ANTICIPATION_GAIN_COEF);
						debugUdp->addData("F", (double)DIST_QUAD_AR_2ND_ACC);
						debugUdp->addData("G", (double)DIST_QUAD_AR_2ND_DEC);
						debugUdp->addData("H", (double)DIST_QUAD_AR_ANTICIPATION_GAIN_COEF);
						debugUdp->addData("I", (double)DIST_TAILLE_FENETRE_ARRIVEE);
						debugUdp->addData("J", (double)ANGLE_QUAD_1ST_POS);
						debugUdp->addData("K", (double)ANGLE_QUAD_2ND_ACC);
						debugUdp->addData("L", (double)ANGLE_QUAD_2ND_DEC);
						debugUdp->addData("M", (double)ANGLE_QUAD_ANTICIPATION_GAIN_COEF);
						debugUdp->addData("N", (double)ANGLE_TAILLE_FENETRE_ARRIVEE);
						debugUdp->sendData();
						break;
	
					case 'S':
						debugUdp->setDebugSend(true);
						pc.printf("debug activé\n");
						break;
	
					case 'T':
						debugUdp->setDebugSend(false);
						break;
	
	            }

				char* newIndex = index(buffer, '#');
				if(newIndex!=NULL) {
					bufferSize = bufferSize-(newIndex-buffer);
					char* newBuffer = (char*) malloc(bufferSize);
					memset(newBuffer, 0, bufferSize);
					strcpy(newBuffer, newIndex+1);
					free(buffer);
					buffer = newBuffer;
				} else {
					buffer[0]=0;
				}


			}
            break;
    }
	if(buffer != NULL) {
		free(buffer);
	}
 }

char* index(const char *s, int c) {
	int i = 0;
	while(s[i]!=0) {
		if(s[i]==c) {
			return (char*)(s+i);
		}
		i++;
	}
	return NULL;
}

 #endif

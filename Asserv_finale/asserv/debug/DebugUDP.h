#ifndef DEBUG_UDP_H
#define DEBUG_UDP_H

#include "LPC1768/if/eth/EthernetNetIf.h"
#include "LPC1768/api/UDPSocket.h"
#include "LPC1768/core/host.h"
#include "../Odometrie/Odometrie.h"
#include "../ConsignController/ConsignController.h"
#include "../CommandManager/CommandManager.h"
#include "../config.h"

#ifdef DEBUG

extern Odometrie *odometrie;
extern CommandManager *commandManager;
extern UDPSocket udp;
extern DigitalOut receiveLed;
extern void resetAsserv();

class DebugUDP {
	
	public:
		DebugUDP(CommandManager* c, Odometrie* o);
		void addData(char* name, double value); // ajoute une valeur au paquet courant
		void addData(char* name, int64_t value); // ajoute une valeur enti�re au paquet courant
		void sendData(); // envoie le paquet de donnees sur l'ethernet
		void dropCurrentData(); //vire les donn�es courantes du buffer
		
		void setNewObjectPointers(CommandManager* c, Odometrie* o); //Reparam�trage des pointeurs

		void setDebugSend(bool b); //Param�tre l'envoi des donn�es de debug
		bool getDebugSend(); //Doit-on envoyer les donn�es ?
		Host* getDebuggerHost() { return debuggerHost; }

	private:
		EthernetNetIf *eth;
		Host* debuggerHost;
		bool sendDebugData; //Doit-on envoyer les donn�es ?
		char trame[4096];
		CommandManager* commandManager;
	    Odometrie* odometrie;
};

extern DebugUDP *debugUdp;

void onUDPSocketEvent(UDPSocketEvent e);

char* index(const char *s, int c);
	
#endif

#endif

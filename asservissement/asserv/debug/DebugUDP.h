#ifndef DEBUG_UDP_H
#define DEBUG_UDP_H

#ifdef DEBUG

#include "LPC1768/if/eth/EthernetNetIf.h"
#include "LPC1768/api/UDPSocket.h"
#include "LPC1768/core/host.h"
#include "../odometrie/Odometrie.h"
#include "../consignController/ConsignController.h"
#include "../commandManager/CommandManager.h"
#include "../config/config.h"

extern Odometrie *odometrie;
extern CommandManager *commandManager;
extern UDPSocket udp;
extern DigitalOut receiveLed;
extern void resetAsserv();

class DebugUDP
{

public:
    DebugUDP(CommandManager* c, Odometrie* o);
    void addData(char* name, double value); // ajoute une valeur au paquet courant
    void addData(char* name, int64_t value); // ajoute une valeur entière au paquet courant
    void sendData(); // envoie le paquet de données sur l'ethernet
    void dropCurrentData(); //vire les données courantes du buffer

    void setNewObjectPointers(CommandManager* c, Odometrie* o); //Reparamétrage des pointeurs

    void setDebugSend(bool b); //Paramètre l'envoi des données de debug
    bool getDebugSend(); //Doit-on envoyer les données ?
    Host* getDebuggerHost() {
        return debuggerHost;
    }

private:
    EthernetNetIf *eth;
    Host* debuggerHost;
    bool sendDebugData; //Doit-on envoyer les données ?
    char trame[4096];
    CommandManager* commandManager;
    Odometrie* odometrie;
};

extern DebugUDP *debugUdp;

void onUDPSocketEvent(UDPSocketEvent e);

char* index(const char *s, int c);

#endif

#endif

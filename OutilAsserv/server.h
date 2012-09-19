#ifndef SERVER_H
#define SERVER_H

/* ****************************************************************
       SERVEUR (qui gére Toute la communication avec le robot)
 **************************************************************** */

#include <QtNetwork/QUdpSocket>
#include "canvasplot.h"
#include <sstream>

class Server: public QWidget
{
    // Ligne obligatoire pour créer ses propres signaux/slots (ie événements/listeners)
    Q_OBJECT

    public:
        // Constructeur/Destructeur
        Server(CanvasPlot& p_canvas); // Le serveur a besoin du canvas pour le modifier directement
        ~Server();

        // Commence/Arrête l’écoute du robot
        void start();
        void stop();

        // Demande les constantes au robot
        void demanderConsts();

    public slots :
        // Méthodes qui lisent les données que le robot envoie (appelées à chaque paquet reçu)
        void readPendingDatagrams(); // … lorsqu’on écoute le robot

        // Modifier une constante
        void setConstante(QChar s, int v);
        void setConstante(QChar s, double v);

        // Indique au robot qu’on commence/arrête l’envoie de constantes
        void startEnvoi();
        void stopEnvoi();

        // Comme son nom l’indique, donne un ordre au robot
        void donnerOrdre(QString s);

    signals :
        // Signaux lorsqu’on reçoit une constante du robot
        void constante(QChar, int);
        void constante(QChar, double);

        // Signal pour indiquer que toutes les constantes ont été envoyées
        void finConstante();

    private:
        QUdpSocket* m_socket;
        CanvasPlot& m_canvas;
        bool m_ecoute; // Ecoute-t-on le robot ?
        bool m_const; // Ecoute-t-on en mode constantes ?
        int m_port;
        QHostAddress m_adress;
        std::ostringstream m_flux;
};

#endif // SERVER_H

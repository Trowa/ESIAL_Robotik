#include "server.h"
#include <QMessageBox>

// Constructeur
Server::Server(CanvasPlot& p_canvas) :
    m_canvas(p_canvas),
    m_socket(NULL),
    m_ecoute(false),
    m_const(false),
    m_port(12340),
    m_adress("192.168.0.2")
{
    // On crée le socket qu’on connecte au robot
    m_socket = new QUdpSocket(this);

    if(!m_socket->bind(m_port))
    {
        QMessageBox::critical(this, "Erreur", "Impossible de se connecter pour la réception (Le programme est peut-être déjà ouvert) !");
    }

    connect(m_socket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
}

// Destructeur
Server::~Server()
{
    delete m_socket;
}

// Méthode appelée lorsque des paquets sont reçus
void Server::readPendingDatagrams()
{
    // TODO : trouver pourquoi ça ne marche pas si on met la boucle qui suit dans la condition m_ecoute
    QByteArray paquetsConcatenes;
    while(m_socket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(m_socket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        // On lit le paquet
        m_socket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        paquetsConcatenes += datagram;

    }
    // */

    if(m_ecoute)
    {

        if(!m_const) // Mode réception des données temps réel
        {
            // On sépare les différents mots
            QList<QByteArray> l = paquetsConcatenes.split('#');
            QList<QByteArray> varVal;
            QString typeVar;
            float var;

            // Tant qu’on a au moins un mot…
            while(l.size() >= 1)
            {
                varVal = l.front().split('=');
                l.pop_front();

                if(varVal.size() == 2)
                {
                    // On met dans typeVar le nom de la variable, on retire le mot
                    typeVar = varVal.front().data();
                    varVal.pop_front();
                    var = ((QString)(varVal.front().data())).toFloat();

                    // Si c’est le temps, on modifie le temps du graphe
                    if(typeVar == "t")
                    {
                        m_canvas.setTime(var);
                    }
                    else // sinon, on ajoute un point
                    {
                        m_canvas.appendPoint(typeVar.toStdString(), var);
                    }
                }
            }
        }
        else // Mode réception des constantes
        {
            // On découpe le paquet en mots
            QList<QByteArray> l = paquetsConcatenes.split('#');
            QList<QByteArray> varVal;
            QString typeVar;
            float var;

            // Tant qu’il y a des mots
            while(l.size() >= 1)
            {
                // On envoie un signal pour modifier le coeff (le premier mot de la liste est le nom du coeff, le deuxième sa valeur)
                varVal = l.front().split('=');
                l.pop_front();

                if(varVal.size() == 2)
                {
                    typeVar = varVal.front().data();
                    varVal.pop_front();
                    var = ((QString)(varVal.front().data())).toFloat();

                    emit constante(*typeVar.begin(), var);
                }
            }

            // On envoie le signal qui dit que toutes les constantes ont été reçus
            emit finConstante();

            // On déconnecte le readConsts : on n’est plus sensé recevoir de paquets (et si on en reçoit, c’est en mode données)
            m_const = false;
            m_ecoute = false;
        }
    }
}

// Envoie le paquet pour lancer la lecture des données (et connecte la méthode de lecture au signal reçu lorsqu’un paquet est prêt)
void Server::start()
{
    m_ecoute = true;
    m_const = false;
    m_socket->writeDatagram(QByteArray("S="), m_adress, m_port);
}

// Envoie le paquet pour lancer la lecture des coeffs (et connecte la méthode qu’il faut)
void Server::demanderConsts()
{
    m_const = true;
    m_ecoute = true;
    m_socket->writeDatagram(QByteArray("O="), m_adress, m_port);
}

// Arrête la lecture et prévient le robot
void Server::stop()
{
    m_ecoute = false;

    m_socket->writeDatagram(QByteArray("T="), m_adress, m_port);
}

// Envoie au robot un coeff et sa nouvelle valeur
void Server::setConstante(QChar s, int v)
{
    m_flux << s.toAscii() << "=" << v << "#";
}

// Idem pour les coeffs double
void Server::setConstante(QChar s, double v)
{
    m_flux << s.toAscii() << "=" << v << "#";
}

// Méthode qui est appelée avant d’envoyer les coeffs au robot lors d’une sauvegarde
void Server::startEnvoi()
{
    m_flux.str("");
}

// Méthode qui est appelée après l’envoi des coeffs lors d’une sauvegarde
void Server::stopEnvoi()
{
    m_socket->writeDatagram(QByteArray(m_flux.str().c_str()), m_adress, m_port);
}

// Méthode qui envoie un ordre au robot (précédé du mot «Ordre» pour prévenir que c’est un ordre)
void Server::donnerOrdre(QString s)
{
    m_socket->writeDatagram(QByteArray(s.toStdString().c_str()), m_adress, m_port);
}

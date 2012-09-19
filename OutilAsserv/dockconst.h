#ifndef DOCKCONST_H
#define DOCKCONST_H

/* ****************************************************************
            DOCK de gauche (où se trouvent les coeffs)
 **************************************************************** */

// On inclut la classe mère
#include <QDockWidget>

// Et quelques classes pour les attributs
#include <QVBoxLayout>
#include <QGroupBox>
#include <QMap>

class QFile;

class DockConst : public QDockWidget
{
    // Ligne obligatoire pour créer ses propres signaux/slots (ie événements/listeners)
    Q_OBJECT

    public:
        // Constructeur
        DockConst(QWidget* p_parent = NULL);
        // Méthode pour ajouter un coeff (et une catégorie si elle n’existe pas)
        void addConst(QChar p_id, QString p_categorie, QString p_nom, QString p_bulle, bool p_double = false);

        void loadFrom(QFile* p_file);
        void saveIn(QFile* p_file);

    public slots :
        // Slots qui modifient un coeff
        void setConstante(QChar s, int v);
        void setConstante(QChar s, double v);
        // Slot qui envoie tous les coeffs
        void envoyerTout();

    signals :
        // Signaux envoyés lors d’une modif d’un coeff
        void constante(QChar, int);
        void constante(QChar, double);

        // Signaux envoyés lorsqu’on commence/arrête l’envoi de coeffs
        void startEnvoi();
        void stopEnvoi();

    private:
        QVBoxLayout* m_layout;
        QMap<QString, QGroupBox*> m_categories; // Map qui associe à un nom de catégorie la groupBox associée
        QMap<QChar, QWidget*> m_constantes; // Map qui associe à un nom de coeff le input correspondant
};

#endif // DOCKCONST_H

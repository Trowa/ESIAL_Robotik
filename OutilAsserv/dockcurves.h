#ifndef DOCKCURVES_H
#define DOCKCURVES_H

/* ************************************************************************************
         DOCK de droite contenant les cases à cocher pour activer/désactiver un graphe
 ************************************************************************************ */

// On inclut la classe mère
#include <QDockWidget>

// On déclare les classes des attributs
class QFormLayout;

class DockCurves : public QDockWidget
{
    // Ligne obligatoire pour créer ses propres signaux/slots (ie événements/listeners)
    Q_OBJECT

    public:
        // Constructeur
        DockCurves(QWidget* p_parent = NULL);

    public slots :
        void addNewVar(QString p); // slot qui crée une nouvelle variable
        void changeBox(QString, Qt::CheckState); // slot exécuté lorsqu’une case à cocher a changé d’état
        void changeColor(QString, QString);

    signals :
        // Signal envoyé lors d’un changement d’une des cases à cocher
        void varChanged(QString, Qt::CheckState);
        void colorChanged(QString, QString);

    private :
        QFormLayout* m_layout;
};

#endif // DOCKCURVES_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H


/* ****************************************************************
                        FENETRE PRINCIPALE
 **************************************************************** */

// On inclut la classe mère
#include <QMainWindow>

// On déclare les classes des attributs
class Server;
class DockCurves;
class DockConst;

class CanvasPlot;


class MainWindow : public QMainWindow
{
    // Ligne obligatoire pour créer ses propres signaux/slots (ie événements/listeners)
    Q_OBJECT

    public:
        // Constructeur/Destructeur
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    public slots :
        void activerTrace();
        void desactiverTrace();
        void demanderOrdre(); // Demande à l’utilisateur de donner un ordre
        void chargerCoeffs(); // Charge les coefficients du robot vers le programme
        void reactiverTout(); // Réactive les actions

        void charger();
        void enregistrer();

    signals :
        void ordre(QString); // signal envoyé après saisie d’un ordre par l’utilisateur

    private:
        QToolBar* toolBar(); // Méthode qui crée la toolbar

        // Les différentes actions de la toolbar
        QAction* m_startAction;
        QAction* m_realLoadAction;
        QAction* m_saveAction;
        QAction* m_loadAction;
        QAction* m_commitAction;
        QAction* m_expectAction;
        QAction* m_zoomAction;

        // Le graphe
        CanvasPlot* m_canvas;

        // Le serveur (qui envoie et reçoit tous les paquets réseau)
        Server* m_server;

        // Le dock de droite (où se trouvent les cases à cocher/décocher pour afficher/cacher un graphe)
        DockCurves* m_dock;
        // Le dock de gauche (où se trouvent les coeffs à régler)
        DockConst* m_dockCst;
};

#endif // MAINWINDOW_H

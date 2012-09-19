#include "mainwindow.h"

#include <QAction>
#include <QToolBar>

#include "server.h"
#include "dockcurves.h"
#include "dockconst.h"

#include "CommandDialog.h"
#include <QInputDialog>

#include "canvasplot.h"

#include <QFileDialog>
#include <QMessageBox>

// Constructeur de la fen�tre principale
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    // On cr�e le canvas qui contient les graphes, on le d�finit comme widget central (<- pour les non anglophones)
    m_canvas = new CanvasPlot(this);
    this->setCentralWidget(m_canvas);

    // On cr�e le serveur (<- commentaire inutile)
    m_server = new Server(*m_canvas);

    // On cr�e les docks /!\ Ceci n?a rien � voir avec les �vang�liques.
    m_dock = new DockCurves(this);
    this->addDockWidget(Qt::RightDockWidgetArea, m_dock);

    m_dockCst = new DockConst(this);
    this->addDockWidget(Qt::LeftDockWidgetArea, m_dockCst);

    // On cr�e la toolbar (barre d?outil pour les non anglophones)
    addToolBar(toolBar());

    // On d�finit la taille minimum de la fen�tre
    this->setMinimumWidth(800);
    this->setMinimumHeight(600);

    // On met la fen�tre on rose, parce que le rose, c?est joli (ou pas, mais rose, c?est Robotik, et Robotik c?est rose)
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(255,105,180));
    this->setPalette(palette);

    // On connecte les signaux aux slots
    connect(m_canvas, SIGNAL(newVar(QString)), m_dock, SLOT(addNewVar(QString)));
    connect(m_dock, SIGNAL(varChanged(QString, Qt::CheckState)), m_canvas, SLOT(changeVar(QString, Qt::CheckState)));
    connect(m_dock, SIGNAL(colorChanged(QString,QString)), m_canvas, SLOT(changeColor(QString, QString)));


    QMessageBox::information(this, "Attention", " - Ne pas redimensionner la fen�tre si des courbes sont d�j� dessin�es !\n - Ne surtout pas ouvrir un fichier autre qu'un fichier cr�� par ce programme.");
/*
    m_canvas->appendPoint("Test", 12);
    m_canvas->setTime(90);
    m_canvas->appendPoint("Test", 2000);*/
}

// Destructeur
MainWindow::~MainWindow()
{
    delete m_server;
    delete m_canvas;
}

// M�thode qui cr�e la toolbar (et la renvoie)
QToolBar *MainWindow::toolBar()
{
    // On cr�e une toolbar en lui donnant un nom (qui n?est affich� nulle part et qui ne sert donc � rien)
    QToolBar *toolBarStart = addToolBar("Barre d?outil");

    // On cr�e les diff�rentes actions
    m_startAction = new QAction("Ecouter", toolBarStart);
    toolBarStart->addAction(m_startAction);

    m_realLoadAction = new QAction("Charger depuis fichier", toolBarStart);
    toolBarStart->addAction(m_realLoadAction);

    m_loadAction = new QAction("Charger depuis Robot", toolBarStart);
    toolBarStart->addAction(m_loadAction);

    m_saveAction = new QAction("Enregistrer", toolBarStart);
    toolBarStart->addAction(m_saveAction);

    m_commitAction = new QAction("Envoyer au robot", toolBarStart);
    toolBarStart->addAction(m_commitAction);

    m_expectAction = new QAction("Soumettre un ordre", toolBarStart);
    toolBarStart->addAction(m_expectAction);

    // On connecte les signaux et les slots de chaque action
    connect(m_startAction, SIGNAL(triggered()), this, SLOT(activerTrace()));
    connect(m_realLoadAction, SIGNAL(triggered()), this, SLOT(charger()));
    connect(m_saveAction, SIGNAL(triggered()), this, SLOT(enregistrer()));
    connect(m_loadAction, SIGNAL(triggered()), this, SLOT(chargerCoeffs()));
    connect(m_commitAction, SIGNAL(triggered()), m_dockCst, SLOT(envoyerTout()));
    connect(m_expectAction, SIGNAL(triggered()), this, SLOT(demanderOrdre()));

    // Ceux du serveur
        // Lorsque le serveur envoie un signal constante, le dock doit mettre � jour cette constante
    connect(m_server, SIGNAL(constante(QChar, int)), m_dockCst, SLOT(setConstante(QChar, int)));
    connect(m_server, SIGNAL(constante(QChar, double)), m_dockCst, SLOT(setConstante(QChar, double)));
        // Lorsque le serveur a fini d?envoyer les constantes, on r�active tous les boutons
    connect(m_server, SIGNAL(finConstante()), this, SLOT(reactiverTout()));

    // Ceux du dock
        // On pr�viens le serveur lorsqu?on commence et finit l?envoi des constantes
    connect(m_dockCst, SIGNAL(startEnvoi()), m_server, SLOT(startEnvoi()));
    connect(m_dockCst, SIGNAL(stopEnvoi()), m_server, SLOT(stopEnvoi()));
        // Lorsque le dock envoie le signal constante, on envoie au serveur la constante (pour qu?il l?envoie au robot)
    connect(m_dockCst, SIGNAL(constante(QChar, int)), m_server, SLOT(setConstante(QChar, int)));
    connect(m_dockCst, SIGNAL(constante(QChar, double)), m_server, SLOT(setConstante(QChar, double)));

    // Lorsque le signal ordre est envoy�, on l?envoie au serveur.
    connect(this, SIGNAL(ordre(QString)), m_server, SLOT(donnerOrdre(QString)));


    // On retourne la toolbar qu?on a cr��e
    return toolBarStart;
}

// Activer le trac�
void MainWindow::activerTrace()
{
    // Le startAction devient un stopAction
    m_startAction->setIconText("Arreter");
    disconnect(m_startAction, SIGNAL(triggered()), this, SLOT(activerTrace()));
    connect(m_startAction, SIGNAL(triggered()), this, SLOT(desactiverTrace()));

    // On d�sactive la possibilit� de charger et de sauvegarder
    m_loadAction->setEnabled(false);
    m_commitAction->setEnabled(false);

    // On pr�vient le serveur qu?il peut envoyer les donn�es
    m_server->start();
}

// D�sactiver le trac�
void MainWindow::desactiverTrace()
{
    // Le stopAction redevient un startAction
    m_startAction->setIconText("Ecouter");
    disconnect(m_startAction, SIGNAL(triggered()), this, SLOT(desactiverTrace()));
    connect(m_startAction, SIGNAL(triggered()), this, SLOT(activerTrace()));

    // On r�active la possibilit� de charger et sauvegarder
    m_loadAction->setEnabled(true);
    m_commitAction->setEnabled(true);

    // On pr�vient le serveur qu?il doit arr�ter d?envoyer les donn�es
    m_server->stop();
}

// Charger les coeffs (<- commentaire inutile)
void MainWindow::chargerCoeffs()
{
    // On d�sactive tous les boutons (pour �viter d?embrouiller le robot, on r�active lorsqu?il a tout envoy�)
    m_startAction->setEnabled(false);
    m_loadAction->setEnabled(false);
    m_commitAction->setEnabled(false);
    m_expectAction->setEnabled(false);

    // On demande les coeffs au serveur
    m_server->demanderConsts();
}

// Demander ordre
void MainWindow::demanderOrdre()
{
    // Demande un ordre � l?utilisateur
    QString t = CommandDialog::getCommand(this);
    //= CommandDialog::getText(this, "Donner un ordre", "Donner un ordre au robot");

    // On envoie l?ordre sous forme d?un signal
    emit ordre(t);
}

// R�activer les actions d�sactiv�es lors du chargement des coeffs
void MainWindow::reactiverTout()
{
    m_startAction->setEnabled(true);
    m_loadAction->setEnabled(true);
    m_commitAction->setEnabled(true);
    m_expectAction->setEnabled(true);
}

void MainWindow::charger()
{
    QString filename = QFileDialog::getOpenFileName(this, "Charger un fichier", QString(), "*.cstsPrincesse");
    QFile file(filename);
    m_dockCst->loadFrom(&file);
}

void MainWindow::enregistrer()
{
    QString filename = QFileDialog::getSaveFileName(this, "Enregistrer un fichier", "test.cstsPrincesse", "*.cstsPrincesse");
    QFile file(filename);

    m_dockCst->saveIn(&file);
}

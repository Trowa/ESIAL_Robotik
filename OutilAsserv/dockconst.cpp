#include "dockconst.h"

#include <QLabel>
#include <QScrollArea>
#include <QGroupBox>
#include <QDoubleSpinBox>
#include <QGridLayout>
#include <iostream>
#include <QTextStream>
#include <QFile>
#include <QMessageBox>

// Constructeur
DockConst::DockConst(QWidget* p_parent) :
    QDockWidget(p_parent)
{
    QWidget* contenu = new QWidget();
    m_layout = new QVBoxLayout();
    contenu->setLayout(m_layout);

    // On définit la taille du dock
    contenu->resize(250,1000);

    // On ajoute une scrollbar verticale
    QScrollArea* scroll = new QScrollArea();
    scroll->setWidget(contenu);
    scroll->verticalScrollBar();
    // On désactive la scrollbar horizontale
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setWidget(scroll);

    // On crée tous les champs pour tous les coeffs

    addConst('s', "Command Manager", "angleThreshold", "Fenetre de l'angle dans lequel on considere que le GoTo peut commencer a avancer.", true);
    //addConst('2', "Command Manager", "returnThreshold", "Distance en dessous de laquelle on revient en marche arriere si on depasse, sinon demi-tour");

    addConst('d', "Odometrie", "uOParFront", "Nombre d'UO pour un tic de codeur");

    addConst('e', "Moteurs", "V_MAX_POS_MOTOR", "MD22 : 1 a 127, vitesse maximum positive");
    addConst('f', "Moteurs", "V_MAX_NEG_MOTOR", "MD22 : -1 a -128, vitesse maximum negative");

    addConst('g', "Pid", "DIST_KP", "Coeff proportionelle");
    addConst('h', "Pid", "DIST_KI", "Coeff integrale");
    addConst('i', "Pid", "DIST_KD", "Coeff derivee");

    addConst('j', "Pid", "DIST_OUT_RATIO", "Coeff permettant de diminuer les valeurs du PID", true);
    addConst('k', "Pid", "DIST_MAX_OUTPUT", "Valeur de sortie maximum pour le moteur");
    addConst('l', "Pid", "DIST_MAX_INTEGRAL", "Valeur maximum de l'integrale (0 = filtre PD)");

    addConst('m', "Pid", "ANGLE_KP", "Coeff proportionel");
    addConst('n', "Pid", "ANGLE_KI", "Coeff integrale");
    addConst('o', "Pid", "ANGLE_KD", "Coeff derivee");
    addConst('p', "Pid", "ANGLE_OUT_RATIO", "Coeff permettant de diminuer les valeurs du PID", true);
    addConst('q', "Pid", "ANGLE_MAX_OUTPUT", "Valeur de sortie maximum pour le moteur");
    addConst('r', "Pid", "ANGLE_MAX_INTEGRAL", "Valeur maximum de l'integrale (0 = filtre PD)");

    addConst('A', "QUADRAMPDerivee", "DIST_QUAD_1ST_POS", "Vitesse max en marche avant");
    addConst('B', "QUADRAMPDerivee", "DIST_QUAD_1ST_NEG", "Vitesse max en marche arriere");
    addConst('C', "QUADRAMPDerivee", "DIST_QUAD_AV_2ND_ACC", "Acceleration max en marche avant");
    addConst('D', "QUADRAMPDerivee", "DIST_QUAD_AV_2ND_DEC", "Deceleration max en marche avant");
    addConst('E', "QUADRAMPDerivee", "DIST_QUAD_AV_ANTICIPATION_GAIN_COEF", "Coeff determinant le debut de la rampe de deceleration en marche avant");
    addConst('F', "QUADRAMPDerivee", "DIST_QUAD_AR_2ND_ACC", "Acceleration max en marche arriere");
    addConst('G', "QUADRAMPDerivee", "DIST_QUAD_AR_2ND_DEC", "Deceleration max en marche arriere");
    addConst('H', "QUADRAMPDerivee", "DIST_QUAD_AR_ANTICIPATION_GAIN_COEF", "Coeff determinant le debut de la rampe de deceleration en marche arriere");
    addConst('I', "QUADRAMPDerivee", "DIST_TAILLE_FENETRE_ARRIVEE", "Largeur de la zone ou l'on considere etre arrive (UO)");

    addConst('J', "QUADRAMPDerivee", "ANGLE_QUAD_1ST_POS", "Vitesse max en rotation");
    addConst('K', "QUADRAMPDerivee", "ANGLE_QUAD_2ND_ACC", "Acceleration max en rotation");
    addConst('L', "QUADRAMPDerivee", "ANGLE_QUAD_2ND_DEC", "Deceleration max en rotation");
    addConst('M', "QUADRAMPDerivee", "ANGLE_QUAD_ANTICIPATION_GAIN_COEF", "Coeff determinant le debut de la rampe de deceleration en rotation");
    addConst('N', "QUADRAMPDerivee", "ANGLE_TAILLE_FENETRE_ARRIVEE", "Largeur de la zone ou l'on considere etre arrive (UO)");
}

// Crée un coeff
void DockConst::addConst(QChar p_id, QString p_categorie, QString p_nom, QString p_bulle, bool p_double)
{
    // Si la catégorie n’existe pas, on la crée
    if(m_categories.find(p_categorie) == m_categories.end())
    {
        m_categories[p_categorie] = new QGroupBox(p_categorie);
        QGridLayout* grid = new QGridLayout;
        m_categories[p_categorie]->setLayout(grid);
        m_layout->addWidget(m_categories[p_categorie]);
    }

    // On récupère le layout
    QGridLayout* grid = (QGridLayout*) m_categories[p_categorie]->layout();

    // si le coeff est un double, on crée un input double, sinon int
    if(p_double || true) // TODO
    {
        m_constantes[p_id] = new QDoubleSpinBox(this);
        ((QDoubleSpinBox*)m_constantes[p_id])->setMaximum(1e100);
        m_constantes[p_id]->setUserData(0, new QObjectUserData()); // On met une valeur particulière a userData[0] pour savoir plus tard que c’est un double
    }
    else
    {
        m_constantes[p_id] = new QSpinBox(this);
        m_constantes[p_id]->setUserData(0, NULL);
    }

    // On crée le label associé au input
    QLabel* label = new QLabel(p_nom, this);
    label->setToolTip(p_bulle);

    // On ajoute au layout le label et l’input
    grid->addWidget(label, grid->rowCount(), 0);
    grid->addWidget(m_constantes[p_id], grid->rowCount()-1, 1);
}

void DockConst::loadFrom(QFile* p_file)
{
    if (p_file->open(QFile::ReadOnly)) {
        QChar c;
        QChar espace;
        double v; // TODO : ce n’est pas toujours un double normalement
        QTextStream in(p_file);
        while(!in.atEnd())
        {
            in >> c >> espace;
            in >> v >> espace;
            setConstante(c,v);
        }
        p_file->close();
    }
    else
    {
        QMessageBox::critical(this, "Erreur", "Impossible de charger ce fichier.");
    }
}

void DockConst::saveIn(QFile* p_file)
{
    if (p_file->open(QFile::WriteOnly)) {
        QTextStream out(p_file);

        for(QMap<QChar, QWidget*>::iterator i = m_constantes.begin(); i != m_constantes.end(); i++)
        {
            if(i.value()->userData(0) != NULL || true) // Même TODO que celui en haut
            {
                out << i.key() << QChar(' ');
                out << (double)(((QDoubleSpinBox*) i.value())->value()) << QChar(' ');
            }
            else
            {
                out << i.key() << QChar(' ');
                out << (int)(((QSpinBox*) i.value())->value()) << QChar(' ');
            }
        }

        p_file->close();
    }
    else
    {
        QMessageBox::critical(this, "Erreur", "Impossible d’enregistrer le fichier sous ce nom.");
    }
}

// Modifie une constante entière
void DockConst::setConstante(QChar s, int v)
{
    ((QSpinBox*)m_constantes[s])->setValue(v);
}

// Modifie une constante double
void DockConst::setConstante(QChar s, double v)
{
    ((QDoubleSpinBox*)m_constantes[s])->setValue(v);
}

// Envoi tous les coeffs
void DockConst::envoyerTout()
{
    emit startEnvoi(); // On prévient qu’on commence l’envoi

    // On parcourt toute la map de coeffs pour les envoyer selon leur type enregistré dans userData[0]
    for(QMap<QChar, QWidget*>::iterator i = m_constantes.begin(); i != m_constantes.end(); i++)
    {
        if(i.value()->userData(0) != NULL)
        {
            emit constante(i.key(), (double)(((QDoubleSpinBox*) i.value())->value()));
        }
        else
        {
            emit constante(i.key(), (int)(((QSpinBox*) i.value())->value()));
        }
    }

    emit stopEnvoi(); // On prévient qu’on a fini l’envoi
}

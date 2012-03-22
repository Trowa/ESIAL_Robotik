#include "dockcurves.h"
#include <QFormLayout>
#include "checkboxvar.h"
#include <QLabel>
#include <QComboBox>
#include "curveform.h"

// Constructeur
DockCurves::DockCurves(QWidget* p_parent) : QDockWidget(p_parent)
{
    // On crée m_layout (où se trouvent tous les widgets)
    QWidget* contenu = new QWidget();
    this->setWidget(contenu);
    m_layout = new QFormLayout();
    contenu->setLayout(m_layout);
}

// Slot qui ajoute une donnée (et donc une case à cocher)
void DockCurves::addNewVar(QString p)
{
    CurveForm* cv = new CurveForm(p);

    m_layout->addRow(cv->getCheckBox(), cv->getColorList());

    connect(cv, SIGNAL(changeBox(QString, Qt::CheckState)), this, SLOT(changeBox(QString, Qt::CheckState)));
    connect(cv, SIGNAL(changeColor(QString,QString)), this, SLOT(changeColor(QString,QString)));

    cv->initColor();
}

// Quand on reçoit un signal d’une case à cocher, on remonte un signal pour prévenir le graphe
void DockCurves::changeBox(QString s, Qt::CheckState p)
{
    emit varChanged(s, p);
}

void DockCurves::changeColor(QString s, QString p_color)
{
    emit colorChanged(s,p_color);
}

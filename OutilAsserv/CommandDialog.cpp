#include "CommandDialog.h"

#include <QComboBox>
#include <QGridLayout>
#include <QPushButton>

#include <sstream>

CommandDialog::CommandDialog(QWidget* p_parent) : QDialog(p_parent),
    m_i(0)
{
    setModal(true);

    QPalette palette;
    palette.setColor(QPalette::Background, QColor(255,105,180));
    this->setPalette(palette);

    QGridLayout* grid = new QGridLayout;

    setLayout(grid);

    QComboBox* cB = new QComboBox(this);
    cB->addItem("Goto");
    cB->addItem("Goto Angle");
    cB->addItem("Go");
    cB->addItem("Turn");

    grid->addWidget(new QLabel("Commande", this), 0,0);
    grid->addWidget(cB,0,1);

    m_x = new QDoubleSpinBox(this);
    m_y = new QDoubleSpinBox(this);
    m_angle = new QDoubleSpinBox(this);
    m_distance = new QDoubleSpinBox(this);

    m_lX = new QLabel("X", this);
    m_lY = new QLabel("Y", this);
    m_lAngle = new QLabel("Angle", this);
    m_lDistance = new QLabel("Distance", this);

    grid->addWidget(m_lX, 1, 0);
    grid->addWidget(m_x, 1,1);

    grid->addWidget(m_lY, 2, 0);
    grid->addWidget(m_y, 2,1);

    grid->addWidget(m_lAngle, 3, 0);
    grid->addWidget(m_angle, 3,1);

    grid->addWidget(m_lDistance, 4, 0);
    grid->addWidget(m_distance, 4,1);

    m_angle->hide();
    m_distance->hide();
    m_lAngle->hide();
    m_lDistance->hide();

    connect(cB, SIGNAL(currentIndexChanged(int)), this, SLOT(onIndexChange(int)));

    QPushButton* button = new QPushButton("Envoyer", this);
    grid->addWidget(button, 5,1);
    connect(button, SIGNAL(clicked()), this, SLOT(accept()));
}

QString CommandDialog::getCommand()
{
    std::ostringstream flux;
    flux << m_i << '=';

    switch(m_i)
    {
        case 0 :
        case 1 :
            flux << m_x->value() << ';' << m_y->value();
            break;
        case 2 :
            flux << m_distance->value();
            break;
        case 3 :
            flux << m_angle->value();
            break;
    }

    return QString::fromStdString(flux.str());
}

void CommandDialog::onIndexChange(int i)
{
    m_angle->hide();
    m_distance->hide();
    m_x->hide();
    m_y->hide();
    m_lAngle->hide();
    m_lDistance->hide();
    m_lX->hide();
    m_lY->hide();

    m_i = i;

    switch(i)
    {
        case 0 :
        case 1 :
            m_lX->show();
            m_lY->show();
            m_x->show();
            m_y->show();
            break;

        case 2 :
            m_distance->show();
            m_lDistance->show();
            break;

        case 3 :
            m_angle->show();
            m_lAngle->show();
            break;
    }
}

QString CommandDialog::getCommand(QWidget *p_parent)
{
    CommandDialog cd(p_parent);
    QString string;

    if(QDialog::Accepted == cd.exec())
        string = cd.getCommand();

    return string;
 }

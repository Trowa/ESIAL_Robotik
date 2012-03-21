#include "curveform.h"

CurveForm::CurveForm(QString p_name) :
    m_check(p_name),
    m_name(p_name)
{


    m_cB.addItem("Noir");
    m_cB.setItemData(0,Qt::black,Qt::BackgroundRole);
    m_cB.addItem("Gris");
    m_cB.setItemData(1,Qt::gray,Qt::BackgroundRole);
    m_cB.addItem("Blanc");
    m_cB.setItemData(2,Qt::white,Qt::BackgroundRole);
    m_cB.addItem("Rouge");
    m_cB.setItemData(3,Qt::red,Qt::BackgroundRole);
    m_cB.addItem("Vert");
    m_cB.setItemData(4,Qt::green,Qt::BackgroundRole);
    m_cB.addItem("Bleu");
    m_cB.setItemData(5,Qt::blue,Qt::BackgroundRole);
    m_cB.addItem("Jaune");
    m_cB.setItemData(6,Qt::yellow,Qt::BackgroundRole);
    m_cB.addItem("Magenta");
    m_cB.setItemData(7,Qt::magenta,Qt::BackgroundRole);
    m_cB.addItem("Cyan");
    m_cB.setItemData(8,Qt::cyan,Qt::BackgroundRole);



    connect(&m_check, SIGNAL(stateChanged(QString, Qt::CheckState)), this, SLOT(onChangeBox(QString, Qt::CheckState)));
    connect(&m_cB, SIGNAL(currentIndexChanged(QString)), this, SLOT(onChangeColor(QString)));

    m_check.setChecked(false);
}

void CurveForm::initColor()
{
    static int iColor = 0;
    m_cB.setCurrentIndex(iColor++%9);

    emit changeColor(m_name, m_cB.currentText());
}

QWidget* CurveForm::getColorList()
{
    return &m_cB;
}

QWidget* CurveForm::getCheckBox()
{
    return &m_check;
}

void CurveForm::onChangeBox(QString p_name, Qt::CheckState state)
{
    emit changeBox(p_name, state);
}

void CurveForm::onChangeColor(QString p_color)
{
    emit changeColor(m_name, p_color);
}

QColor CurveForm::getColor(QString p_color)
{
    if(p_color=="Rouge")
        return Qt::red;
    else if(p_color=="Vert")
        return Qt::green;
    else if(p_color=="Bleu")
        return Qt::blue;
    else if(p_color=="Jaune")
        return Qt::yellow;
    else if(p_color=="Magenta")
        return Qt::magenta;
    else if(p_color=="Cyan")
        return Qt::cyan;
    else if(p_color=="Noir")
        return Qt::black;
    else if(p_color=="Gris")
        return Qt::gray;
    else if(p_color=="Blanc")
        return Qt::white;
}

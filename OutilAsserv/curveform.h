#ifndef CURVEFORM_H
#define CURVEFORM_H

#include <QWidget>
#include "checkboxvar.h"
#include <QComboBox>
#include <QString>


class CurveForm : public QObject
{
    Q_OBJECT

public:
    CurveForm(QString p_name);

    void initColor(); // On ne pourrait pas envoyer la couleur à la courbe si elle était initialisée dans le constructeur

    QWidget* getCheckBox();
    QWidget* getColorList();

    static QColor getColor(QString p_color);

public slots :
    void onChangeBox(QString, Qt::CheckState);
    void onChangeColor(QString);

signals :
    void changeColor(QString, QString);
    void changeBox(QString, Qt::CheckState);

private :
    CheckBoxVar m_check;
    QComboBox m_cB;
    QString m_name;
};

#endif // CURVEFORM_H

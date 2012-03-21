#ifndef CANVASPLOT_H
#define CANVASPLOT_H

#include "sfmlCanvas.h"
#include "curve.h"

class CanvasPlot : public SfmlCanvas
{
    Q_OBJECT

public :

    CanvasPlot(QWidget* Parent);

    // Méthode pour actualiser le temps
    void setTime(float);
    // Méthode pour ajouter un point (param1 : nom de la donnée, param2 : valeur)
    void appendPoint(const std::string &, double);


public slots :
    // Slot qui change l’état d’un graphe (affiché/caché)
    void changeVar(QString p_var, Qt::CheckState p_state);
    void changeColor(QString p_var, QString p_color);


signals :
    void newVar(QString); // Un signal envoyé lors de la création d’une nouvelle variable


private :

    virtual void onInit();

    virtual void onUpdate();

    float m_time;
    std::map<std::string, Curve*> m_curves;
};


#endif // CANVASPLOT_H

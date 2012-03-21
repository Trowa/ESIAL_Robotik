#include "canvasplot.h"

#include <SFML/Graphics.hpp>

CanvasPlot::CanvasPlot(QWidget* Parent) : SfmlCanvas(Parent), m_time(0)
{
    setCursor(Qt::BlankCursor);
}

// Méthode pour actualiser le temps
void CanvasPlot::setTime(float p_time)
{
    m_time = p_time;
}

// Méthode pour ajouter un point (param1 : nom de la donnée, param2 : valeur)
void CanvasPlot::appendPoint(const std::string & p, double p_value)
{
    // Si la courbe n’existe pas, on la crée
    if(m_curves.find(p) == m_curves.end())
    {
        m_curves[p] = new Curve(p, this->getDefaultView());

        emit newVar(QString::fromStdString(p)); // On prévient qu’on crée une nouvelle variable (pour actualiser le dock de courbes)
    }


    // Maintenant on est sûr que la courbe existe, on ajoute le point à la courbe qui correspond
    m_curves[p]->append(m_time, p_value);
}

void CanvasPlot::onInit()
{

}

void CanvasPlot::onUpdate()
{
    clear(sf::Color(255,105,180));


    // On update et on affiche chaque graphe
    for(std::map<std::string, Curve*>::iterator i = m_curves.begin(); i != m_curves.end(); i++)
    {
        i->second->update();
        i->second->draw(this);
    }

    int id=1;
    for(std::map<std::string, Curve*>::iterator i = m_curves.begin(); i != m_curves.end(); i++)
    {
        if(i->second->isDisplayed())
        {
            ++id;
            i->second->drawCursorText(this, id);
        }
    }
}

// L’état (affiché/caché) d’une var a changé, on attache/détache la courbe
void CanvasPlot::changeVar(QString p_var, Qt::CheckState p_state)
{
    if(p_state == Qt::Unchecked)
    {
        m_curves[p_var.toStdString()]->display(false);
    }
    else
    {
        m_curves[p_var.toStdString()]->display(true);
    }
}

// La couleur d’une var a changé, on actualise la courbe
void CanvasPlot::changeColor(QString p_var, QString p_color)
{
    m_curves[p_var.toStdString()]->setColor(p_color.toStdString());
}

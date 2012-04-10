#include "canvasplot.h"

#include <SFML/Graphics.hpp>
#include <QMouseEvent>

CanvasPlot::CanvasPlot(QWidget* Parent) : SfmlCanvas(Parent), m_time(0), m_mouseClick(false)
{

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


    sf::VertexArray cursor(sf::Lines, 4);
    cursor[0].position.x = (float)sf::Mouse::getPosition(*this).x - this->getSize().x*2;
    cursor[0].position.y = (float)sf::Mouse::getPosition(*this).y;
    cursor[1].position.x = (float)sf::Mouse::getPosition(*this).x + this->getSize().x*2;
    cursor[1].position.y = (float)sf::Mouse::getPosition(*this).y;
    cursor[2].position.x = (float)sf::Mouse::getPosition(*this).x;
    cursor[2].position.y = (float)sf::Mouse::getPosition(*this).y - this->getSize().y*2;
    cursor[3].position.x = (float)sf::Mouse::getPosition(*this).x;
    cursor[3].position.y = (float)sf::Mouse::getPosition(*this).y + this->getSize().y*2;

    this->setView(this->getDefaultView());

    int k=0;
    for(std::map<std::string, Curve*>::iterator i = m_curves.begin(); i != m_curves.end(); i++)
    {
        if(i->second->isDisplayed())
        {
            if(k==0) i->second->drawScaleT(this);
            i->second->drawScale(this, k);
            ++k;
        }
    }

    if(m_mouseClick)
    {
        sf::VertexArray array(sf::TrianglesFan, 4);
        array[0].position.x = m_lastPoint.x();
        array[0].position.y = m_lastPoint.y();
        array[0].color = sf::Color(255,255,255,128);

        array[1].position.x = m_lastPoint.x();
        array[1].position.y = m_movingPoint.y();
        array[1].color = sf::Color(255,255,255,128);

        array[2].position.x = m_movingPoint.x();
        array[2].position.y = m_movingPoint.y();
        array[2].color = sf::Color(255,255,255,128);

        array[3].position.x = m_movingPoint.x();
        array[3].position.y = m_lastPoint.y();
        array[3].color = sf::Color(255,255,255,128);



        sf::View tmp;
        tmp = this->getView();
        this->setView(this->getDefaultView());
        this->draw(array);
        this->setView(tmp);
    }

    this->draw(cursor);
}

void CanvasPlot::mouseReleaseEvent ( QMouseEvent * e )
{
    if(m_mouseClick)
    {
        // Si la souris a bougé
        if((e->pos() != m_lastPoint) && e->button() == Qt::LeftButton)
        {
            // <IOCCC>
            float maxY, maxT;
            float minY = (m_lastPoint.y()<e->pos().y())?(maxY=m_lastPoint.y(),e->pos().y()):(maxY=e->pos().y(),m_lastPoint.y()); // Pas logique, mais c’est parce que SFML a un axe vertical descendant
            float minT = (m_lastPoint.x()>e->pos().x())?(maxT=m_lastPoint.x(),e->pos().x()):(maxT=e->pos().x(),m_lastPoint.x());
            // </IOCCC>


            for(std::map<std::string, Curve*>::iterator i = m_curves.begin(); i != m_curves.end(); i++)
            {
                sf::Vector2f mins = convertCoords(sf::Vector2i(minT, minY), i->second->getView());
                sf::Vector2f maxs = convertCoords(sf::Vector2i(maxT, maxY), i->second->getView());

                Scale::setMinMaxT(mins.x, maxs.x);
                Scale::setDeltaT(maxs.x-mins.x);
                i->second->updateScale(mins.y, maxs.y);
            }
        }
        else
        {
            Scale::setMinMaxT(0, m_time);
            Scale::setDeltaT(90);
            for(std::map<std::string, Curve*>::iterator i = m_curves.begin(); i != m_curves.end(); i++)
            {
                i->second->updateScale(i->second->getMin(0,m_time), i->second->getMax(0,m_time));
            }
        }
    }

    m_mouseClick = false;
}

void CanvasPlot::mouseMoveEvent(QMouseEvent * e)
{
    if(m_mouseClick)
    {
        m_movingPoint = e->pos();
    }
}

void CanvasPlot::mousePressEvent ( QMouseEvent * e )
{
    m_lastPoint = e->pos();
    m_movingPoint = m_lastPoint;
    m_mouseClick = true;
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

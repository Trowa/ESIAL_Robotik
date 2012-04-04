#include "curve.h"
#include <sstream>
#include <cfloat>


Curve::Curve(std::string p_nom, const sf::View& p_view) :
    m_vertexArray(sf::LinesStrip, 0),
    m_view(p_view),

    m_display(false)
{

}

bool Curve::isDisplayed() const
{
    return m_display;
}

void Curve::updateScale(float minY, float maxY)
{
    m_scale.setMinMax(minY, maxY);
    update();
}

const sf::View& Curve::getView() const
{
    return m_view;
}

float Curve::getMin(float p_tMin, float p_tMax) const
{
    float min = FLT_MAX;
    for(int i=0; i<m_vertexArray.getVertexCount(); ++i)
    {
        if(m_vertexArray[i].position.x >= p_tMin && m_vertexArray[i].position.x <= p_tMax)
        {
            if(m_vertexArray[i].position.y < min) min = m_vertexArray[i].position.y;
        }
    }
    return min;
}

float Curve::getMax(float p_tMin, float p_tMax) const
{
    float max = -FLT_MAX;
    for(int i=0; i<m_vertexArray.getVertexCount(); ++i)
    {
        if(m_vertexArray[i].position.x >= p_tMin && m_vertexArray[i].position.x <= p_tMax)
        {
            if(m_vertexArray[i].position.y > max) max = m_vertexArray[i].position.y;
        }
    }
    return max;
}

void Curve::update()
{
    m_view.setCenter(m_scale.getCenter());
    m_view.setSize(m_scale.getSize());
}

void Curve::draw(sf::RenderTarget* p_target) const
{
    if(m_display)
    {
        p_target->setView(m_view);
        p_target->draw(m_vertexArray);

        p_target->setView(p_target->getDefaultView());
    }
}

void Curve::drawScale(sf::RenderWindow* p_window, int p_k) const
{
    if(m_display)
    {
        p_window->setView(m_view);
        sf::Transform t = sf::Transform::Identity;
        t.translate((p_k%2==0)?(p_k/2)*40.f:p_window->getSize().x-(p_k/2+1)*40.f, 0);
        p_window->draw(m_scale, t);
        p_window->setView(p_window->getDefaultView());
    }
}

void Curve::drawScaleT(sf::RenderWindow* p_window) const
{
    if(m_display)
    {
        p_window->setView(m_view);
        Scale::drawScaleT(*p_window);
        p_window->setView(p_window->getDefaultView());
    }
}

void Curve::display(bool p)
{
    m_display = p;
}

void Curve::setColor(const std::string& p)
{
    if(p=="Rouge")
        m_color = sf::Color::Red;
    else if(p=="Vert")
        m_color = sf::Color::Green;
    else if(p=="Bleu")
        m_color = sf::Color::Blue;
    else if(p=="Jaune")
        m_color = sf::Color::Yellow;
    else if(p=="Magenta")
        m_color = sf::Color::Magenta;
    else if(p=="Cyan")
        m_color = sf::Color::Cyan;
    else if(p=="Noir")
        m_color = sf::Color::Black;
    else if(p=="Gris")
        m_color = sf::Color(128,128,128);
    else if(p=="Blanc")
        m_color = sf::Color::White;

    for(int i=0; i<m_vertexArray.getVertexCount(); ++i)
    {
        m_vertexArray[i].color = m_color;
    }
    m_scale.setColor(m_color);
}

void Curve::append(float p_time, float p_info)
{
    sf::Vertex vertex;
    vertex.position.x = p_time;
    vertex.position.y = p_info;
    vertex.color = m_color;
    m_vertexArray.append(vertex);

    m_scale.update(p_time, p_info);
}

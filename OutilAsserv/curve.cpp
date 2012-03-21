#include "curve.h"
#include <cfloat>
#include <sstream>



float Curve::m_minT = FLT_MAX;
float Curve::m_maxT = 1;
float Curve::m_deltaT = 90;

Curve::Curve(std::string p_nom, const sf::View& p_view) :
    m_vertexArray(sf::LinesStrip, 0),
    m_view(p_view),
    m_minV(FLT_MAX),
    m_maxV(-FLT_MAX),
    m_display(false)
{

}

bool Curve::isDisplayed() const
{
    return m_display;
}

void Curve::update()
{
    m_view.setCenter((m_maxT-m_minT<m_deltaT/2.f*(1+0.8f))?m_minT+m_deltaT/2.f:m_maxT-(m_deltaT/2.f)*0.8, (m_maxV+m_minV)/2.f);
    m_view.setSize(m_deltaT, (m_maxV-m_minV)*1.2f);
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

void Curve::drawCursorText(sf::RenderWindow* p_window, int id) const
{
    sf::Vector2i v = sf::Mouse::getPosition(*p_window);
    sf::Vector2f coordonnees = p_window->convertCoords(v.x, v.y, m_view);

    std::ostringstream flux;
    flux << coordonnees.y;
    sf::Text t(flux.str());
    t.setColor(m_color);
    t.setScale(0.4f, 0.4f);
    t.setPosition(v.x, v.y-13*id);
    p_window->draw(t);
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
}

void Curve::append(float p_time, float p_info)
{
    sf::Vertex vertex;
    vertex.position.x = p_time;
    vertex.position.y = p_info;
    vertex.color = m_color;
    m_vertexArray.append(vertex);

    if(m_minV > p_info) m_minV = p_info;
    if(m_maxV < p_info) m_maxV = p_info;

    if(m_maxT < p_time) m_maxT = p_time;
    if(m_minT > p_time) m_minT = p_time;
}

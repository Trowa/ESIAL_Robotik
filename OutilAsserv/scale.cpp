#include "scale.h"

#include <sstream>
#include <cmath>
#include <cfloat>

float Scale::m_minT = FLT_MAX;
float Scale::m_maxT = -FLT_MAX;
float Scale::m_deltaT = 90;

Scale::Scale() :
    m_min(FLT_MAX),
    m_max(-FLT_MAX)
{

}

void Scale::setMinMaxT(float min, float max)
{
    m_minT = min;
    m_maxT = max;
}

void Scale::setMinMax(float min, float max)
{
    m_min = min;
    m_max = max;
}

float Scale::getMin()
{
    return m_min;
}

float Scale::getMax()
{
    return m_max;
}

float Scale::getMinT()
{
    return m_minT;
}

float Scale::getMaxT()
{
    return m_maxT;
}

void Scale::setDeltaT(float p)
{
    m_deltaT = p;
}

sf::Vector2f Scale::getCenter()
{
    return sf::Vector2f((m_maxT-m_minT<m_deltaT)?m_minT+m_deltaT/2.f:m_maxT-m_deltaT/2.f, (m_max+m_min)/2.f);
}

sf::Vector2f Scale::getSize()
{
    return sf::Vector2f(m_deltaT*1.2f, (m_min-m_max)*1.2f); // Pas logique mais c’est parce que SFML a un axe descendant.
}

void Scale::update(float p_time, float p_info)
{
    if(m_min > p_info) m_min = p_info;
    if(m_max < p_info) m_max = p_info;

    if(m_maxT < p_time) m_maxT = p_time;
    if(m_minT > p_time) m_minT = p_time;
}

void Scale::setColor(const sf::Color& p_color)
{
    m_color = p_color;
}

void Scale::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::View tmp_view = target.getView();
    sf::VertexArray array(sf::Lines, 400);

    int logDelta = floor(log10(m_max - m_min));
    float delta = pow(10, logDelta);

    float minScale = 10*delta*floor(m_min/(10*delta));

    sf::Text t;
    std::ostringstream flux;
    t.setColor(m_color);

    target.setView(target.getDefaultView());

    float top = target.getViewport(tmp_view).top;
    float height = (float)target.getViewport(tmp_view).height;

    for(int i=0; i<200; ++i)
    {
        float y = minScale + i*delta/10.f;
        // Inverse de la formule trouvée dans SFML/Graphics/RenderTarget.cpp -> convertCoords
        float yy = top + height * (1.f-tmp_view.getTransform().transformPoint(0.f,y).y)/2.f;

        array[i*2].position.x = 5;
        array[i*2].position.y = yy;
        array[i*2+1].position.x = 5 + ((i%10==0)?10:((i%10==5)?5:2));
        array[i*2+1].position.y = yy;

        array[i*2].color = m_color;
        array[i*2+1].color = m_color;

        if(i%5==0)
        {
            if(i%2==0) t.setScale(0.4f, 0.4f);
            else t.setScale(0.35f, 0.35f);

            flux.str("");
            flux << y;
            t.setString(flux.str());

            t.setPosition(10, yy);
            target.draw(t, states);
        }
    }

    target.draw(array, states);
}

void Scale::drawScaleT(sf::RenderTarget& target, sf::RenderStates states)
{
    sf::View tmp_view = target.getView();
    sf::VertexArray array(sf::Lines, 400);

    int logDelta = floor(log10(tmp_view.getSize().x));
    float delta = pow(10, logDelta);

    float minScale = 10*delta*floor(m_minT/(10*delta));

    sf::Text t;
    std::ostringstream flux;
    t.setColor(sf::Color::Black);

    target.setView(target.getDefaultView());

    float bottom = target.getViewport(tmp_view).top + target.getViewport(tmp_view).height;
    float left = target.getViewport(tmp_view).left;
    float width = (float)target.getViewport(tmp_view).width;

    for(int i=0; i<200; ++i)
    {
        float x = minScale + i*delta/10.f;
        // Inverse de la formule trouvée dans SFML/Graphics/RenderTarget.cpp -> convertCoords
        float xx = left + width * (1+tmp_view.getTransform().transformPoint(x, 0.f).x)/2.f;

        array[i*2].position.x = xx;
        array[i*2].position.y = bottom - (5+((i%10==0)?10:((i%10==5)?5:2)));
        array[i*2+1].position.x = xx;
        array[i*2+1].position.y = bottom;

        array[i*2].color = sf::Color::Black;
        array[i*2+1].color = sf::Color::Black;


        if(i%5==0)
        {
            if(i%2==0) t.setScale(0.35f, 0.35f);
            else t.setScale(0.3f, 0.3f);

            flux.str("");
            flux << x;
            t.setString(flux.str());

            t.setPosition(xx, bottom-30);
            target.draw(t, states);
        }
    }

    target.draw(array, states);
}

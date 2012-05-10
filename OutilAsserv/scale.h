#ifndef SCALE_H
#define SCALE_H

#include <SFML/Graphics.hpp>

#include "sfmlCanvas.h"

class Scale
{
public:
    Scale();
    float getMin();
    float getMax();
    static float getMinT();
    static float getMaxT();

    void setMinMax(float min, float max);
    static void setMinMaxT(float min, float max);

    static void setDeltaT(float p);
    void update(float p_time, float p_info);
    void setColor(const sf::Color& p_color);

    sf::Vector2f getCenter();
    sf::Vector2f getSize();

    virtual void draw(SfmlCanvas& target, sf::RenderStates states = sf::RenderStates::Default) const;
    static void drawScaleT(SfmlCanvas& target, sf::RenderStates states = sf::RenderStates::Default);

private :
    float m_min;
    float m_max;
    static float m_minT;
    static float m_maxT;
    static float m_deltaT;
    sf::Color m_color;
};

#endif // SCALE_H

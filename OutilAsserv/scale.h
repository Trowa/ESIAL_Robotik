#ifndef SCALE_H
#define SCALE_H

#include <SFML/Graphics.hpp>

class Scale : public sf::Drawable
{
public:
    Scale();
    float getMin();
    float getMax();
    float getMinT();
    float getMaxT();

    void setDeltaT(float p);
    void update(float p_time, float p_info);
    void setColor(const sf::Color& p_color);

    sf::Vector2f getCenter();
    sf::Vector2f getSize();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    static void drawScaleT(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default);

private :
    float m_min;
    float m_max;
    static float m_minT;
    static float m_maxT;
    static float m_deltaT;
    sf::Color m_color;
};

#endif // SCALE_H

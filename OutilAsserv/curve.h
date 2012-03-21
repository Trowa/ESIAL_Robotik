#ifndef CURVE_H
#define CURVE_H

#include <SFML/Graphics.hpp>
#include <vector>

class Curve
{
public:
    Curve(std::string p_nom, const sf::View& p_view);

    void update();
    void draw(sf::RenderTarget* p_target) const;
    void drawCursorText(sf::RenderWindow* p_window, int id) const;

    void display(bool p);
    bool isDisplayed() const;

    void setColor(const std::string& p);

    void append(float p_time, float p_info);

private:
    bool m_display;
    sf::VertexArray m_vertexArray;
    sf::Color m_color;
    sf::View m_view;
    float m_minV;
    float m_maxV;
    static float m_minT;
    static float m_maxT;
    static float m_deltaT;
};

#endif // CURVE_H

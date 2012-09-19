#ifndef CURVE_H
#define CURVE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "scale.h"

class Curve
{
public:
    Curve(std::string p_nom, const sf::View& p_view);

    void update();
    void draw(SfmlCanvas* p_target) const;
    void drawScale(SfmlCanvas* p_window, int p_k) const;
    void drawScaleT(SfmlCanvas* p_window) const;

    void updateScale(float minY, float maxY);

    void display(bool p);
    bool isDisplayed() const;

    const sf::View& getView() const;

    float getMin(float p_tMin, float p_tMax) const;
    float getMax(float p_tMin, float p_tMax) const;

    void setColor(const std::string& p);

    void append(float p_time, float p_info);

private:
    bool m_display;
    sf::VertexArray m_vertexArray;
    sf::Color m_color;
    sf::View m_view;
    Scale m_scale;
};

#endif // CURVE_H

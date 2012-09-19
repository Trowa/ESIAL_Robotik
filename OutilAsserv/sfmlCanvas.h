#ifndef SFMLPLOT_H
#define SFMLPLOT_H

#include <QWidget>
#include <QTimer>
#include <SFML/Graphics.hpp>

class SfmlCanvas : public QWidget, public sf::RenderWindow
{
    Q_OBJECT
public:
    SfmlCanvas(QWidget* Parent);
    virtual ~SfmlCanvas();

    const sf::View& getMyView();
    
signals:
    
public slots:

private :

    virtual void onInit() = 0;

    virtual void onUpdate() = 0;

    virtual void showEvent(QShowEvent*);

    QPaintEngine* paintEngine() const;
    virtual void paintEvent(QPaintEvent*);

    virtual void resizeEvent ( QResizeEvent * event );

    bool myInitialized;
    QTimer myTimer;
    sf::View m_defaultView;
};

#endif // SFMLPLOT_H

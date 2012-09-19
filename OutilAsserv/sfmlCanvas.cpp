#include "sfmlCanvas.h"
#include <QResizeEvent>

SfmlCanvas::SfmlCanvas(QWidget* Parent) :
    QWidget       (Parent),
    myInitialized (false),
    m_defaultView(this->getDefaultView().getCenter(), this->getDefaultView().getSize())
{
    // Mise en place de quelques options pour autoriser le rendu direct dans le widget
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
/*
    m_defaultView.setViewport(this->getDefaultView().getViewport());
    setView(m_defaultView);
*/

    myTimer.setInterval(60);
}

SfmlCanvas::~SfmlCanvas()
{
    myTimer.stop();
}

#ifdef Q_WS_X11
    #include <Qt/qx11info_x11.h>
    #include <X11/Xlib.h>
#endif

void SfmlCanvas::showEvent(QShowEvent*)
{
    if (!myInitialized)
    {
        // Sous X11, il faut valider les commandes qui ont été envoyées au serveur
        // afin de s'assurer que SFML aura une vision à jour de la fenêtre
        #ifdef Q_WS_X11
            XFlush(QX11Info::display());
        #endif

        // On crée la fenêtre SFML avec l'identificateur du widget
        sf::RenderWindow::create(winId());

        // On laisse la classe dérivée s'initialiser si besoin
        onInit();

        // On paramètre le timer de sorte qu'il génère un rafraîchissement à la fréquence souhaitée
        connect(&myTimer, SIGNAL(timeout()), this, SLOT(repaint()));
        myTimer.start();

        myInitialized = true;
    }
}

const sf::View& SfmlCanvas::getMyView()
{
    return this->getDefaultView();
    return m_defaultView;
}

QPaintEngine* SfmlCanvas::paintEngine() const
{
    return 0;
}

void SfmlCanvas::paintEvent(QPaintEvent*)
{
    m_defaultView.setSize(getSize().x, getSize().y);

    // On laisse la classe dérivée faire sa tambouille
    onUpdate();

    // On rafraîchit le widget
    display();
}

void SfmlCanvas::resizeEvent ( QResizeEvent * event )
{
    // Sous X11, il faut valider les commandes qui ont été envoyées au serveur
    // afin de s'assurer que SFML aura une vision à jour de la fenêtre
    #ifdef Q_WS_X11
        XFlush(QX11Info::display());
    #endif

    // On crée la fenêtre SFML avec l'identificateur du widget
    sf::RenderWindow::create(winId());
}

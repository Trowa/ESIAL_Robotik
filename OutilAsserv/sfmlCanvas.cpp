#include "sfmlCanvas.h"

SfmlCanvas::SfmlCanvas(QWidget* Parent) :
QWidget       (Parent),
myInitialized (false)
{
    // Mise en place de quelques options pour autoriser le rendu direct dans le widget
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);

    myTimer.setInterval(60);
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

void SfmlCanvas::paintEvent(QPaintEvent*)
{
    // On laisse la classe dérivée faire sa tambouille
    onUpdate();

    // On rafraîchit le widget
    display();
}

void SfmlCanvas::resizeEvent ( QResizeEvent * event )
{
    #ifdef Q_WS_X11
        XFlush(QX11Info::display());
    #endif

    sf::RenderWindow::create(winId());
}

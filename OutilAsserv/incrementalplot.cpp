#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_canvas.h>
#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_symbol.h>
#include <qwt/qwt_plot_directpainter.h>
#include "incrementalplot.h"
#include <qpaintengine.h>
#include <qwt/qwt_scale_widget.h>
#include <qwt/qwt_plot_grid.h>

#include <qwt/qwt_legend.h>
#include <iostream>
#include "curveform.h"


/* ********************************************************************
  Classe qui n’a rien à faire ici… Mais c’est plus simple comme ça :p
  ******************************************************************** */

// Classe qui correspond à l’ensemble des données d’un graphe
class CurveData: public QwtArraySeriesData<QPointF>
{
    public:
        // Constructeur
        CurveData()
        {
            d_boundingRect = QRectF( 0.0, 0.0, 90.0, 10 );
        }

        // Retourne le rectangle qui doit être inclut (et donc définit indirectement les axes)
        virtual QRectF boundingRect() const
        {
            // 0s -> 90s pour le temps, le reste se met à jour en fonction des données, mais avec un arrondi pour éviter de changer d’échelle à chaque point
            return QRectF(d_boundingRect.width()>100?d_boundingRect.width()-100:0, floor(d_boundingRect.top()/100)*100, d_boundingRect.width(), ceil(d_boundingRect.height()/10)*10);
        }

        // Ajoute un point
        inline void append( const QPointF &point )
        {
            // On met à jour les min et max des axes
            if(point.x() < d_boundingRect.left()) d_boundingRect.setLeft(point.x());
            if(point.x() > d_boundingRect.right()) d_boundingRect.setRight(point.x());
            if(point.y() < d_boundingRect.top()) d_boundingRect.setTop(point.y());
            if(point.y() > d_boundingRect.bottom()) d_boundingRect.setBottom(point.y());

            // On ajoute le point
            d_samples += point;
        }

        // Efface le graphe
        void clear()
        {
            d_samples.clear();
            d_samples.squeeze();
            d_boundingRect = QRectF( 0.0, 0.0, 90.0, 10 );
        }
};


/* ********************************************************************
       La vraie classe du fichier : celle qui gère tous les graphes
  ******************************************************************** */

// Constructeur
IncrementalPlot::IncrementalPlot(QWidget *parent):
    QwtPlot(parent),
    m_currentTime(0)
{
    m_directPainter = new QwtPlotDirectPainter(this);

    // On met la légende à droite
    insertLegend(new QwtLegend(), QwtPlot::RightLegend);

    // On met le fond en rose (mais un rose un peu différent du reste de la fenêtre)
    setCanvasBackground(QColor(QColor(255,125,200)));

    // On désactive l’autoreplot (on veut dire exactement nous mêmes quand on redessine les graphes)
    setAutoReplot(false);
}

// Destructeur
IncrementalPlot::~IncrementalPlot()
{
    // On supprime chaque graphe
    for(QMap<QString, QwtPlotCurve*>::iterator i = m_curves.begin(); i != m_curves.end(); i++)
    {
        delete i.value();
    }
}

// Met à jour le temps
void IncrementalPlot::setTime(float p)
{
    m_currentTime = p;
}

// Ajoute un point (p : nom de la donnée, et donc du graphe, p_info : valeur)
void IncrementalPlot::appendPoint(const QString& p, double p_info)
{
    // Si la courbe n’existe pas, on la crée
    if(m_curves.find(p) == m_curves.end())
    {
        emit newVar(p); // On prévient qu’on crée une nouvelle variable (pour actualiser le dock de courbes)

        m_curves[p] = new QwtPlotCurve(p);
        m_curves[p]->setStyle(QwtPlotCurve::Lines); // On relie les points par un trait
        m_curves[p]->setData( new CurveData() );

        // On crée une couleur
        QColor c;

        // On définit le type et la taille des points (croix), la couleur des traits, etc
        m_curves[p]->setSymbol(new QwtSymbol(QwtSymbol::Diamond, Qt::NoBrush, QPen(c), QSize(2, 2)));

        // On attache la courbe au graphe
        m_curves[p]->attach(this);
    }


    // Maintenant on est sûr que la courbe existe, on ajoute le point à la courbe qui correspond
    CurveData *data = static_cast<CurveData*>(m_curves[p]->data());
    data->append(QPointF(m_currentTime, p_info));

    // On redéfinit ce qu’il faut dessiner (???)
    m_directPainter->drawSeries(m_curves[p], data->size()-1, data->size()-1);

    // On redessine
    replot();
}

// Efface les points de toutes les courbes
void IncrementalPlot::clearPoints()
{
    for(QMap<QString, QwtPlotCurve*>::iterator i = m_curves.begin(); i != m_curves.end(); i++)
    {
        CurveData *data = static_cast<CurveData*>(i.value()->data());
        data->clear();
    }

    // Redessine
    replot();
}

// L’état (affiché/caché) d’une var a changé, on attache/détache la courbe
void IncrementalPlot::changeVar(QString p_var, Qt::CheckState p_state)
{
    if(p_state == Qt::Unchecked)
    {
        m_curves[p_var]->detach();
    }
    else
    {
        m_curves[p_var]->attach(this);
    }

    // On redessine
    replot();
}

// La couleur d’une var a changé, on actualise la courbe
void IncrementalPlot::changeColor(QString p_var, QString p_color)
{
    QColor c = CurveForm::getColor(p_color);

    m_curves[p_var]->setPen(QPen(c));
    m_curves[p_var]->setSymbol(new QwtSymbol(QwtSymbol::Diamond, Qt::NoBrush, QPen(c), QSize(2, 2)));

    // On redessine
    replot();
}

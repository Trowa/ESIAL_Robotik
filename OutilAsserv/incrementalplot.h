#ifndef _INCREMENTALPLOT_H_
#define _INCREMENTALPLOT_H_ 1

/* ****************************************************************
                        ZONE GRAPHES
 **************************************************************** */

// On inclut la classe mère
#include <qwt/qwt_plot.h>

// … et une classe utile : la qmap
#include <qmap.h>

// On déclare les classes des attributs
class QwtPlotCurve;
class QwtPlotDirectPainter;

class IncrementalPlot : public QwtPlot
{
    // Ligne obligatoire pour créer ses propres signaux/slots (ie événements/listeners)
    Q_OBJECT

    public:
        // Constructeur/Destructeur
        IncrementalPlot(QWidget *parent = NULL);
        virtual ~IncrementalPlot();

        // Méthode pour actualiser le temps
        void setTime(float);
        // Méthode pour ajouter un point (param1 : nom de la donnée, param2 : valeur)
        void appendPoint(const QString &, double);

        // Méthode pour tout effacer
        void clearPoints(); // (<- Cette méthode est inutile car jamais appelée, pour le moment)

    public slots :
        // Slot qui change l’état d’un graphe (affiché/caché)
        void changeVar(QString p_var, Qt::CheckState p_state);
        void changeColor(QString p_var, QString p_color);


    signals :
        void newVar(QString); // Un signal envoyé lors de la création d’une nouvelle variable

    private:
        float m_currentTime; // Temps actuel (utilisé lors d’un ajout d’un point)
        QMap<QString, QwtPlotCurve*> m_curves; // Map associant à un nom de donnée un graphe
        QwtPlotDirectPainter *m_directPainter; // ???
};

#endif // _INCREMENTALPLOT_H_

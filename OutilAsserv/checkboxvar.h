#ifndef CHECKBOXVAR_H
#define CHECKBOXVAR_H

/* ****************************************************************
            CASE A COCHER (pour afficher/cacher un graphe)
 **************************************************************** */

// On inclut la classe mère
#include <QCheckBox>

class CheckBoxVar : public QCheckBox
{
    // Ligne obligatoire pour créer ses propres signaux/slots (ie événements/listeners)
    Q_OBJECT

    public:
        // Constructeur
        CheckBoxVar(const QString& p_s);

    public slots :
        // Slot appelé lorsqu’on coche/décoche la case (avec comme paramètre le nouvel état : 0 pour décoché, 2 pour coché)
        void changeState(int p);

    signals :
        // Signal reçu lorsqu’on coche/décoche la case
        void stateChanged(QString, Qt::CheckState);
};

#endif // CHECKBOXVAR_H

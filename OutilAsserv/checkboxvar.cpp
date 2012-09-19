#include "checkboxvar.h"

// Constructeur
CheckBoxVar::CheckBoxVar(const QString& p_s) :
    QCheckBox(p_s)
{
    // Par défaut, la case est cochée
    setChecked(true);

    // On connecte le signal au slot
    connect(this, SIGNAL(stateChanged(int)), this, SLOT(changeState(int)));
}


// Le slot renvoie juste un nouveau signal (qui comporte le nom de la case et son état)
void CheckBoxVar::changeState(int p)
{
    emit stateChanged(this->text(), (Qt::CheckState) p);
}

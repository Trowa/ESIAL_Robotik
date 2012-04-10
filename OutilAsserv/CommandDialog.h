#ifndef COMMANDDIALOG_H
#define COMMANDDIALOG_H

#include <QDialog>
#include <QDoubleSpinBox>
#include <QLabel>

class CommandDialog : public QDialog
{
    Q_OBJECT

    public:
            CommandDialog(QWidget * p_parent = NULL);

            QString getCommand();

            static QString getCommand(QWidget * p_parent);

    public slots :
            void onIndexChange(int i);

    private:
            int m_i;
            QString m_string;
            QDoubleSpinBox* m_x;
            QDoubleSpinBox* m_y;
            QDoubleSpinBox* m_angle;
            QDoubleSpinBox* m_distance;
            QLabel* m_lX; // Label x
            QLabel* m_lY; // Label y
            QLabel* m_lAngle; // Label angle
            QLabel* m_lDistance;
};

#endif // COMMANDDIALOG_H

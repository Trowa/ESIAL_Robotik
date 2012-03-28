#include <QtGui/QApplication>
#include "mainwindow.h"

#include <cmath>
#include <iostream>

int main(int argc, char *argv[])
{
    // Classique de Qt…
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

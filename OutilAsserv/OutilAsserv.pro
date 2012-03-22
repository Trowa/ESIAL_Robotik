#-------------------------------------------------
#
# Project created by QtCreator 2011-11-18T20:26:38
#
#-------------------------------------------------

QT       += core gui network

TARGET = OutilAsserv
TEMPLATE = app


SOURCES += main.cpp\
    server.cpp \
    mainwindow.cpp \
    dockcurves.cpp \
    checkboxvar.cpp \
    dockconst.cpp \
    CommandDialog.cpp \
    curveform.cpp \
    sfmlCanvas.cpp \
    canvasplot.cpp \
    curve.cpp

HEADERS  += \
    server.h \
    mainwindow.h \
    dockcurves.h \
    checkboxvar.h \
    dockconst.h \
    CommandDialog.h \
    curveform.h \
    sfmlCanvas.h \
    canvasplot.h \
    curve.h

CONFIG += qwt

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../usr/local/qwt-6.0.1/lib/release/ -lqwt -lsfml-graphics -lsfml-window -lsfml-system
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../usr/local/qwt-6.0.1/lib/debug/ -lqwt -lsfml-graphics -lsfml-window -lsfml-system
else:symbian: LIBS += -lqwt -lsfml-graphics -lsfml-window -lsfml-system
else:unix: LIBS += -L$$PWD/../../../../../../usr/local/qwt-6.0.1/lib/ -lqwt -lsfml-graphics -lsfml-window -lsfml-system -lX11

INCLUDEPATH += $$PWD/../../../../../../usr/local/qwt-6.0.1/include
DEPENDPATH += $$PWD/../../../../../../usr/local/qwt-6.0.1/include




























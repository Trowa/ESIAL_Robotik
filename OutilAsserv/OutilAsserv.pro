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
    curve.cpp \
    scale.cpp

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
    curve.h \
    scale.h

win32:CONFIG(release, debug|release): LIBS += -lsfml-graphics -lsfml-window -lsfml-system
else:win32:CONFIG(debug, debug|release): LIBS += -lsfml-graphics -lsfml-window -lsfml-system
else:symbian: LIBS += -lsfml-graphics -lsfml-window -lsfml-system
else:unix: LIBS += -lsfml-graphics -lsfml-window -lsfml-system -lX11























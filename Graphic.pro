#-------------------------------------------------
#
# Project created by QtCreator 2014-08-28T20:12:35
#
#-------------------------------------------------

QT       += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Graphic
TEMPLATE = app


SOURCES += \
    basicgraphs.cpp \
    canvasscene.cpp \
    canvasview.cpp \
    colourfillcontroller.cpp \
    colourlinecontroller.cpp \
    cornergrabber.cpp \
    edge.cpp \
    graph.cpp \
    graphmimedata.cpp \
    html-label.cpp \
    labelcontroller.cpp \
    labelsizecontroller.cpp \
    main.cpp \
    mainwindow.cpp \
    node.cpp \
    preview.cpp \
    sizecontroller.cpp

HEADERS += \
    basicgraphs.h \
    canvasscene.h \
    canvasview.h \
    colourfillcontroller.h \
    colourlinecontroller.h \
    cornergrabber.h \
    defuns.h \
    edge.h \
    graph.h \
    graphmimedata.h \
    html-label.h \
    labelcontroller.h \
    labelsizecontroller.h \
    mainwindow.h \
    node.h \
    preview.h \
    sizecontroller.h


FORMS += mainwindow.ui

CONFIG += c++11 \
    thread


RESOURCES = application.qrc

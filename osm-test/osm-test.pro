#-------------------------------------------------
#
# Project created by QtCreator 2015-09-06T15:14:23
#
#-------------------------------------------------

QT       += core gui sql
qtHaveModule(printsupport): QT += printsupport
qtHaveModule(opengl): QT += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = osm-test
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    nodeassociatedtoway.cpp \
    osmhandler.cpp \
    waysignaldetector.cpp \
    way.cpp \
    osmpoint.cpp \
    vector2d.cpp \
    forwardview.cpp \
    forwardviewwidget.cpp \
    mapviewwidget.cpp \
    mapcache.cpp

HEADERS  += mainwindow.h \
    nodeassociatedtoway.h \
    osmhandler.h \
    waysignaldetector.h \
    way.h \
    osmpoint.h \
    vector2d.h \
    forwardview.h \
    forwardviewwidget.h \
    mapviewwidget.h \
    mapcache.h

FORMS    += mainwindow.ui

RESOURCES += \
    signals.qrc


#-------------------------------------------------
#
# Project created by QtCreator 2016-07-16T18:43:14
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenGL-Demos
TEMPLATE = app

include(libGraphics/libGraphics.pri)

SOURCES += main.cpp \
    MainWindow.cpp \
    GLWidget.cpp

HEADERS  += \
    MainWindow.h \
    GLWidget.h \
    android.h

FORMS    += \
    MainWindow.ui

CONFIG += mobility
MOBILITY = 


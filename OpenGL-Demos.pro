#-------------------------------------------------
#
# Project created by QtCreator 2016-07-16T18:43:14
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenGL-Demos
TEMPLATE = app


SOURCES += Sources/main.cpp \
    Sources/MainWindow.cpp \
    Sources/GLWidget.cpp

HEADERS  += Sources/MainWindow.h \
    Sources/GLWidget.h \
    Sources/android.h

FORMS    += Sources/MainWindow.ui

CONFIG += mobility
MOBILITY = 


#-------------------------------------------------
#
# Project created by QtCreator 2017-07-16T23:02:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = csv_pager
TEMPLATE = app

unix: QMAKE_CXXFLAGS += -Wall

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        viewer.cpp

HEADERS += \
        mainwindow.h \
        viewer.h

FORMS += \
        mainwindow.ui \
        viewer.ui

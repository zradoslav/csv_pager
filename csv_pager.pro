#-------------------------------------------------
#
# Project created by QtCreator 2017-07-16T23:02:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = csv_pager
TEMPLATE = app

# Jaroševi
unix: QMAKE_CXXFLAGS += -Wall -std=c++1y
win32: INCLUDEPATH += C:/dev/boost_1_64_0

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        viewer.cpp \
        fielddialog.cpp

HEADERS += \
        mainwindow.h \
        viewer.h \
        fielddialog.h

FORMS += \
        mainwindow.ui \
        viewer.ui \
        fielddialog.ui

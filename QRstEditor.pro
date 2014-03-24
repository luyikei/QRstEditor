#-------------------------------------------------
#
# Project created by QtCreator 2014-03-22T14:21:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QRstEditor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    edit.cpp \
    completionpopup.cpp

HEADERS  += mainwindow.h \
    edit.h \
    completionpopup.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc

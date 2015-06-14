#-------------------------------------------------
#
# Project created by QtCreator 2015-05-17T17:03:33
#
#-------------------------------------------------

QT       += core gui\
             xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Agenda
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Calendar.cpp \
    timing.cpp \
    projetmanager.cpp \
    tachemanager.cpp \
    programmationmanager.cpp

HEADERS  += mainwindow.h \
    Calendar.h \
    timing.h \
    frabiquesingleton.h \
    projetmanager.h \
    fenetre.h \
    tachemanager.h \
    programmationmanager.h
    projetmanager.h

FORMS    += \
    mainwindow.ui

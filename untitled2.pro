#-------------------------------------------------
#
# Project created by QtCreator 2016-01-06T17:15:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

wince*: {
   DEPLOYMENT_PLUGIN += qjpeg qgif
}

TARGET = untitled2
TEMPLATE = app


SOURCES += main.cpp \
        mainwindow.cpp \
    taskone.cpp \
    tasktwo.cpp \
    taskthree.cpp \
    taskfour.cpp \
    taskfive.cpp \
    tasksix.cpp \
    helper.cpp

HEADERS  += mainwindow.h \
    taskone.h \
    tasktwo.h \
    taskthree.h \
    taskfour.h \
    taskfive.h \
    tasksix.h \
    helper.h

FORMS    += mainwindow.ui

#-------------------------------------------------
#
# Project created by QtCreator 2015-06-10T14:31:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PacketMaker
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    calculationmodel.cpp \
    eventprediction.cpp \
    toteprediction.cpp \
    totepredictionmodel.cpp \
    toteresultmodel.cpp \
    toteround.cpp

HEADERS  += mainwindow.h \
    calculationmodel.h \
    eventprediction.h \
    toteprediction.h \
    totepredictionmodel.h \
    toteresultmodel.h \
    toteround.h

FORMS    += mainwindow.ui

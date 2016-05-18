#-------------------------------------------------
#
# Project created by QtCreator 2016-05-09T13:53:50
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QTDataSender
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    conexao.cpp

HEADERS  += mainwindow.h \
    conexao.h \
    conexaonaoestabelecida.h

FORMS    += mainwindow.ui

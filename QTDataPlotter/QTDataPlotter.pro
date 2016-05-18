#-------------------------------------------------
#
# Project created by QtCreator 2016-05-17T10:11:08
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QTDataPlotter
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    conexao.cpp \
    grafico.cpp

HEADERS  += mainwindow.h \
    conexao.h \
    conexaonaoestabelecida.h \
    grafico.h

FORMS    += mainwindow.ui

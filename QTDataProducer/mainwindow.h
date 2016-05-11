#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QIntValidator>
#include <QStringListModel>
#include "conexao.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    QRegExpValidator *ipPortaValidator;
    QIntValidator *faixaValidator;
    QStringListModel *model;
    Conexao *conexao;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void conectarServidor(bool);
    void inserirDadoLista(QString);

};

#endif // MAINWINDOW_H

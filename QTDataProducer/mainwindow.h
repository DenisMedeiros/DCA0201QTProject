#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDoubleValidator>
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
    QDoubleValidator *faixaValidator;
    QStringListModel *model;
    Conexao *conexao;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void conectarServidor(bool);

};

#endif // MAINWINDOW_H

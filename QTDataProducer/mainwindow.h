#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDoubleValidator>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QRegExpValidator *ipValidator;
    QDoubleValidator *faixaValidator;

public slots:
    void alteraTextoBotaoConectar(bool);

};

#endif // MAINWINDOW_H

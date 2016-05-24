#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRegExpValidator>
#include <QStringListModel>
#include <QTimer>

#include "conexaoplotter.h"
#include "erroconexao.h"

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

    QTimer *timer;
    QStringList *clientes;
    QString *clienteSelecionado;

    /** Validator para validar a string no formato IP:porta. */
    QRegExpValidator *ipPortaValidator;

    /** Modelo usado para desenhar na lista de IPs. */
    QStringListModel *model;

    /** Conexão utilizada para se comunicar com o servidor. */
    ConexaoPlotter *conexao;

    void atualizarListaClientes(void);
    void atualizarDadosCliente(void);

public slots:
    void conectar(bool ativado);
    void plot(void);
    void atualizar(void);
    void falhaConexao(void);
};

#endif // MAINWINDOW_H

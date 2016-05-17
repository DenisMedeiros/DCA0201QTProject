/**
 * @file mainwindow.h
 * @author Denis Ricardo da Silva Medeiros
 * @date 10 May 2016
 * @brief Programa produtor de dados para o servidor remoto.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QIntValidator>
#include <QStringListModel>
#include "conexao.h"

/**
 * @brief Este é o namespace padrão do próprio Qt para a janela principal.
 *
 */
namespace Ui {
    class MainWindow;
}

/**
 * @brief Esta é a janela principal do programa.
 *
 * Esta é a janela principal do programa, por onde o usuário
 * vai definir os parâmetros de geração de dados aleatórios,
 * bem como os parâmetros de acesso ao servidor remoto.
 *
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    /** UI da janela principal. */
    Ui::MainWindow *ui;

    /** Validator para validar a string no formato IP:porta. */
    QRegExpValidator *ipPortaValidator;

    /** Validator para validar as faixas de valores como inteiro. */
    QIntValidator *faixaValidator;

    /** Modelo usado para desenhar na lista de logs. */
    QStringListModel *model;

    /** Conexão utilizada para se comunicar com o servidor. */
    Conexao *conexao;

public:
    /**
     * @brief Este é o construtor padrão desta classe.
     * @param parent É o QWidget onde a MainWindow será
     * desenhada.
     */
    explicit MainWindow(QWidget *parent = 0);

    /**
     * @brief Este é o destrutor padrão desta classe.
     */
    ~MainWindow();

public slots:
    /**
     * @brief Este slot é invocado quando o botão de conectar é ativado e
     * ele tenta estabelecer a conexão com o servidor remoto.
     *
     * @param ativado Status sobre se o botão de conectar está ativo
     * ou inativo.
     */
    void conectarServidor(bool ativado);

    /**
     * @brief Este slot é invocado quando um dado foi enviado com sucesso para
     * o servidor e escreve na lista de logs o dado que fora enviado.
     *
     * @param dado O dado que foi enviado com sucesso para o servidor.
     */
    void inserirDadoLista(QString dado);

    /**
     * @brief Este slot é responsável por alterar o intervalo de tempo da geração dos dados.
     *
     * Ele é ativado quando o HorizontalSlider do intervalo tem seu valor mudado e, com isso,
     * ele para e reinicia o timer o novo valor estabelecido.
     *
     */
    void alterarIntervalo(int intervalo);


};

#endif // MAINWINDOW_H

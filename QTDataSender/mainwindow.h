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
#include <QRegExpValidator>
#include <QTimer>
#include "conexaosender.h"

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
    /** @brief UI da janela principal. */
    Ui::MainWindow *ui;

    /** @brief Timer utilizado para enviar os dados em um período definido. */
    QTimer *timer;

    /** @brief Valor mínimo que pode ser gerado e enviado para os servidor. */
    int faixaInicio;

    /** @brief Valor máximo que pode ser gerado e enviado para os servidor. */
    int faixaFim;

    /** @brief Período de tempo para o envio períodico de dados para o servidor. */
    int intervalo;

    /** @brief Validator para validar a string no formato IP:porta. */
    QRegExpValidator *ipPortaValidator;

    /** @brief Validator para validar as faixas de valores como inteiro. */
    QIntValidator *faixaValidator;

    /** @brief Modelo usado para desenhar na lista de logs. */
    QStringListModel *model;

    /** @brief Conexão utilizada para se comunicar com o servidor. */
    ConexaoSender *conexao;

    /** @brief Endereço IP do servidor. */
    QString ip;

    /** @brief Porta do servidor. */
    quint16 porta;

    /**
     * @brief Este método auxiliar gera um número aleatório inteiro entre
     * min e max (eles não são incluídos).
     *
     * @param min Valor mínimo para geração do número inteiro.
     * @param max Valor máximo para a geração do número inteiro.
     *
     * @return Um número aleatório entre e inclusive min e max.
     */
    int numero_aleatorio(int min, int max);

public:
    /**
     * @brief Este é o construtor padrão desta classe.
     *
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
     * Após conectado, o programa inicia o envio dos dados.
     *
     * @param ativado Status sobre se o botão de conectar está ativo
     * ou inativo.
     */
    void conectar(bool ativado);

    /**
     * @brief Este slot é responsável por alterar o intervalo de tempo da geração dos dados.
     *
     * Ele é ativado quando o HorizontalSlider do intervalo tem seu valor mudado e, com isso,
     * ele para e reinicia o timer o novo valor estabelecido.
     *
     */
    void alterarIntervalo(int _intervalo);


    /**
     * @brief Este slot é invocado quando o botão de enviar dados é acionado.
     *
     * Ele basicamente inicia o timer responsável por fazer o envio periódio
     * dos dados.
     *
     */
    void iniciarEnvioDados(bool ativado);


    /**
     * @brief Este slot é invocado quando o timer dispara.
     *
     * Ele basicamente gera um dado (data/hora e valor) com valor aleatório
     * definidos pelas faixas e, em seguida, faz seu envio para o servidor.
     *
     */
    void enviarDado(void);

    /**
     * @brief Este slot é invocado quando ocorre um erro com a conexão.
     *
     * Quando ocorre algum erro na conexão, ela envia um signal para a janela principal.
     * Nessa situação, o programa volta ao seu estado inicial e exibe a mensagem de erro.
     *
     */
    void falhaConexao(void);


};

#endif // MAINWINDOW_H

/**
 * @file mainwindow.h
 * @author Denis Ricardo da Silva Medeiros
 * @date 18 May 2016
 * @brief Janela principal do plotador de dados do cliente.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRegExpValidator>
#include <QStringListModel>
#include <QTimer>

#include "conexaoplotter.h"
#include "erroconexao.h"

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
 * vai selecionar um cliente da lista para plotar seus dados.
 *
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
     /** @brief UI da janela principal. */
    Ui::MainWindow *ui;

    /** @brief Timer responsável por atualizar a lista de dados no gráfico. */
    QTimer *timerDados;

    /** @brief Timer responsável por atualizar a lista clientes. */
    QTimer *timerListaClientes;

    /** @brief Lista de clientes conectados ao servidor. */
    QStringList *clientes;

    /** @brief Cliente atualmente selecionado para plotar seus dados. */
    QString *clienteSelecionado;

    /** @brief Validator para validar a string no formato IP:porta. */
    QRegExpValidator *ipPortaValidator;

    /** @brief Modelo usado para desenhar na lista de IPs. */
    QStringListModel *model;

    /** @brief Conexão utilizada para obter do servidor a lista de clientes. */
    ConexaoPlotter *conexaoDados;

    /** @brief Conexão utilizada para obter do servidor os dados de um cliente. */
    ConexaoPlotter *conexaoListaClientes;

    /** @brief Período de tempo para o atualizaçao períodica do gráfico. */
    int intervalo;

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
     *
     * Ele limpa todos os objetos da memória.
     */
    ~MainWindow();

public slots:
    /**
     * @brief Este slot é invocado quando o botão de conectar é ativado e
     * ele tenta estabelecer a conexão com o servidor remoto.
     *
     * Além de abrir as duas conexoes (da lista de clientes e de dados), ele também
     * inicia o timer responsável por ficar atualizando a lista de clientes
     * periodicamente.
     *
     * @param ativado Status sobre se o botão de conectar está ativo
     * ou inativo.
     */
    void conectar(bool ativado);

    /**
     * @brief Este slot é responsável por alterar o intervalo de tempo da atualização do gráfico.
     *
     * Ele é ativado quando o HorizontalSlider do intervalo tem seu valor mudado e, com isso,
     * ele para e reinicia o timer o novo valor estabelecido.
     *
     */
    void alterarIntervalo(int _intervalo);

    /**
     * @brief Este slot é invocado quando o botão de plot é clicado e inicia a plotagem dos dados.
     *
     * Ele também inicia o timer responsável por atualizar a lista de dados do gráfico
     * para o cliente selecionado.
     *
     */
    void plot(void);

    /**
     * @brief Este slot é invocado quando o timer de dados expira.
     *
     * Ele solicita ao servidor a lista mais recente de dados e repassa-os para o gráfico.
     * Além disso, ele também reorganiza os labels dos eixos do gráfico.
     *
     */
    void atualizarDados(void);

    /**
     * @brief Este slot é invocado quando o timer da lista de clientes expira.
     *
     * Ele solicita ao servidor a lista mais recente dos clientes conectados e
     * atualiza a lista lateral da janela principal.
     *
     */
    void atualizarListaClientes(void);

    /**
     * @brief Este slot é invocado quando ocorre um erro com a conexão.
     *
     * Quando ocorre algum erro na conexão, ela envia um signal para a janela principal.
     * Nessa situação, o programa volta ao seu estado inicial e exibe a mensagem de erro.
     *
     */
    void falhaConexao(const QString &erro);
};

#endif // MAINWINDOW_H

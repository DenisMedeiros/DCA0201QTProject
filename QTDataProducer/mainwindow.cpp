/**
 * @file mainwindow.cpp
 * @author Denis Ricardo da Silva Medeiros
 * @date 10 May 2016
 * @brief Programa produtor de dados para o servidor remoto.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QRegExp>
#include <QStringList>
#include <QDebug>

#include <cstdlib>

/**
 * @brief Este é o construtor padrão desta classe.
 * @param parent É o QWidget onde a MainWindow será
 * desenhada.
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{



    QString ipRange, portRange;
    QRegExp ipPortaRegex;

    ui->setupUi(this);

    /* Expressão regular para validar um endereço IP + uma porta. */
    ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";

    portRange = "([0-9]{1,4}|[1-5][0-9]{4}|6[0-4][0-9]{3}|"
                          "65[0-4][0-9]{2}|655[0-2][0-9]|6553[0-5])";

    ipPortaRegex = QRegExp("^" + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange + "\\:" + portRange + "$");



    /* Atribui um validator para o campo IPPorta. */
    ipPortaValidator = new QRegExpValidator(ipPortaRegex, this);
    ui->lineEditIPPorta->setValidator(ipPortaValidator);

    /* Cria um validator para números inteiros nos campos de faixa. */
    faixaValidator = new QIntValidator();

    ui->lineEditFaixaInicio->setValidator(faixaValidator);
    ui->lineEditFaixaFim->setValidator(faixaValidator);

    /* Altere a mensagem padrão na barra de status. */
    ui->statusBar->showMessage("Desconectado");

    /* Prepara a lista de dados enviados. */
    model = new QStringListModel(this);

    /* Adicione o modelo na lista e torne-a não editável. */
    ui->listViewRegistros->setModel(model);
        ui->listViewRegistros->setEditTriggers(QAbstractItemView::NoEditTriggers);

    /* Crie a conexão. */
    conexao = new Conexao();

    /* Faça a conexão do sinal e slot de quando um dado é enviado para o servidor. */
    connect(conexao, SIGNAL(dadoEnviado(QString)), this, SLOT(inserirDadoLista(QString)));

    /* Faça a conexão do sinal e slot de quando o intervalo é alterado. */
    connect(ui->horizontalSliderIntervalo, SIGNAL(valueChanged(int)), conexao, SLOT(alterarIntervalo(int)));

}

/**
 * @brief Este é o destrutor padrão desta classe.
 */
MainWindow::~MainWindow()
{
    delete ipPortaValidator;
    delete faixaValidator;
    delete conexao;
    delete ui;
}

/**
 * @brief Este slot é invocado quando o botão de conectar é ativado e
 * ele tenta estabelecer a conexão com o servidor remoto.
 * @param ativado Status sobre se o botão de conectar está ativo
 * ou inativo.
 */
void MainWindow::conectarServidor(bool ativado)
{
    QString ip;
    QStringList ipPorta;
    unsigned int porta;
    int faixaInicio, faixaFim, intervalo;


    if (ativado) /* Se o botão foi ativado. */
    {
        /* A string está no formato IP:porta e são separados abaixo. */
        ipPorta = ui->lineEditIPPorta->text().split(":");


        /* Valida a porta e IP. */
        if(ipPorta.size() != 2) {
            ui->statusBar->clearMessage();
            ui->statusBar->showMessage("Porta ou IP inválidos.");
            ui->pushButtonConectar->setChecked(false);
            return;
        }

        ip = ipPorta.at(0);
        porta = ipPorta.at(1).toUInt();

        /* Tente criar a conexão. */
        try
        {
            conexao->abrir(ip, porta);

            /* Se não ocorreu erro durante a abertura da conexão, então
             * mude os elementos visuais da tela. */
            ui->pushButtonConectar->setText("Desonectar");
            ui->statusBar->clearMessage();
            ui->statusBar->showMessage("Conectado com sucesso ao servidor "
                    + ip + " na porta " + QString::number(porta) + ".");

            /* Inicie imediatamente o envio de dados com base nos parâmetros definidos. */

            faixaInicio = ui->lineEditFaixaInicio->text().toInt();
            faixaFim = ui->lineEditFaixaFim->text().toInt();
            intervalo = ui->horizontalSliderIntervalo->value();

            conexao->enviarDados(faixaInicio, faixaFim, intervalo);
        }

        /* Caso tenha ocorrido algum erro na tentativa de conexão,
         * exiba o erro na barra de status. */
        catch(ConexaoNaoEstabelecida &erro)
        {
            ui->pushButtonConectar->setChecked(false);
            ui->statusBar->clearMessage();
            ui->statusBar->showMessage(QString(erro.getMensagem()));
        }
    }

    else /* Se o botão foi desativado. */
    {
        /* Altera os elementos visuais, para o  envio de dados e fecha a coneções. */
        ui->pushButtonConectar->setText("Conectar");
        ui->statusBar->clearMessage();
        ui->statusBar->showMessage("Desconectado.");
        conexao->pararEnvio();
        conexao->fechar();
    }
}

/**
 * @brief Este slot é invocado quando um dado foi enviado com sucesso para
 * o servidor e escreve na lista de logs o dado que fora enviado.
 * @param dado O dado que foi enviado com sucesso para o servidor.
 */
void MainWindow::inserirDadoLista(QString dado)
{
    /* Insere um dado numa nova linha no início da lista. */
    model->insertRows(0, 1);
    model->setData(model->index(0), dado);
}

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

    /* Adiciona o modelo na lista e torne-a não editável. */
    ui->listViewRegistros->setModel(model);
    ui->listViewRegistros->setEditTriggers(QAbstractItemView::NoEditTriggers);

    /* Cria a conexão. */
    conexao = new Conexao();

    /* Faz a conexão do sinal e slot de quando um dado é enviado para o servidor. */
    connect(conexao, SIGNAL(dadoEnviado(QString)), this, SLOT(inserirDadoLista(QString)));

    /* Faz a conexão do sinal e slot de quando o intervalo é alterado. */
    connect(ui->horizontalSliderIntervalo, SIGNAL(valueChanged(int)), this, SLOT(alterarIntervalo(int)));

    /* Faz a conexão do botão 'Conectar' com o slot que abre a conexão com o servidor. */
    connect(ui->pushButtonConectar, SIGNAL(clicked(bool)), this, SLOT(conectarServidor(bool)));

    /* Faz a conexão do slider de intervalo com o label que exibe o valor atual. */
    connect(ui->horizontalSliderIntervalo, SIGNAL(valueChanged(int)), ui->labelValorIntervalo, SLOT(setNum(int)));

}

MainWindow::~MainWindow()
{
    delete ipPortaValidator;
    delete faixaValidator;
    delete conexao;
    delete ui;
}

void MainWindow::conectarServidor(bool ativado)
{
    QString ip, faixaInicioStr, faixaFimStr;
    QStringList ipPorta;
    unsigned int porta;
    int faixaInicio, faixaFim, intervalo;


    if (ativado) /* Se o usuário clicou em 'Conectar'. */
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

        intervalo = ui->horizontalSliderIntervalo->value();

        /* Valida as faixas de valores. */

        faixaInicioStr = ui->lineEditFaixaInicio->text();
        faixaFimStr = ui->lineEditFaixaFim->text();

        if(faixaInicioStr.isEmpty() || faixaFimStr.isEmpty())
        {
            ui->statusBar->clearMessage();
            ui->statusBar->showMessage("Faixa de início ou fim em branco. Digite um número válido.");
            ui->pushButtonConectar->setChecked(false);
            return;
        }

        /* Inicie imediatamente o envio de dados com base nos parâmetros definidos. */

        faixaInicio = faixaInicioStr.toInt();
        faixaFim = faixaFimStr.toInt();

        /* Tente criar a conexão. */
        try
        {
            conexao->abrir(ip, porta);

            /* Se não ocorreu erro durante a abertura da conexão, então
             * mude os elementos visuais da tela. */
            ui->pushButtonConectar->setText("Desconectar");
            ui->statusBar->clearMessage();
            ui->statusBar->showMessage("Conectado e enviando dados para servidor "
                    + ip + " na porta " + QString::number(porta) + ".");


            /* Desabilita a alteração dos valores do intervalo. */
            ui->lineEditFaixaInicio->setEnabled(false);
            ui->lineEditFaixaFim->setEnabled(false);

            conexao->setFaixaInicio(faixaInicio);
            conexao->setFaixaFim(faixaFim);
            conexao->setIntervalo(intervalo);

            conexao->iniciarEnvio();
        }

        /* Caso tenha ocorrido algum erro na tentativa de conexão,
         * exiba o erro na barra de status. */
        catch(ErroConexao &erro)
        {
            ui->pushButtonConectar->setChecked(false);
            ui->statusBar->clearMessage();
            ui->statusBar->showMessage(QString(erro.getMensagem()));
        }
    }

    else /* Se o usuário clicou em 'Desconectar'. */
    {
        /* Altera os elementos visuais, para o  envio de dados e fecha a coneções. */
        ui->pushButtonConectar->setText("Conectar");
        ui->statusBar->clearMessage();
        ui->statusBar->showMessage("Desconectado.");

        /* Habilita a alteração dos valores do intervalo. */
        ui->lineEditFaixaInicio->setEnabled(true);
        ui->lineEditFaixaFim->setEnabled(true);

        conexao->pararEnvio();
        conexao->fechar();
    }
}


void MainWindow::inserirDadoLista(QString dado)
{
    /* Insere um dado numa nova linha no início da lista. */
    model->insertRows(0, 1);
    model->setData(model->index(0), dado);
}


void MainWindow::alterarIntervalo(int intervalo)
{
    if(conexao->isAtiva())
    {
        conexao->setIntervalo(intervalo);
    }
}

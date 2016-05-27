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
#include <QDateTime>
#include <cstdlib>

int MainWindow::numero_aleatorio(int min, int max)
{
    return ((float(rand()) / float(RAND_MAX)) * (max - min + 2)) + min - 1;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    QString ipRange, portRange;
    QRegExp ipPortaRegex;

    ui->setupUi(this);

    timer = new QTimer();
    conexao = new ConexaoSender();

    faixaInicio = 0;
    faixaFim = 10;
    intervalo = 1;

    /* Usa a hora atual como seed para os números aleatórios. */
    qsrand(QDateTime::currentDateTime().toTime_t());

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


    ui->pushButtonEnviarDados->setEnabled(false);


    /* Faz a conexão do sinal e slot de quando o intervalo é alterado. */
    connect(ui->horizontalSliderIntervalo, SIGNAL(valueChanged(int)), this, SLOT(alterarIntervalo(int)));

    /* Faz a conexão do botão 'Conectar' com o slot que abre a conexão com o servidor. */
    connect(ui->pushButtonConectar, SIGNAL(clicked(bool)), this, SLOT(conectar(bool)));

    /* Faz a conexão do slider de intervalo com o label que exibe o valor atual. */
    connect(ui->horizontalSliderIntervalo, SIGNAL(valueChanged(int)), ui->labelValorIntervalo, SLOT(setNum(int)));

    /* Faz a conexão da expiração do timer com o slot que envia o dado para o servidor. */
    connect(timer, SIGNAL(timeout()), this, SLOT(enviarDado()));

    connect(conexao, SIGNAL(falhaConexao()), this, SLOT(falhaConexao()));

    connect(ui->pushButtonEnviarDados, SIGNAL(clicked(bool)),)

}

MainWindow::~MainWindow()
{
    delete timer;
    delete ipPortaValidator;
    delete faixaValidator;
    delete conexao;
    delete ui;
}

void MainWindow::conectar(bool ativado)
{
    QString faixaInicioStr, faixaFimStr;
    QStringList ipPorta;

    if (ativado) /* Se o usuário clicou em 'Conectar'. */
    {
        /* A string está no formato IP:porta e são separados abaixo. */
        ipPorta = ui->lineEditIPPorta->text().split(":");

        /* Valida o endereço IP e a porta. */
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

        faixaInicio = faixaInicioStr.toInt();
        faixaFim = faixaFimStr.toInt();

        /* Abre a conexão e inicia o timer que envia os dados. */
        try
        {
            conexao->abrir(ip, porta);

            /* Se não ocorreu erro durante a abertura da conexão, então
             * mude os elementos visuais da tela. */
            ui->pushButtonConectar->setText("Desconectar");
            ui->statusBar->clearMessage();
            ui->statusBar->showMessage("Conectado ao servidor "
                    + ip + " na porta " + QString::number(porta) + ".");

            ui->pushButtonEnviarDados->setEnabled(true);
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

        conexao->fechar();
    }
}


void MainWindow::alterarIntervalo(int _intervalo)
{
    if(conexao->isAtiva())
    {
        intervalo = _intervalo;

        if(timer->isActive())
        {
            timer->setInterval(intervalo * 1000);
        }
    }
}

void MainWindow::iniciarEnvioDados(bool ativado)
{

    if(ativado)
    {
        ui->statusBar->clearMessage();
        ui->statusBar->showMessage("Enviando dados para servidor "
                + ip + " na porta " + QString::number(porta) + ".");

        /* Desabilita a alteração dos valores do intervalo. */
        ui->lineEditFaixaInicio->setEnabled(false);
        ui->lineEditFaixaFim->setEnabled(false);

        ui->pushButtonEnviarDados->setText("Parar");
        ui->pushButtonEnviarDados->setChecked(true);

        timer->start(intervalo * 1000);
    }
    else
    {
        /* Altera os elementos visuais, para o  envio de dados e fecha a coneções. */
        ui->pushButtonConectar->setText("Enviar Dados");
        ui->statusBar->clearMessage();
        ui->statusBar->showMessage("Conectado ao servidor "
                + ip + " na porta " + QString::number(porta) + ".");

        /* Habilita a alteração dos valores do intervalo. */
        ui->lineEditFaixaInicio->setEnabled(true);
        ui->lineEditFaixaFim->setEnabled(true);

        timer->stop();

    }
}

void MainWindow::enviarDado(void)
{
    QString dado;

    if(conexao->isAtiva())
    {
        dado = QDateTime::currentDateTime().toString(Qt::ISODate)
                + " " + QString::number(numero_aleatorio(faixaInicio, faixaFim));

        conexao->enviar(dado);

        model->insertRows(0, 1);
        model->setData(model->index(0), dado);
    }

}

void MainWindow::falhaConexao(void)
{
    conexao->fechar();
    timer->stop();

    /* Remove a lista de dados. */
    model->removeRows(0, model->rowCount());

    ui->pushButtonConectar->setEnabled(true);
    ui->pushButtonConectar->setChecked(false);
    ui->pushButtonConectar->setText("Conectar");

    ui->statusBar->clearMessage();
    ui->statusBar->showMessage("Erro na conexão: O servidor parou de responder.");
}

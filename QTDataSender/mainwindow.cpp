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

    /* Expressão regular para validar um endereço IP + uma port separados por ":". */
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

    /* Adiciona o modelo na lista e torna-a não editável. */
    ui->listViewRegistros->setModel(model);
    ui->listViewRegistros->setEditTriggers(QAbstractItemView::NoEditTriggers);

    /* O botão de envio começa desabilitado até que a conexão seja aberta. */
    ui->pushButtonEnviarDados->setEnabled(false);
    ui->pushButtonEnviarDados->setCheckable(true);

    /* Conexão do sinal e slot de quando o intervalo é alterado. */
    connect(ui->horizontalSliderIntervalo, SIGNAL(valueChanged(int)), this, SLOT(alterarIntervalo(int)));

    /* Conexão do botão 'Conectar' com o slot que abre a conexão com o servidor. */
    connect(ui->pushButtonConectar, SIGNAL(clicked(bool)), this, SLOT(conectar(bool)));

    /* Conexão do slider de intervalo com o label que exibe o valor atual. */
    connect(ui->horizontalSliderIntervalo, SIGNAL(valueChanged(int)), ui->labelValorIntervalo, SLOT(setNum(int)));

    /* Conexão do sinal de erro na conexão com o slot que faz o seu tratamento. */
    connect(conexao, SIGNAL(falhaConexao()), this, SLOT(falhaConexao()));

    /* Conexão do botão que inicia o envio de dados e o slot que inicia o timer. */
    connect(ui->pushButtonEnviarDados, SIGNAL(clicked(bool)), this, SLOT(iniciarEnvioDados(bool)));

    /* Conexão da expiração do timer com o slot que envia o dado para o servidor. */
    connect(timer, SIGNAL(timeout()), this, SLOT(enviarDado()));
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
    QStringList ipPorta;

    if (ativado) /* O usuário clicou em 'Conectar'. */
    {
        /* A string está no formato "IP:porta" e são separados abaixo. */
        ipPorta = ui->lineEditIPPorta->text().split(":");

        /* Valida o endereço IP e a porta. */
        if(ipPorta.size() != 2) {
            ui->statusBar->showMessage("Porta ou IP inválidos.");
            ui->pushButtonConectar->setChecked(false);
            return;
        }

        ip = ipPorta.at(0);
        porta = ipPorta.at(1).toUInt();
        intervalo = ui->horizontalSliderIntervalo->value();

        try /* Tenta abrir a conexão. */
        {
            conexao->abrir(ip, porta);

            /* Se não ocorreu erro durante a abertura da conexão, então
             * mude os elementos visuais da tela. */
            ui->pushButtonConectar->setText("Desconectar");

            ui->statusBar->showMessage("Conectado ao servidor "
                    + ip + " na porta " + QString::number(porta) + ".");

            ui->pushButtonEnviarDados->setEnabled(true);
        }

        /* Caso tenha ocorrido algum erro na tentativa de conexão,
         * exiba o erro na barra de status. */
        catch(ErroConexao &erro)
        {
            ui->pushButtonConectar->setChecked(false);
            ui->statusBar->showMessage(QString(erro.getMensagem()));
        }
    }

    else /* O usuário clicou em 'Desconectar'. */
    {
        /* Para o envio de dados e fecha a conexão. */
        timer->stop();
        conexao->fechar();

        /* Altera os elementos visuais, para o  envio de dados e fecha a conexões. */
        ui->pushButtonConectar->setText("Conectar");

        ui->pushButtonEnviarDados->setText("Enviar Dados");
        ui->pushButtonEnviarDados->setEnabled(false);
        ui->pushButtonEnviarDados->setChecked(false);

        ui->statusBar->showMessage("Desconectado.");

        /* Habilita a alteração dos valores do intervalo. */
        ui->lineEditFaixaInicio->setEnabled(true);
        ui->lineEditFaixaFim->setEnabled(true);

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
    QString faixaInicioStr, faixaFimStr;

    if(ativado) /* O usuário clicou para iniciar o envio. */
    {

        /* Valida as faixas de valores. */
        faixaInicioStr = ui->lineEditFaixaInicio->text();
        faixaFimStr = ui->lineEditFaixaFim->text();

        if(faixaInicioStr.isEmpty() || faixaFimStr.isEmpty())
        {
            ui->statusBar->showMessage("Faixa de início ou fim em branco. Digite um número válido.");
            ui->pushButtonEnviarDados->setChecked(false);
            return;
        }

        faixaInicio = faixaInicioStr.toInt();
        faixaFim = faixaFimStr.toInt();

        /* Verifique se as faixas são válidas. */
        if(faixaInicio >= faixaFim)
        {
            ui->statusBar->showMessage("Faixa de início deve ser menor que a de fim.");
            ui->pushButtonEnviarDados->setChecked(false);
            return;
        }

        ui->statusBar->showMessage("Enviando dados para servidor "
                + ip + " na porta " + QString::number(porta) + ".");

        /* Desabilita a alteração dos valores do intervalo. */
        ui->lineEditFaixaInicio->setEnabled(false);
        ui->lineEditFaixaFim->setEnabled(false);

        /* Altera a aparência do botão de envio. */
        ui->pushButtonEnviarDados->setText("Parar");
        ui->pushButtonEnviarDados->setChecked(true);

        /* Inicia o envio periódico de dados. */
        timer->start(intervalo * 1000);
    }
    else /* O usuário clicou para parar o envio. */
    {
        /* Altera os elementos visuais, para o  envio de dados e para o timer. */

        ui->pushButtonEnviarDados->setText("Enviar Dados");
        ui->pushButtonEnviarDados->setChecked(false);

        ui->statusBar->showMessage("Conectado ao servidor "
                + ip + " na porta " + QString::number(porta) + ".");

        /* Habilita a alteração dos valores do intervalo. */
        ui->lineEditFaixaInicio->setEnabled(true);
        ui->lineEditFaixaFim->setEnabled(true);

        /* Para o envio periódico de dados. */
        timer->stop();
    }
}

void MainWindow::enviarDado(void)
{
    QString dado;

    if(conexao->isAtiva())
    {

        /* Gera o dado aleatório, obtém a hora atual e faz o envio para o servidor. */
        dado = QDateTime::currentDateTime().toString(Qt::ISODate)
                + " " + QString::number(numero_aleatorio(faixaInicio, faixaFim));

        conexao->enviar(dado);

        /* Atualiza a lista de dados enviados inserindo o registro. */
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

    /* Altera a aparência dos botões para o modelo inicial. */
    ui->pushButtonConectar->setEnabled(true);
    ui->pushButtonConectar->setChecked(false);
    ui->pushButtonConectar->setText("Conectar");

    ui->pushButtonEnviarDados->setEnabled(true);
    ui->pushButtonEnviarDados->setChecked(false);
    ui->pushButtonEnviarDados->setText("Enviar Dados");

    ui->statusBar->showMessage("Erro na conexão: O servidor parou de responder.");
}

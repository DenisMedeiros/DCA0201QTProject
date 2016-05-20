#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QRegExp>
#include <QDebug>
#include <QAbstractItemView>
#include <QModelIndexList>
#include <QList>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{    
    QString ipRange, portRange;
    QRegExp ipPortaRegex;

    ui->setupUi(this);

    conexao = new Conexao();
    timerEnvio = new QTimer();
    timerListaClientes = new QTimer();
    clientes = new QStringList();
    clienteSelecionado = new QString();


    model = new QStringListModel(this);

    /* Adiciona o modelo na lista e torne-a não editável. */
    ui->listViewClientes->setModel(model);
    ui->listViewClientes->setEditTriggers(QAbstractItemView::NoEditTriggers);

    /* Selecione apenas uma linha. */

    ui->listViewClientes->setSelectionBehavior(QAbstractItemView::SelectRows);

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

    /* Conectando os sinais e slots. */
    connect(ui->pushButtonConectar, SIGNAL(clicked(bool)), this, SLOT(conectar(bool)));
    connect(ui->pushButtonPlot, SIGNAL(pressed()), this, SLOT(plot(void)));

    connect(timerEnvio, SIGNAL(timeout()), this, SLOT(atualizarDados()));
    connect(timerListaClientes, SIGNAL(timeout()), this, SLOT(atualizarListaClientes()));

    /* Esconde o gráfico e os labels. */
    ui->grafico->hide();
    ui->labelXInicio->hide();
    ui->labelXFim->hide();
    ui->labelYInicio->hide();
    ui->labelYFim->hide();


}

MainWindow::~MainWindow()
{
    if(timerEnvio->isActive())
    {
        timerEnvio->stop();
    }
    if(timerListaClientes->isActive())
    {
        timerListaClientes->stop();
    }

    delete clienteSelecionado;
    delete clientes;
    delete ipPortaValidator;
    delete model;
    delete conexao;
    delete ui;
}

void MainWindow::conectar(bool ativado)
{
    QString ip;
    unsigned int porta;
    QStringList ipPorta;

    if(ativado){ /* Se o usuário clicou em 'Conectar'. */

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

        try
        {
            conexao->abrir(ip, porta);

            ui->statusBar->clearMessage();
            ui->statusBar->showMessage("Conectado com sucesso ao servidor "
                    + ip + " na porta " + QString::number(porta) + ".");

            ui->pushButtonPlot->setEnabled(true);
            ui->pushButtonConectar->setText("Desconectar");

            atualizarListaClientes();
            timerListaClientes->start(5000);
        }
        catch(ConexaoNaoEstabelecida &erro)
        {
            ui->pushButtonConectar->setChecked(false);
            ui->statusBar->clearMessage();
            ui->statusBar->showMessage(QString(erro.getMensagem()));
        }

    } else { /* Se o usuário clicou em 'Desconectar'. */

        conexao->fechar();
        timerEnvio->stop();
        timerListaClientes->stop();

        /* Remove a lista de clientes. */
        model->removeRows(0, model->rowCount());

        ui->statusBar->clearMessage();
        ui->statusBar->showMessage("Desconectado");

        ui->pushButtonConectar->setEnabled(true);
        ui->pushButtonConectar->setText("Conectar");

        /* Esconda o gráfico. */
        ui->grafico->hide();
        ui->labelXInicio->hide();
        ui->labelXFim->hide();
        ui->labelYInicio->hide();
        ui->labelYFim->hide();
    }
}

void MainWindow::plot(void)
{

    QString cliente;
    QModelIndexList indices;

    if(conexao->isAtiva()){

        indices = ui->listViewClientes->selectionModel()->selectedIndexes();
        cliente = ui->listViewClientes->model()->data(indices.at(0)).toString();

        qDebug() << cliente;

        if(cliente != *clienteSelecionado)
        {
            *clienteSelecionado = cliente;
        }

        qDebug() << *clienteSelecionado;

        ui->statusBar->clearMessage();
        ui->statusBar->showMessage("Plotando os dados do cliente " + *clienteSelecionado + ".");

        atualizarDados();
        timerEnvio->start(1000);
    }

    /* Exibe o gráfico e os labels. */
    ui->grafico->show();
    ui->labelXInicio->show();
    ui->labelXFim->show();
    ui->labelYInicio->show();
    ui->labelYFim->show();

}

void MainWindow::atualizarDados(void)
{
    QString cliente;
    QModelIndexList indices;
    QList<Dado> dados, ultimos20dados;
    QList<int> valores;
    int menorY, maiorY;

    indices = ui->listViewClientes->selectionModel()->selectedIndexes();

    dados = conexao->getDados(*clienteSelecionado);

    /* Obtem os ultimos 20 dados. */
    if(dados.size() <= 20)
    {
        ultimos20dados = dados;
    }
    else
    {
        ultimos20dados = dados.mid(dados.size()-20);
    }

    /* Encontre o menor e maior valor do eixo Y. */

    foreach(Dado dado, ultimos20dados)
    {
        valores.append(dado.valor);
    }

    qSort(valores);

    /* Altere os labels para os valores iniciais e finais em cada eixo. */

    ui->labelXInicio->setText(ultimos20dados.at(0).datetime.toString("dd.MM.yyyy hh:mm:ss"));
    ui->labelXFim->setText(ultimos20dados.at(ultimos20dados.size()- 1).datetime.toString("dd.MM.yyyy hh:mm:ss"));

    ui->labelYInicio->setText(QString::number(valores.at(0)));
    ui->labelYFim->setText(QString::number(valores.at(valores.size() - 1)));

    ui->grafico->setDados(ultimos20dados);

    menorY = valores.at(0);
    maiorY = valores.at(valores.size()-1);


    /* Se surgiu um valor maior ou menor que os do eixo Y atuais, atualize-os. */

    if(maiorY > (ui->grafico->getMenorY()))
    {
        ui->grafico->setMenorY(menorY);
    }

    if(maiorY > ui->grafico->getMaiorY())
    {
        ui->grafico->setMaiorY(maiorY);
    }

    ui->grafico->repaint();

}

void MainWindow::atualizarListaClientes()
{
    QStringList clientesServidor;

    if(conexao->isAtiva())
    {
        clientesServidor = conexao->getClientes();

        if(clientesServidor != *clientes)
        {
            *clientes = clientesServidor;
        }

        ui->listViewClientes->selectionModel()->

        model->removeRows(0, model->rowCount());

        foreach(QString cliente, *clientes){
            /* Insere um dado numa nova linha no início da lista. */
            model->insertRows(0, 1);
            model->setData(model->index(0), cliente);
        }
    }
}

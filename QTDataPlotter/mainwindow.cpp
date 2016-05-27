#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QRegExp>
#include <QDebug>
#include <QAbstractItemView>
#include <QModelIndexList>
#include <QList>
#include <QStringList>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{    
    QString ipRange, portRange;
    QRegExp ipPortaRegex;

    ui->setupUi(this);

    conexaoDados = new ConexaoPlotter();
    conexaoListaClientes = new ConexaoPlotter();

    timerDados = new QTimer();
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

    connect(timerDados, SIGNAL(timeout()), this, SLOT(atualizarDados()));
    connect(timerListaClientes, SIGNAL(timeout()), this, SLOT(atualizarListaClientes()));


    connect(conexaoDados, SIGNAL(falhaConexao()), this, SLOT(falhaConexao()));
    connect(conexaoListaClientes, SIGNAL(falhaConexao()), this, SLOT(falhaConexao()));

    ui->pushButtonPlot->setEnabled(false);

    /* Esconde o gráfico e os labels. */
    ui->grafico->hide();
    ui->labelXInicio->hide();
    ui->labelXFim->hide();
    ui->labelYInicio->hide();
    ui->labelYFim->hide();

}

MainWindow::~MainWindow()
{
    delete timerDados;
    delete timerListaClientes;
    delete clienteSelecionado;
    delete clientes;
    delete ipPortaValidator;
    delete model;
    delete conexaoDados;
    delete conexaoListaClientes;
    delete ui;
}

void MainWindow::conectar(bool ativado)
{
    QString ip;
    quint16 porta;
    QStringList ipPorta;

    if(ativado){ /* Se o usuário clicou em 'Conectar'. */

        /* Começa a atualizar a lista de clientes. */

        timerListaClientes->start(1000);

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
            conexaoDados->abrir(ip, porta);
            conexaoListaClientes->abrir(ip, porta);

            /* Verifique se existe algum cliente conectado. */
            atualizarListaClientes();

            ui->statusBar->clearMessage();
            ui->statusBar->showMessage("Conectado com sucesso ao servidor "
                    + ip + " na porta " + QString::number(porta) + ".");

            ui->pushButtonPlot->setEnabled(true);
            ui->pushButtonConectar->setText("Desconectar");

            ui->pushButtonPlot->setEnabled(true);

            atualizarListaClientes();
        }
        catch(ErroConexao &erro)
        {
            conexaoDados->fechar();
            conexaoListaClientes->fechar();

            ui->pushButtonPlot->setEnabled(false);

            ui->pushButtonConectar->setChecked(false);
            ui->statusBar->clearMessage();
            ui->statusBar->showMessage(QString(erro.getMensagem()));
        }

    } else { /* Se o usuário clicou em 'Desconectar'. */

        conexaoDados->fechar();
        conexaoListaClientes->fechar();

        timerDados->stop();
        timerListaClientes->stop();

        /* Remove a lista de clientes. */
        model->removeRows(0, model->rowCount());

        ui->statusBar->clearMessage();
        ui->statusBar->showMessage("Desconectado");

        ui->pushButtonPlot->setEnabled(false);

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

    if(conexaoDados->isAtiva()){

        /* Verifica se o usuário selecionou algum cliente na lista. */

        indices = ui->listViewClientes->selectionModel()->selectedIndexes();

        if(indices.isEmpty())
        {
            ui->statusBar->clearMessage();
            ui->statusBar->showMessage("Você precisa selecionar um cliente na lista.");
            return;
        }

        cliente = ui->listViewClientes->model()->data(indices.at(0)).toString();

        if(cliente != *clienteSelecionado)
        {
            *clienteSelecionado = cliente;
        }



        ui->statusBar->clearMessage();
        ui->statusBar->showMessage("Plotando os dados do cliente " + *clienteSelecionado + ".");

        atualizarDados();
        timerDados->start(1000);

        /* Exibe o gráfico e os labels. */
        ui->grafico->show();
        ui->labelXInicio->show();
        ui->labelXFim->show();
        ui->labelYInicio->show();
        ui->labelYFim->show();

    }
}

void MainWindow::atualizarDados(void)
{
    QString cliente;
    QModelIndexList indices;
    QList<Dado> ultimos20Dados;
    QList<int> valores;
    int menorY, maiorY;

    if(conexaoDados->isAtiva())
    {

        ultimos20Dados = conexaoDados->getUltimos20Dados(*clienteSelecionado);

        ui->grafico->setDados(ultimos20Dados);
        ui->grafico->update();

        /* Encontre o menor e maior valor do eixo Y. */

        foreach(Dado dado, ultimos20Dados)
        {
            valores.append(dado.valor);
        }

        qSort(valores);

        /* Altere os labels para os valores iniciais e finais em cada eixo. */

        ui->labelXInicio->setText(ultimos20Dados.at(0).datetime.toString("dd.MM.yyyy hh:mm:ss"));
        ui->labelXFim->setText(ultimos20Dados.at(ultimos20Dados.size()- 1).datetime.toString("dd.MM.yyyy hh:mm:ss"));

        ui->labelYInicio->setText(QString::number(valores.at(0)));
        ui->labelYFim->setText(QString::number(valores.at(valores.size() - 1)));

        menorY = valores.at(0);
        maiorY = valores.at(valores.size()-1);

        /* Atualize os limites do eixo Y. */

        ui->grafico->setMenorY(menorY);
        ui->grafico->setMaiorY(maiorY);
    }
}

void MainWindow::atualizarListaClientes()
{
    QStringList clientesServidor;

    if(conexaoListaClientes->isAtiva())
    {

        clientesServidor = conexaoListaClientes->getClientes();

        /* Verifica se é o primeiro preenchimento ou se a lista atualizou. */
        if((ui->listViewClientes->model()->rowCount() == 0) || (*clientes != clientesServidor))
        {
            /* Limpa a lista e a preenche novamente. */
            model->removeRows(0, model->rowCount());

            foreach(QString cliente, *clientes){
                /* Insere um dado numa nova linha no início da lista. */
                model->insertRows(0, 1);
                model->setData(model->index(0), cliente);
            }

            /* Atualiza a lista de clientes localmente. */
            *clientes = clientesServidor;
        }

    }
}

void MainWindow::falhaConexao()
{
    conexaoDados->fechar();
    conexaoListaClientes->fechar();
    timerDados->stop();
    timerListaClientes->stop();

    /* Remove a lista de clientes. */
    model->removeRows(0, model->rowCount());

    ui->pushButtonConectar->setEnabled(true);
    ui->pushButtonConectar->setChecked(false);
    ui->pushButtonConectar->setText("Conectar");

    /* Esconda o gráfico. */
    ui->grafico->hide();
    ui->labelXInicio->hide();
    ui->labelXFim->hide();
    ui->labelYInicio->hide();
    ui->labelYFim->hide();

    ui->statusBar->clearMessage();
    ui->statusBar->showMessage("Erro na conexão: O servidor parou de responder.");

}

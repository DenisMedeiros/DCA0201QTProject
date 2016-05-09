#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QRegExp>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    QString ipRange;
    QRegExp ipRegex, faixaRegex;

    ui->setupUi(this);

    /* Expressão regular para garantir um endereço IP. */
    ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    ipRegex = QRegExp("^" + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange + "$");

    /* Cria um validator para IP. */
    ipValidator = new QRegExpValidator(ipRegex, this);
    ui->lineEditIP->setValidator(ipValidator);

    /* Cria um validator para números reais. */
    faixaValidator = new QDoubleValidator();

    ui->lineEditFaixaInicio->setValidator(faixaValidator);
    ui->lineEditFaixaFim->setValidator(faixaValidator);

}

MainWindow::~MainWindow()
{
    delete ipValidator;
    delete faixaValidator;
    delete ui;
}

void MainWindow::alteraTextoBotaoConectar(bool ativado)
{
    if (ativado)
        ui->pushButtonConectar->setText("Desonectar");
    else
        ui->pushButtonConectar->setText("Conectar");
}

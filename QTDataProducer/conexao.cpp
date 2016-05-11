#include "conexao.h"
#include <QDateTime>

Conexao::Conexao(void) : QObject()
{

    socket = new QTcpSocket();
    timer = new QTimer();
    ativa = false;

    qDebug() << QDateTime::currentDateTime().toTime_t();
    qsrand(QDateTime::currentDateTime().toTime_t());

    connect(timer, SIGNAL(timeout()), this, SLOT(enviar(void)));
}

Conexao::~Conexao(void)
{
    delete timer;
    delete socket;
}

void Conexao::abrir(QString &ip, unsigned int porta)
{
    socket->connectToHost(ip, porta);

    /* Aguarda 3 segundos pela conexão. Caso não conecte, gere a exceção. */
    if(!socket->waitForConnected(30000))
    {
        throw ConexaoNaoEstabelecida("Erro na conexão: " + socket->errorString());
    }

    ativa = true;
}

int Conexao::numero_aleatorio(int min, int max)
{
    return ((float(rand()) / float(RAND_MAX)) * (max - min)) + min;
}

void Conexao::fechar()
{
    socket->close();
    ativa = false;
}

void Conexao::enviarDados(int _faixaInicio, int _faixaFim, int _intervalo)
{
    faixaInicio = _faixaInicio;
    faixaFim = _faixaFim;
    intervalo = _intervalo;

    /* intervalo vem em segundos, logo converta para milisegundos. */
    timer->start(intervalo * 100);
}

void Conexao::pararEnvio()
{
    if (timer->isActive())
    {
        timer->stop();
    }
}

void Conexao::enviar(void)
{
    QString dadoLog, dadoEnvio;
    QDateTime datetime;

    /* Preparando a faixa de valores. */

    datetime = QDateTime::currentDateTime();
    dadoLog = datetime.toString(Qt::ISODate)
            + " " + QString::number(numero_aleatorio(faixaInicio, faixaFim));
    dadoEnvio = "set "+ dadoLog + "\r\n";

    if (socket->isOpen())
    {
        socket->write(dadoEnvio.toStdString().c_str());
        socket->waitForBytesWritten(3000);
    }

    emit dadoEnviado(dadoLog);

}

void Conexao::alterarIntervalo(int intervalo)
{
    timer->stop();
    timer->start(intervalo * 100);
}

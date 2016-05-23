/**
 * @file conexao.cpp
 * @author Denis Ricardo da Silva Medeiros
 * @date 10 May 2016
 * @brief Classe responsável pela conexao entre o programa e
 * o servidor remoto.
 */
#include "conexao.h"
#include <QDateTime>


int Conexao::numero_aleatorio(int min, int max)
{
    return ((float(rand()) / float(RAND_MAX)) * (max - min + 2)) + min - 1;
}


Conexao::Conexao(void) : QObject()
{
    /* Inicializa os objetos e variáveis. */
    socket = new QTcpSocket();
    timer = new QTimer();

    faixaInicio = 0;
    faixaFim = 0;
    intervalo = 0;

    /* Usa a hora atual como seed para os números aleatórios. */
    qsrand(QDateTime::currentDateTime().toTime_t());

    /* Conecta o timer ao slot que envia os dados para o servidor. */
    connect(timer, SIGNAL(timeout()), this, SLOT(enviar(void)));
}


Conexao::~Conexao(void)
{
    /* Antes de limpar a memória, pare o timer e feche a conexão. */

    if(timer->isActive())
    {
        timer->stop();
    }

    if(socket->isOpen())
    {
        socket->close();
    }

    delete timer;
    delete socket;
}

void Conexao::setFaixaInicio(int _faixaInicio)
{
    faixaInicio = _faixaInicio;
}

void Conexao::setFaixaFim(int _faixaFim)
{
    faixaFim = _faixaFim;
}

void Conexao::setIntervalo( int _intervalo)
{
    intervalo = _intervalo;

    /* Se o timer estiver ativo, altere o seu valor. */
    if(timer->isActive())
    {
        timer->setInterval(intervalo * 1000);
    }
}

void Conexao::abrir(QString &ip, unsigned int porta)
{
    socket->connectToHost(ip, porta);

    /* Aguarda 3 segundos pela conexão. Caso não conecte, gere a exceção. */
    if(!socket->waitForConnected(3000))
    {
        throw ErroConexao("Erro na abertura da conexão: " + socket->errorString());
    }

}


void Conexao::fechar()
{
    if(socket->isOpen())
    {
        socket->close();
    }
}


bool Conexao::isAtiva(void)
{
    /* Checa se o socket está aberto e conectado. */
    return (socket->isOpen() && socket->state() == QTcpSocket::ConnectedState);
}


void Conexao::iniciarEnvio(void)
{
    /* O intervalo está em segundos, logo converta para milisegundos. */
    timer->start(intervalo * 1000);
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

    /* Preparando o dado a ser enviado. */
    datetime = QDateTime::currentDateTime();
	
	/* Este dado está formatado para ser enviado via signal para a MainWindow. */
    dadoLog = datetime.toString(Qt::ISODate)
            + " " + QString::number(numero_aleatorio(faixaInicio, faixaFim));
			
	/* Este comando está formatado para ser enviado para o servidor (ele contem o dado acima). */
    dadoEnvio = "set "+ dadoLog + "\r\n";

    if (this->isAtiva())
    {
        socket->write(dadoEnvio.toStdString().c_str());
		if(!socket->waitForBytesWritten(3000))
		{
            qDebug() << "Falha durante o envio.";
		}
    } else {
        qDebug() << "Falha durante o envio.";
    }
	
	/* Emite o sinal para a MainWindow mostrar o dado enviado na lista de logs. */
    emit dadoEnviado(dadoLog);

}

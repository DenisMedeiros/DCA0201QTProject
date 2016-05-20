#include "conexao.h"
#include "conexaonaoestabelecida.h"
#include <QStringList>

Conexao::Conexao(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket();

    connect(socket, SIGNAL(disconnected()), this, SLOT(avisaFalhaConexao()));
}

Conexao::~Conexao()
{
    if(socket->isOpen())
    {
        socket->close();
    }
    delete socket;
}

void Conexao::abrir(QString ip, unsigned int porta)
{
    socket->connectToHost(ip, porta);

    /* Aguarda 3 segundos pela conexão. Caso não conecte, gere a exceção. */
    if(!socket->waitForConnected(3000))
    {
        throw ConexaoNaoEstabelecida("Erro na conexão: " + socket->errorString());
    }

}

void Conexao::fechar()
{
    if(socket->isOpen())
    {
        socket->close();
    }
}

QStringList Conexao::getClientes()
{
    QString linha;
    QStringList clientes;

    if(isAtiva())
    {

      socket->write("list\r\n");
      socket->waitForBytesWritten(3000);
      socket->waitForReadyRead(3000);

      while(socket->bytesAvailable())
      {
        linha = socket->readLine().replace("\n","").replace("\r","");
        clientes.append(linha);
      }
    } else {
        throw ConexaoNaoEstabelecida("Erro na conexão: Servidor parou de responder.");
    }

    return clientes;
}

QList<Dado> Conexao::getDados(QString cliente)
{

    QList<Dado> dados;
    QString linha, comando, datetimeStr, valorStr;
    QStringList datetimeValor;
    QDateTime datetime;
    int valor;
    Dado dado;

    comando = "get " + cliente + "\r\n";

    if(isAtiva())     
    {
        socket->write(comando.toStdString().c_str());
        socket->waitForBytesWritten(3000);
        socket->waitForReadyRead(3000);

        while(socket->bytesAvailable())
        {
            linha = socket->readLine().replace("\n","").replace("\r","");

            /* O dado está n oformato '2016-05-19T08:21:58 8'. */
            datetimeValor = linha.split(" ");

            if(datetimeValor.size() == 2)
            {
                datetimeStr = datetimeValor.at(0);
                valorStr = datetimeValor.at(1);

                datetime = QDateTime::fromString(datetimeStr, Qt::ISODate);
                valor = valorStr.toInt();

                dado.datetime = datetime;
                dado.valor = valor;

                dados.append(dado);

            }
        }
    }
    else
    {
        throw ConexaoNaoEstabelecida("Erro na conexão: Servidor parou de responder.");
    }

    return dados;
}

bool Conexao::isAtiva()
{
    /* Checa se o socket está aberto e conectado. */
    return (socket->isOpen() && socket->state() == QTcpSocket::ConnectedState);
}


void Conexao::avisaFalhaConexao()
{
    emit falhaConexao();
}

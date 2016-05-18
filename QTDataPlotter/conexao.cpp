#include "conexao.h"
#include "conexaonaoestabelecida.h"

Conexao::Conexao(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket();
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

    if(socket->isOpen()){

      socket->write("list\r\n");
      socket->waitForBytesWritten(3000);
      socket->waitForReadyRead(3000);

      while(socket->bytesAvailable())
      {
        linha = socket->readLine().replace("\n","").replace("\r","");
        clientes.append(linha);
      }
    } else {
        throw ConexaoNaoEstabelecida();
    }

    return clientes;
}

QStringList Conexao::getDados(QString clienteIP)
{

    QStringList dados;
    /*
    QString str;
    QByteArray array;
    QStringList list;
    QDateTime datetime;

    if(socket->isOpen()){
      socket->write("get 127.0.0.1\r\n");
      socket->waitForBytesWritten(3000);
      socket->waitForReadyRead(3000);
      qDebug() << socket->bytesAvailable();
      while(socket->bytesAvailable()){
        str = socket->readLine().replace("\n","").replace("\r","");
        list = str.split(" ");
        if(list.size() == 2){
          datetime.fromString(list.at(0),Qt::ISODate);
          str = list.at(1);
          qDebug() << datetime << ": " << str;
        }
      }
    }*/

    return dados;
}

bool Conexao::isAtivada()
{
    return socket->isOpen();
}

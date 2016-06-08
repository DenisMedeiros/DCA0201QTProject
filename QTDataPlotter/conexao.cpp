#include "conexao.h"

Conexao::Conexao(QObject *parent) : QObject(parent)
{
    /* Cria o socket e faz a conexão do seu sinal de 'disconnect' com o slot especificado. */
    socket = new QTcpSocket();
    socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    connect(socket, SIGNAL(disconnected()), this, SLOT(emiteFalhaConexao()));
}

Conexao::~Conexao()
{
    /* Fecha o socket se ele estiver aberto e libera sua memória. */
    if(socket->isOpen())
    {
        socket->close();
    }

    delete socket;
}

void Conexao::abrir(const QString &ip, quint16 porta)
{
    /* Tenta abrir a conexão por 3 segundos. Se falhar, gera uma exceção. */
    socket->connectToHost(ip, porta);

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

bool Conexao::isAtiva() const
{
    /* Checa se o socket está aberto e conectado. */
    return (socket->isOpen() && socket->state() == QTcpSocket::ConnectedState);
}


void Conexao::emiteFalhaConexao()
{
    emit falhaConexao("Erro na conexão: o servidor parou de responder.");
}

#include "conexao.h"

Conexao::Conexao(void)
{
    socket = new QTcpSocket();
    ativa = false;
}

Conexao::~Conexao(void)
{
    delete socket;
}

void Conexao::abrir(QString &ip, unsigned int porta)
{
    socket->connectToHost(ip, porta);

    /* Aguarda 3 segundos pela conexão. Caso não conecte, gere a exceção. */
    if(!socket->waitForConnected(30000))
    {
      throw ConexaoNaoEstabelecida();
    }

    ativa = true;
}

void Conexao::fechar()
{
    socket->close();
    ativa = false;
}

bool Conexao::isAtiva()
{
    return ativa;
}

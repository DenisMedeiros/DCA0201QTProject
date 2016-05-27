#include "conexaosender.h"
#include <QDateTime>

ConexaoSender::ConexaoSender() : Conexao()
{
}

ConexaoSender::~ConexaoSender()
{
}

void ConexaoSender::enviar(const QString &dado) const
{
    QString comando;

    if (isAtiva())
    {

        /* Formata o comando e faz seu envio para o servidor. */

        comando = "set "+ dado + "\r\n";

        socket->write(comando.toStdString().c_str());
        if(!socket->waitForBytesWritten(3000))
        {
            throw ErroConexao("Erro na conexão: O servidor não aceitou esta requisição.");
        }
    } else {
        throw ErroConexao("Erro na conexão: O servidor não está mais ativo.");
    }

}

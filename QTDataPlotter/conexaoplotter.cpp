#include "conexaoplotter.h"

ConexaoPlotter::ConexaoPlotter(void) : Conexao()
{

}

ConexaoPlotter::~ConexaoPlotter()
{

}

QStringList ConexaoPlotter::getClientes(void)
{
    QString linha, comando;
    QStringList clientes;

    comando = "list\r\n";

    if(isAtiva())
    {
        /* Envia o comando para o servidor.*/

        socket->write(comando.toStdString().c_str());
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

QList<Dado> ConexaoPlotter::getDados(QString cliente)
{

    QList<Dado> dados;
    QString linha, comando, datetimeStr, valorStr;
    QStringList datetimeValor;
    QDateTime datetime;
    Dado dado;
    int valor;

    comando = "get " + cliente + "\r\n";

    if(isAtiva())
    {
        socket->write(comando.toStdString().c_str());
        socket->waitForBytesWritten(3000);
        socket->waitForReadyRead(3000);

        while(socket->bytesAvailable())
        {
            linha = socket->readLine().replace("\n","").replace("\r","");

            /* O dado está no formato '2016-05-19T08:21:58 8'. */
            datetimeValor = linha.split(" ");

            if(datetimeValor.size() == 2)
            {
                datetimeStr = datetimeValor.at(0);
                valorStr = datetimeValor.at(1);

                /* Transforme as strings nos tipos de dados corretos, construa o dado e adicione-o na lista de retorno. */
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

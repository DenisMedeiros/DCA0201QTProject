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
        /* Envia o comando para o servidor, com tolerância de 3 segundos. */
        socket->write(comando.toStdString().c_str());
        socket->waitForBytesWritten(3000);
        socket->waitForReadyRead(3000);

        while(socket->bytesAvailable())
        {
            linha = socket->readLine().replace("\n","").replace("\r","");

            if (linha.isEmpty())
            {
                throw ErroConexao("Erro na conexão: Nenhum cliente conectou-se ao servidor ainda.");
            }

            clientes.append(linha);
        }
    } else {
        throw ErroConexao("Erro na conexão: O servidor parou de responder.");
    }

    return clientes;
}

QList<Dado> ConexaoPlotter::getTodosDados(QString cliente)
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

            /* Faz o tratamento dos dados e adiciona-o na lista de retorno. */
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
        throw ErroConexao("Erro na conexão: O servidor parou de responder.");
    }

    return dados;
}

QList<Dado> ConexaoPlotter::getUltimos30Dados(QString cliente)
{
    QList<Dado> dados;
    QString linha, comando, datetimeStr, valorStr;
    QStringList datetimeValor, todosDadosStr, ultimos30DadosStr;
    QDateTime datetime;
    Dado dado;
    int valor;

    comando = "get " + cliente + "\r\n";

    if(isAtiva())
    {

        socket->write(comando.toStdString().c_str());
        socket->waitForBytesWritten(3000);
        socket->waitForReadyRead(3000);

        /* Primeiro lê todos os dados e trate-os como QString. */
        while(socket->bytesAvailable())
        {
            linha = socket->readLine().replace("\n","").replace("\r","");
            todosDadosStr.append(linha);
        }

        /* Obtém os últimos 20 dados. */
        if(todosDadosStr.size() <= 30)
        {
            ultimos30DadosStr = todosDadosStr;
        }
        else
        {
            ultimos30DadosStr = todosDadosStr.mid(todosDadosStr.size()-30);
        }

        /* Faz o tratamento desses últimos 20 dados. */
        foreach(QString dadoStr, ultimos30DadosStr)
        {
            /* O dado está no formato '2016-05-19T08:21:58 8'. */
            datetimeValor = dadoStr.split(" ");

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
        throw ErroConexao("Erro na conexão: O servidor parou de responder.");
    }

    return dados;
}

/**
 * @file conexao.cpp
 * @author Denis Ricardo da Silva Medeiros
 * @date 10 May 2016
 * @brief Classe responsável pela conexao entre o programa e
 * o servidor remoto.
 */
#include "conexao.h"
#include <QDateTime>


/**
 * @brief Este é o construtor padrão desta classe.
 *
 * Ele instancia os objeto socket e timer; prepara a geração de números
 * aleatórios e conecta alguns sinais a seus respectivos slots.
 */
Conexao::Conexao(void) : QObject()
{

    socket = new QTcpSocket();
    timer = new QTimer();

    /* Usa a hora atual como seed para os números aleatórios. */
    qsrand(QDateTime::currentDateTime().toTime_t());

    /* Conecta o timer ao slot que envia os dados para o servidor. */
    connect(timer, SIGNAL(timeout()), this, SLOT(enviar(void)));
}

/**
 * @brief Este é o destrutor padrão desta classe.
 *
 * Ele checa se tanto o timer quanto o socket ainda estão ativos e, após isso,
 * libera a memória alocada por esses objetos.
 *
 */
Conexao::~Conexao(void)
{
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

/**
 * @brief Este método abre a conexão com o servidor removo.
 *
 * Ele abre a conexao com o servidor remoto, que é identificado
 * pelo ip e pela porta na qual os serviço estará
 * recebendo as requisições.
 *
 * @param ip Endereço IP do servidor.
 * @param porta Porta TCP por onde o servidor recebe as conexões.
 *
 * @exception ConexaoNaoEstabelecida Caso haja um erro na criação do socket de comunicação.
 */
void Conexao::abrir(QString &ip, unsigned int porta)
{
    socket->connectToHost(ip, porta);

    /* Aguarda 3 segundos pela conexão. Caso não conecte, gere a exceção. */
    if(!socket->waitForConnected(30000))
    {
        throw ConexaoNaoEstabelecida("Erro na conexão: " + socket->errorString());
    }

}

/**
 * @brief Este método fecha a conexão com o servidor removo.
 *
 * Ele fecha a conexão com o servidor remoto, parando o timer e
 * fechando o socket de comunicação.
 *
 */
void Conexao::fechar()
{
    socket->close();
}

/**
 * @brief Este método checa se a conexão está ativa.
 *
 * Ele checa se a conexão continua ativada verificando se o socket
 * de comunicação continua aberto com o servidor remoto.
 *
 * @return true Se o socket estiver aberto;
 *         false Se o socket estiver fechado.
 */
bool Conexao::isAtiva(void)
{
    return socket->isOpen();
}

/**
 * @brief Este método inicia o processo de envio de dados.
 *
 * Ele altera todos os parâmetros acerca da geração dos valores
 * e inicia o timer responsável por ativar o envio de dados para o servidor
 * remoto no período determinado pelo usuário.
 *
 * @param _faixaInicio Menor valor do intervalo que o dado pode assumir.
 * @param _faixaFim Maior valor do intervalo que o dado pode assumir.
 * @param _intervalo Intervalo de tempo em que os dados serão ferados e enviados.
 *
 */
void Conexao::enviarDados(int _faixaInicio, int _faixaFim, int _intervalo)
{
    faixaInicio = _faixaInicio;
    faixaFim = _faixaFim;
    intervalo = _intervalo;

    /* intervalo vem em segundos, logo converta para milisegundos. */
    timer->start(intervalo * 100);
}

/**
 * @brief Este método para o envio de dados.
 *
 * Ele basicamente para o timer, que é responsável por disparar o sinal
 * que chama a função que envia o dado para o servidor.
 *
 * @return true Se o socket estiver aberto;
 *         false Se o socket estiver fechado.
 */
void Conexao::pararEnvio()
{
    if (timer->isActive())
    {
        timer->stop();
    }
}

/**
 * @brief Este slot é responsável por gerar e enviar o dado para o servidor.
 *
 * Uma vez que o timer disparou o sinal e este slot foi invocado, ele gerará o dado
 * a ser enviado para o servidor com base nos parâmetros de faixas estabelecidos e
 * utilizará o socket de comunicação para realizar esta ação.
 *
 */
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

/**
 * @brief Este slot é responsável por alterar o intervalo de tempo da geração dos dados.
 *
 * Ele é ativado quando o HorizontalSlider do intervalo tem seu valor mudado e, com isso,
 * ele para e reinicia o timer o novo valor estabelecido.
 *
 */
void Conexao::alterarIntervalo(int intervalo)
{
    timer->stop();
    timer->start(intervalo * 100);
}

/**
 * @brief Este método auxiliar gera um número aleatório inteiro entre e inclusive
 * min e max.
 *
 * @param min Valor mínimo para geração do número inteiro.
 * @param max Valor máximo para a geração do número inteiro.
 *
 * @return Um número aleatório entre e inclusive min e max.
 */
int Conexao::numero_aleatorio(int min, int max)
{
    return ((float(rand()) / float(RAND_MAX)) * (max - min)) + min;
}

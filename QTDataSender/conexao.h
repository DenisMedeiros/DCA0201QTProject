/**
 * @file conexao.h
 * @author Denis Ricardo da Silva Medeiros
 * @date 10 May 2016
 * @brief Classe responsável pela conexao entre o programa e
 * o servidor remoto.
 */

#ifndef CONEXAO_H
#define CONEXAO_H

#include <QObject>
#include <QTcpSocket>
#include <QString>
#include <QTimer>
#include "conexaonaoestabelecida.h"

/**
 * @brief Esta é a classe responsável pela conexão entre o cliente
 * e o servidor remoto.
 *
 * Ela representa conexão entre o cliente o servidor remoto. Para isso,
 * ela utiliza o IP e portas fornecidos pelo usuário para realizar a conexão.
 *
 */
class Conexao : public QObject
{
    Q_OBJECT

private:

    /** @brief Socket utilizado na comunicação. */
    QTcpSocket *socket;

    /** @brief Timer utilizado para enviar os dados em um período definido. */
    QTimer *timer;

    /** @brief Valor mínimo que pode ser gerado e enviado para os servidor. */
    int faixaInicio;

    /** @brief Valor máximo que pode ser gerado e enviado para os servidor. */
    int faixaFim;

    /** @brief Período de tempo para o envio períodico de dados para o servidor. */
    int intervalo;

    /**
     * @brief Este método auxiliar gera um número aleatório inteiro entre
     * min e max (eles não são incluídos).
     *
     * @param min Valor mínimo para geração do número inteiro.
     * @param max Valor máximo para a geração do número inteiro.
     *
     * @return Um número aleatório entre e inclusive min e max.
     */
    int numero_aleatorio(int min, int max);

public:
    /**
     * @brief Este é o construtor padrão desta classe.
     *
     * Ele instancia os objeto socket e timer; prepara a geração de números
     * aleatórios e conecta alguns sinais a seus respectivos slots.
     */
    Conexao(void);

    /**
     * @brief Este é o destrutor padrão desta classe.
     *
     * Ele checa se tanto o timer quanto o socket ainda estão ativos e, após isso,
     * libera a memória alocada por esses objetos.
     *
     */
    ~Conexao(void);

    /**
     * @brief Este método altera a faixa inicial dos dados.
     *
     * @param _faixaInicio Menor valor do intervalo que o dado pode assumir.
     */
    void setFaixaInicio(int _faixaInicio);

    /**
     * @brief Este método altera a faixa final dos dados.
     *
     * @param _faixaFim Menor valor do intervalo que o dado pode assumir.
     */
    void setFaixaFim(int _faixaFim);

    /**
     * @brief Este método altera o intervalo de envio dos dados.
     *
     * @param _intervalo Intervalo de tempo em que os dados serão enviados.
     */
    void setIntervalo(int _intervalo);

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
    void abrir(QString &ip, unsigned int porta);

    /**
     * @brief Este método fecha a conexão com o servidor removo.
     *
     * Ele fecha a conexão com o servidor remoto, parando o timer e
     * fechando o socket de comunicação.
     *
     */
    void fechar(void);

    /**
     * @brief Este método checa se a conexão está ativa.
     *
     * Ele checa se a conexão continua ativada verificando se o socket
     * de comunicação continua aberto com o servidor remoto.
     *
     * @return true Se o socket estiver aberto;
     *         false Se o socket estiver fechado.
     */
    bool isAtiva(void);

    /**
     * @brief Este método inicia o processo de envio de dados.
     *
     * Ele inicia o timer responsável por ativar o envio de dados para o servidor
     * remoto, no período identificado pelo atributo 'intervalo'.
     *
     */
    void iniciarEnvio(void);


    /**
     * @brief Este método para o envio de dados.
     *
     * Ele basicamente para o timer, que é responsável por disparar o sinal
     * que chama a função que envia o dado para o servidor.
     *
     * @return true Se o socket estiver aberto;
     *         false Se o socket estiver fechado.
     */
    void pararEnvio(void);


public slots:
    /**
     * @brief Este slot é responsável por gerar e enviar o dado para o servidor.
     *
     * Uma vez que o timer disparou o sinal e este slot foi invocado, ele gerará o dado
     * a ser enviado para o servidor com base nos parâmetros de faixas estabelecidos e
     * utilizará o socket de comunicação para realizar esta ação.
     *
     */
    void enviar(void);

signals:
    /**
     * @brief Este sinal é disparado quando um dado foi enviado com sucesso para o servidor.
     *
     * Ele é utilizado para informar à outras classes sobre o sucesso do envio,
     * principalmente à MainWindow, para que esta exiba na tela o dado enviado como forma de log.
     *
     * @param dado O dado que foi enviado com sucesso.
     *
     * @return QString O dado que foi enviado com sucesso.
     */
    QString dadoEnviado(QString dado);

    QString falhaDuranteEnvio(void);


};

#endif // CONEXAO_H

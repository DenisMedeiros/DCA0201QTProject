/**
 * @file conexao.h
 * @author Denis Ricardo da Silva Medeiros
 * @date 18 May 2016
 * @brief Classe responsável pela conexao entre o programa e
 * o servidor remoto.
 */

#ifndef CONEXAO_H
#define CONEXAO_H

#include <QObject>
#include <QTcpSocket>
#include <QString>
#include <QList>
#include <QDateTime>
#include "erroconexao.h"

/**
 * @brief Esta estrutura serve para agrupar os dados para facilitar a plotagem
 * em um plano cartesiano.
 *
 * O datetime será tratado como o 'x' e o valor será tratado como o 'y'.
 *
 */
struct Dado {
    QDateTime datetime;
    int valor;
};

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
protected:
    /** @brief Socket utilizado na comunicação. */
    QTcpSocket *socket;

public:
    /**
     * @brief Este é o construtor padrão desta classe.
     *
     * Ele instancia os objeto socket e também interliga os principais
     * sinais e slots.
     */
    explicit Conexao(QObject *parent = 0);

    /**
     * @brief Este é o destrutor padrão desta classe.
     *
     * Ele checa se o socket ainda estão ativo e, após isso,
     * libera a memória alocada por esses objetos.
     *
     */
    ~Conexao(void);

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
    void abrir(QString &ip, quint16 porta);

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
     * de comunicação continua aberto com o servidor remoto, além de verificar
     * o status atual da conexão.
     *
     * @return true Se o socket estiver aberto;
     *         false Se o socket estiver fechado.
     */
    bool isAtiva(void);

signals:
    /**
     * @brief Este sinal é emitido quando um erro ocorre com socket de forma assíncrona.
     *
     * A ideia é que este sinal seja emitido de forma assíncrona quando a conexão com o servidor cair.
     * Para isso, ele trabalha junto com o slot 'emiteFalhaConexao', que está conectado com o o sinal
     * do socket chamado de 'disconnected'.
     *
     * Na prática, quando ocorrer um erro com o socket, ele emitirá um sinal a ser capturado pelo slot
     * 'emiteFalhaConexao' e este, finalmente, emite o sinal 'falhaConexao' para ser capturado por outro QObject
     * avisando que ocorreu uma falha na conexão.
     *
     */
    void falhaConexao(void);

public slots:
    /**
     * @brief Este slot detecta quando o socket perde a conexão com o servidor remoto.
     *
     * Em sincronia com o sinal 'falhaConexao', este socket emite aquele sinal para avisar
     * outros QObjects que ocorreu um erro na comunicação com o servidor.
     *
     */
    void emiteFalhaConexao(void);
};

#endif // CONEXAO_H

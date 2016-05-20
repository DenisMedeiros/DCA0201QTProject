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
#include <QTimer>

/**
 * @brief Esta estrutura serve para agrudar os dados para facilitar a plotagem
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
private:
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
    void abrir(QString &ip, unsigned int porta);

    /**
     * @brief Este método fecha a conexão com o servidor removo.
     *
     * Ele fecha a conexão com o servidor remoto, parando o timer e
     * fechando o socket de comunicação.
     *
     */
    void fechar(void);


    QStringList getClientes(void);
    QList<Dado> getDados(QString cliente);

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
    void falhaConexao(void);

public slots:
    void avisaFalhaConexao(void);
};

#endif // CONEXAO_H

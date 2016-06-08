/**
 * @file conexaoplotter.h
 * @author Denis Ricardo da Silva Medeiros
 * @date 18 May 2016
 * @brief Classe responsável pela conexao entre o programa e
 * o servidor remoto, com funções mais específicas.
 */

#ifndef CONEXAOPLOTTER_H
#define CONEXAOPLOTTER_H

#include "conexao.h"
#include <QString>
#include <QList>

/**
 * @brief Esta é a classe responsável pela conexão entre o cliente
 * e o servidor remoto e por mais funções específicas.
 *
 * Ela herda todas as funções básicas de gerenciamento da conexão da classe
 * Conexao e possui algumas peculiaridades para este programa.
 *
 */
class ConexaoPlotter : public Conexao
{
private:

public:
    /**
     * @brief Este é o construtor padrão desta classe.
     *
     * Ele basicamente chama o constructor da classe Conexao,
     * que já prepara o socket de comunicação automaticamente.
     */
    ConexaoPlotter(void);

    /**
     * @brief Este é o destrutor padrão desta classe.
     *
     * Ele não faz nenhuma operação relevante.
     *
     */
    ~ConexaoPlotter(void);

    /**
     * @brief Este método retorna a lista dos clientes conectados ao servidor.
     *
     * @return Lista de clientes conectados ao servidor.
     *
     */
    QStringList getClientes(void);

    /**
     * @brief Este método retorna apenas os últimos 30 dados enviados pelo cliente.
     *
     * Os dados retornos já estão devidamente tratados e pronto para uso.
     *
     * @param cliente IP do cliente que se deseja obter os dados enviados.
     *
     * @return Lista com os dados enviados por aquele cliente para o servidor.
     */
    QList<Dado> getUltimos30Dados(QString cliente);
};

#endif // CONEXAOPLOTTER_H

/**
 * @file conexaosender.h
 * @author Denis Ricardo da Silva Medeiros
 * @date 27 May 2016
 * @brief Classe responsável pela conexao entre o programa e
 * o servidor remoto, com funções mais específicas.
 */

#ifndef CONEXAOSENDER_H
#define CONEXAOSENDER_H

#include "conexao.h"

/**
 * @brief Esta é a classe responsável pela conexão entre o cliente
 * e o servidor remoto e por mais funções específicas.
 *
 * Ela herda todas as funções básicas de gerenciamento da conexão da classe
 * Conexao e possui algumas peculiaridades para este programa.
 *
 */
class ConexaoSender : public Conexao
{
public:
    /**
    * @brief Este é o construtor padrão desta classe.
    *
    * Ele basicamente chama o constructor da classe Conexao,
    * que já prepara o socket de comunicação automaticamente.
    */
    ConexaoSender(void);

    /**
     * @brief Este é o destrutor padrão desta classe.
     *
     * Ele não faz nenhuma operação relevante.
     *
     */
    ~ConexaoSender(void);

    /**
     * @brief Esta função faz o envio do dado para o servidor remoto.
     *
     * Ele envia o dado no seu formato original, logo ele precisa vir devidamente tratado.
     *
     * @param dado O dado (data/hora e valor) a ser enviado para o servidor.
     *
     */
    void enviar(const QString &dado) const;
};

#endif // CONEXAOSENDER_H

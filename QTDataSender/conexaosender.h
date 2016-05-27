#ifndef CONEXAOSENDER_H
#define CONEXAOSENDER_H

#include "conexao.h"

class ConexaoSender : public Conexao
{
public:
    ConexaoSender();
    ~ConexaoSender();
    void enviar(const QString &dado);
};

#endif // CONEXAOSENDER_H

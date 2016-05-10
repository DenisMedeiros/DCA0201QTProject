#ifndef CONEXAO_H
#define CONEXAO_H

#include <QObject>
#include <QTcpSocket>
#include <QString>

#include "conexaonaoestabelecida.h"

class Conexao
{

private:
    QTcpSocket *socket;
    bool ativa;

public:
    Conexao(void);
    ~Conexao(void);
    void abrir(QString &, unsigned int);
    void fechar(void);
    bool isAtiva(void);
};

#endif // CONEXAO_H

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
 * Esta é a classe responsável pela conexão entre o cliente
 * e o servidor remoto. Ela utiliza o IP e portas fornecidos pelo
 * usuário para realizar a conexão.
 *
 */
class Conexao : public QObject
{
    Q_OBJECT

private:
    /** */
    QTcpSocket *socket;
    QTimer *timer;
    bool ativa;
    int faixaInicio, faixaFim, intervalo;
    int numero_aleatorio(int, int);

public:
    Conexao(void);
    ~Conexao(void);

    void abrir(QString &, unsigned int);
    void fechar(void);
    bool isAtiva(void);
    void enviarDados(int, int, int);
    void pararEnvio(void);


public slots:
    void enviar(void);
    void alterarIntervalo(int intervalo);

signals:
    QString dadoEnviado(QString);
};

#endif // CONEXAO_H

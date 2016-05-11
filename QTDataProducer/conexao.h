#ifndef CONEXAO_H
#define CONEXAO_H

#include <QObject>
#include <QTcpSocket>
#include <QString>
#include <QTimer>
#include "conexaonaoestabelecida.h"

class Conexao : public QObject
{
    Q_OBJECT

private:
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

signals:
    QString dadoEnviado(QString);
};

#endif // CONEXAO_H

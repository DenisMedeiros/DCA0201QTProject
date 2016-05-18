#ifndef CONEXAO_H
#define CONEXAO_H

#include <QObject>
#include <QStringList>
#include <QTcpSocket>
#include <QString>

class Conexao : public QObject
{
    Q_OBJECT
private:

    /** @brief Socket utilizado na comunicação. */
    QTcpSocket *socket;
public:
    explicit Conexao(QObject *parent = 0);
    ~Conexao(void);
    void abrir(QString ip, unsigned int porta);
    void fechar(void);
    QStringList getClientes(void);
    QStringList getDados(QString clienteIP);
    bool isAtivada(void);

signals:

public slots:
};

#endif // CONEXAO_H

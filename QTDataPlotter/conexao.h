#ifndef CONEXAO_H
#define CONEXAO_H

#include <QObject>
#include <QTcpSocket>
#include <QString>
#include <QList>
#include <QDateTime>
#include <QTimer>

struct Dado {
    QDateTime datetime;
    int valor;
};

class Conexao : public QObject
{
    Q_OBJECT
private:

    /** @brief Socket utilizado na comunicação. */
    QTcpSocket *socket;
    QTimer *timer;
public:
    explicit Conexao(QObject *parent = 0);
    ~Conexao(void);
    void abrir(QString ip, unsigned int porta);
    void fechar(void);
    QStringList getClientes(void);
    QList<Dado> getDados(QString cliente);
    bool isAtiva(void);

signals:

public slots:
};

#endif // CONEXAO_H

#ifndef GRAFICO_H
#define GRAFICO_H

#include <QWidget>
#include <QPaintEvent>
#include <QList>
#include <conexao.h>

class Grafico : public QWidget
{
    Q_OBJECT

private:
    QList<Dado> dados;
    int menorY;
    int maiorY;
public:
    explicit Grafico(QWidget *parent = 0);
    void setDados(QList<Dado> &_dados);
    void setMenorMaiorY(int _menorY, int _maiorY);
signals:

protected:
    void paintEvent(QPaintEvent *e);

public slots:

};

#endif // GRAFICO_H

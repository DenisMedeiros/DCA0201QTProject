#include "grafico.h"
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QColor>
#include <QDateTime>
#include <cmath>

Grafico::Grafico(QWidget *parent) : QWidget(parent)
{
    /* Inicializa as variáveis e instancia o objeto. */
    menorY = maiorY = 0;
    dados = new QList<Dado>();
}

Grafico::~Grafico()
{
    delete dados;
}

int Grafico::getMenorY() const
{
    return menorY;
}

int Grafico::getMaiorY() const
{
    return maiorY;
}

void Grafico::setDados(const QList<Dado> &_dados)
{
    *dados = _dados;
}

void Grafico::setMenorY(int _menorY)
{
    menorY = _menorY;
}

void Grafico::setMaiorY(int _maiorY)
{
    maiorY = _maiorY;
}

void Grafico::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    QBrush brush;
    QPen pen;
    float proporcaoY;
    int tamanho, xi, yi, xf, yf;

    tamanho = dados->size();

    painter.setRenderHint(QPainter::Antialiasing);

    /* Prepara o brush e o pen para desenhar as linhas na tela. */
    pen.setColor(QColor(0, 0, 0));
    painter.setPen(pen);

    brush.setColor(QColor(255, 255, 255));
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);

    /* Desenha o fundo da tela. */
    painter.drawRect(0, 0, width(), height());

    /* Calcula a proporção do tamanho de cada intervalo no eixo y. */
    proporcaoY = height()/((float) (maiorY - menorY));

    /* Desenha as retas no gráfico. */

    xi = 0;
    yi = qRound(height() - (dados->at(0).valor - menorY) * proporcaoY);

    for(int i = 1; i < tamanho; i++)
    {
        xf = qRound(i * width()/((float) tamanho-1));
        yf = qRound(height() - (dados->at(i).valor - menorY) * proporcaoY);
        painter.drawLine(xi, yi, xf, yf);
        xi = xf;
        yi = yf;
    }
}


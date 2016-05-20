#include "grafico.h"
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QColor>
#include <QDateTime>
#include <cmath>

Grafico::Grafico(QWidget *parent) : QWidget(parent)
{
    menorY = maiorY = 0;
}

void Grafico::setDados(QList<Dado> &_dados)
{
    dados = _dados;
}

void Grafico::setMenorY(int _menorY)
{
    menorY = _menorY;
}

void Grafico::setMaiorY(int _maiorY)
{
    maiorY = _maiorY;
}

int Grafico::getMenorY()
{
    return menorY;
}

int Grafico::getMaiorY()
{
    return maiorY;
}

void Grafico::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    QBrush brush;
    QPen pen;
    double proporcaoY;
    int tamanho, xi, yi, xf, yf;

    painter.setRenderHint(QPainter::Antialiasing);

    pen.setColor(QColor(0, 0, 0));
    painter.setPen(pen);

    brush.setColor(QColor(255, 255, 255));
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);

    /* Desenha o fundo da tela. */
    painter.drawRect(0, 0, width(), height());

    /* Normaliza os dados entre 0 e 1. */

    proporcaoY = height()/((float) (maiorY - menorY));

    xi = 0;
    yi = qRound(height() - (dados[0].valor - menorY) * proporcaoY);
    tamanho = dados.size();

    for(int i = 1; i < tamanho; i++)
    {
        xf = qRound(i * width()/((float) tamanho-1));
        yf = qRound(height() - (dados[i].valor - menorY) * proporcaoY);
        painter.drawLine(xi, yi, xf, yf);
        xi = xf;
        yi = yf;
    }
}


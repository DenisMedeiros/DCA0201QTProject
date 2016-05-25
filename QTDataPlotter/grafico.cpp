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

    /* Verifica se já existem dados atribuídos. */
    if(tamanho == 0)
    {
        return;
    }

    painter.setRenderHint(QPainter::Antialiasing);

    /* Prepara o brush. */

    brush.setColor(QColor(255, 255, 255));
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);

    /* Desenha o fundo da tela. */
    painter.setPen(QPen(Qt::black, 1));
    painter.drawRect(0, 0, width(), height());

    /* Calcula a proporção do tamanho de cada intervalo no eixo y. */
    proporcaoY = height()/((float) (maiorY - menorY));

    xi = 0;
    yi = qRound(height() - (dados->at(0).valor - menorY) * proporcaoY);

    /* Desenha o ponto. */

    painter.setPen(QPen(Qt::blue, 5));
    painter.drawPoint(xi, yi);

    /* Desenha as retas no gráfico. */


    for(int i = 1; i < tamanho; i++)
    {
        xf = qRound(i * width()/((float) tamanho-1));
        yf = qRound(height() - (dados->at(i).valor - menorY) * proporcaoY);

        painter.setPen(QPen(Qt::blue, 5));
        painter.drawPoint(xi, yi);
        painter.drawPoint(xf, yf);

        if(dados->at(i).valor != menorY && dados->at(i).valor != maiorY)
        {
            painter.drawText(xf+5, yf, QString::number(dados->at(i).valor));
        }
        else
        {
            if(dados->at(i).valor == menorY)
            {
                painter.drawText(xf+5, yf-5, QString::number(dados->at(i).valor));
            }
            else
            {
                painter.drawText(xf+5, yf+15, QString::number(dados->at(i).valor));
            }
        }

        painter.setPen(QPen(Qt::black, 1));
        painter.drawLine(xi, yi, xf, yf);
        xi = xf;
        yi = yf;
    }
}


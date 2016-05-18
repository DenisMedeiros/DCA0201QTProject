#include "grafico.h"
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QColor>

Grafico::Grafico(QWidget *parent) : QWidget(parent)
{

}

void Grafico::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    QBrush brush;
    QPen pen;

    painter.setRenderHint(QPainter::Antialiasing);

    pen.setColor(QColor(0, 0, 0));
    painter.setPen(pen);

    brush.setColor(QColor(255, 255, 255));
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);

    /* Desenha o fundo da tela. */
    painter.drawRect(0,0,width()-1, height()-1);

    /* Desenha a linha horizontal. */
    painter.drawLine(0, height()/2, width(), height()/2);

    /* Desenha a linha vertical. */
    painter.drawLine(width()/2, 0, width()/2, height());

}


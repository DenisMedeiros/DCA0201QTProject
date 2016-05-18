#ifndef GRAFICO_H
#define GRAFICO_H

#include <QWidget>

class Grafico : public QWidget
{
    Q_OBJECT
public:
    explicit Grafico(QWidget *parent = 0);

signals:

protected:
    void paintEvent(QPaintEvent *e);

public slots:

};

#endif // GRAFICO_H

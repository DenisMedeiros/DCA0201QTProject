/**
 * @file grafico.h
 * @author Denis Ricardo da Silva Medeiros
 * @date 18 May 2016
 * @brief Widget que fará o plot do gráfico com base nos dados informados.
 */

#ifndef GRAFICO_H
#define GRAFICO_H

#include <QWidget>
#include <QPaintEvent>
#include <QList>
#include <conexao.h>

/**
 * @brief Este widget é um gráfico de duas dimensões, que exibe sempre 20 pontos no plano.
 *
 * O desenho do gráfico é feito através do método paintEvent, que é chamado toda vez
 * que o gerenciador gráfico necessita redesenhá-lo na tela.
 *
 */
class Grafico : public QWidget
{
    Q_OBJECT

private:
    /** @brief Lista de dados (datetime e valor) que representam as coordenadas x e y no gráfico. */
    QList<Dado> *dados;

    /** @brief Menor valor (y) que o gráfico possui no eixo vertical. */
    int menorY;

    /** @brief Maior valor (y) que o gráfico possui no eixo vertical. */
    int maiorY;
public:

    /**
     * @brief Este é o construtor padrão desta classe.
     *
     * Ele apenas inicializa os valores de menorY e maiorY e instancia o objeto dados.
     */
    explicit Grafico(QWidget *parent = 0);

    /**
     * @brief Este é o destrutor padrão desta classe.
     *
     * Ele não realiza nenhuma operação relevante.
     */
    ~Grafico(void);

    /**
     * @brief Este método retorna o menor valor do eixo y.
     *
     * @return Menor valor do eixo y.
     */
    int getMenorY(void) const;

    /**
     * @brief Este método retorna o maior valor do eixo y.
     *
     * @return Maior valor do eixo y.
     */
    int getMaiorY(void) const;

    /**
     * @brief Este método altera os dados do gráfico.
     *
     * @param _dados Novos dados do gráfico.
     */
    void setDados(const QList<Dado> &_dados);

    /**
     * @brief Este método altera o menor valor do eixo y do gráfico.
     *
     * @param _menorY Menor valor do eixo y do gráfico.
     */
    void setMenorY(int _menorY);

    /**
     * @brief Este método altera o maior valor do eixo y do gráfico.
     *
     * @param _maiorY Maior valor do eixo y do gráfico.
     */
    void setMaiorY(int _maiorY);

protected:
    /**
     * @brief Este método redesenha o gráfico na tela.
     *
     * @param evento Objeto do evento ocorrido informado pelo sistema.
     */
    void paintEvent(QPaintEvent *evento);

signals:

public slots:



};

#endif // GRAFICO_H

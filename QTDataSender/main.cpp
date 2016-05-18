/**
 * @file main.cpp
 * @author Denis Ricardo da Silva Medeiros
 * @date 10 May 2016
 * @brief Arquivo que contém a função main.
 */

#include "mainwindow.h"
#include <QApplication>

/**
 * @brief Esta é a função main do programa.
 *
 * Ela cria uma MainWindow e a exibe para o usuário.
 *
 * @param argc Número de argumentos informados passados para o programa.
 * @param argv Vetor de strings com os parâmetros informados pelo usuário.
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

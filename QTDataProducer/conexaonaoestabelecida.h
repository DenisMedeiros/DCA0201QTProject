#ifndef CONEXAONAOESTABELECIDA_H
#define CONEXAONAOESTABELECIDA_H

#include <iostream>
#include <QString>

class ConexaoNaoEstabelecida
{

private:
  QString mensagem;

public:
  ConexaoNaoEstabelecida(QString _mensagem = "Erro na conexão.") {
      mensagem = _mensagem;
  }

  QString getMensagem() const {
      return mensagem;
  }

};

#endif // CONEXAONAOESTABELECIDA_H

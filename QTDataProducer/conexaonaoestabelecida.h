#ifndef CONEXAONAOESTABELECIDA_H
#define CONEXAONAOESTABELECIDA_H

#include <iostream>

class ConexaoNaoEstabelecida
{

private:
  const char* mensagem;

public:
  ConexaoNaoEstabelecida(const char* msg = "Não foi possível conectar ao servidor") : mensagem(msg) {
  }

  const char* getMensagem() const {
      return mensagem;
  }

};

#endif // CONEXAONAOESTABELECIDA_H

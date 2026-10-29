#include "despacho/despacho.hpp"

#include <iostream>

void executar(
    const comando& atual,
    lista_persistente& lista
) {
    switch (atual.tipo) {
        case tipo_comando::inclusao:
            lista.incluir(atual.primeiro_argumento);
            break;
        case tipo_comando::remocao:
            lista.remover(atual.primeiro_argumento);
            break;
        case tipo_comando::sucessor:
        case tipo_comando::impressao:
            std::cout << atual.linha_original << '\n';
            break;
    }
}

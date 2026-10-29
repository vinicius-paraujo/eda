#include "despacho/despacho.hpp"

#include <iostream>

void executar(const comando& atual) {
    switch (atual.tipo) {
        case tipo_comando::inclusao:
        case tipo_comando::remocao:
            break;
        case tipo_comando::sucessor:
        case tipo_comando::impressao:
            std::cout << atual.linha_original << '\n';
            break;
    }
}

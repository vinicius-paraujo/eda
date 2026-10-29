#include "despacho/despacho.hpp"

#include <iostream>
#include <optional>

namespace {

void executar_sucessor(
    const comando& atual,
    const lista_persistente& lista
) {
    const tipo_chave chave = atual.primeiro_argumento;
    const tipo_versao versao = atual.segundo_argumento;
    const tipo_versao mais_recente = lista.versao_atual();

    if (versao > mais_recente) {
        std::cout << "Versão " << versao << " não existe; a versão mais recente é " << mais_recente
                  << ".\n";
        return;
    }

    const std::optional<tipo_chave> resultado = lista.sucessor(chave, versao);
    if (resultado) {
        std::cout << *resultado << '\n';
    } else {
        std::cout << "inf\n";
    }
}

}

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
            std::cout << atual.linha_original << '\n';
            executar_sucessor(atual, lista);
            break;
        case tipo_comando::impressao:
            std::cout << atual.linha_original << '\n';
            break;
    }
}

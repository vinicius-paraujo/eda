#include "despacho/despacho.hpp"

#include <algorithm>
#include <iostream>
#include <optional>
#include <vector>

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

void executar_impressao(
    const comando& atual,
    const lista_persistente& lista
) {
    // O enunciado determina que uma versão inexistente seja impressa como a mais recente.
    const tipo_versao versao = std::min(atual.primeiro_argumento, lista.versao_atual());
    const std::vector<tipo_chave> chaves = lista.listar(versao);

    for (std::size_t i = 0; i < chaves.size(); ++i) {
        if (i > 0) std::cout << ", ";
        std::cout << chaves[i];
    }
    std::cout << '\n';
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
            executar_impressao(atual, lista);
            break;
    }
}

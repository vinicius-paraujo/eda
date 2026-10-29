#include "estrutura/lista_persistente.hpp"

lista_persistente::lista_persistente() : raizes{nullptr} {}

lista_persistente::no* lista_persistente::criar_no(tipo_chave chave, no* proximo) {
    nos.push_back(std::make_unique<no>(no{chave, proximo, nullptr, {}, 0}));
    no* novo = nos.back().get();
    if (proximo) proximo->ponteiro_retorno = novo;
    return novo;
}

lista_persistente::no* lista_persistente::ler_proximo(const no* atual, tipo_versao versao) {
    no* resultado = atual->proximo;
    for (std::size_t i = 0; i < atual->quantidade_de_mods; ++i) {
        const mod& modificacao = atual->mods[i];
        if (modificacao.versao > versao) break;
        if (modificacao.campo_modificado == campo::proximo) resultado = modificacao.valor;
    }
    return resultado;
}

void lista_persistente::atribuir_proximo(no* atual, no* destino, tipo_versao versao) {
    if (atual->quantidade_de_mods < MAX_MODS) {
        atual->mods[atual->quantidade_de_mods++] = mod{versao, campo::proximo, destino};
        if (destino) destino->ponteiro_retorno = atual;
        return;
    }

    // O nó cheio permanece intacto para as versões antigas; a última versão passa a usar a cópia.
    no* copia = criar_no(atual->chave, destino);
    no* predecessor = atual->ponteiro_retorno;
    if (predecessor) {
        atribuir_proximo(predecessor, copia, versao);
    } else {
        definir_raiz(copia, versao);
    }
}

void lista_persistente::definir_raiz(no* nova_raiz, tipo_versao versao) {
    raizes[versao] = nova_raiz;
    if (nova_raiz) nova_raiz->ponteiro_retorno = nullptr;
}

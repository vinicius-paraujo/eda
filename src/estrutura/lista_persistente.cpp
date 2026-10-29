#include "estrutura/lista_persistente.hpp"

lista_persistente::lista_persistente() {
    raizes.reserve(MAXIMO_DE_VERSOES);
    raizes.push_back(nullptr);
}

tipo_versao lista_persistente::versao_atual() const {
    return static_cast<tipo_versao>(raizes.size() - 1);
}

// A nova versão começa igual à anterior; a operação que a criou altera apenas o necessário.
tipo_versao lista_persistente::criar_versao() {
    raizes.push_back(raizes.back());
    return versao_atual();
}

lista_persistente::no* lista_persistente::raiz_da_versao(tipo_versao versao) const {
    return raizes.at(versao);
}

void lista_persistente::incluir(tipo_chave chave) {
    const tipo_versao versao = criar_versao();
    no* predecessor = ultimo_menor_que(chave, versao);

    if (predecessor) {
        no* novo = criar_no(chave, ler_proximo(predecessor, versao));
        atribuir_proximo(predecessor, novo, versao);
    } else {
        no* novo = criar_no(chave, raiz_da_versao(versao));
        definir_raiz(novo, versao);
    }
}

// O enunciado exige uma nova versão mesmo quando a chave não está presente.
void lista_persistente::remover(tipo_chave chave) {
    const tipo_versao versao = criar_versao();
    no* predecessor = ultimo_menor_que(chave, versao);
    no* alvo = predecessor ? ler_proximo(predecessor, versao) : raiz_da_versao(versao);
    if (!alvo || alvo->chave != chave) return;

    no* sucessor = ler_proximo(alvo, versao);
    if (predecessor) {
        atribuir_proximo(predecessor, sucessor, versao);
    } else {
        definir_raiz(sucessor, versao);
    }
}

std::optional<tipo_chave> lista_persistente::sucessor(
    tipo_chave chave,
    tipo_versao versao
) const {
    for (const no* atual = raiz_da_versao(versao); atual; atual = ler_proximo(atual, versao)) {
        if (atual->chave > chave) return atual->chave;
    }
    return std::nullopt;
}

std::vector<tipo_chave> lista_persistente::listar(tipo_versao versao) const {
    std::vector<tipo_chave> chaves;
    for (const no* atual = raiz_da_versao(versao); atual; atual = ler_proximo(atual, versao)) {
        chaves.push_back(atual->chave);
    }
    return chaves;
}

// Devolve nulo quando a posição procurada é o início da lista.
lista_persistente::no* lista_persistente::ultimo_menor_que(
    tipo_chave chave,
    tipo_versao versao
) const {
    no* anterior = nullptr;
    no* atual = raiz_da_versao(versao);
    while (atual && atual->chave < chave) {
        anterior = atual;
        atual = ler_proximo(atual, versao);
    }
    return anterior;
}

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

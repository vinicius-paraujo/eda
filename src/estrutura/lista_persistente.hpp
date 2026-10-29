#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

using tipo_chave = std::uint32_t;
using tipo_versao = std::uint32_t;

class lista_persistente {
public:
    lista_persistente();

private:
    // Numa lista simplesmente encadeada cada nó é apontado apenas pelo predecessor ou pela raiz.
    static constexpr std::size_t P = 1;
    static constexpr std::size_t MAX_MODS = 2 * P;

    enum class campo { proximo };

    struct no;

    struct mod {
        tipo_versao versao;
        campo campo_modificado;
        no* valor;
    };

    struct no {
        tipo_chave chave;
        no* proximo;
        no* ponteiro_retorno;
        std::array<mod, MAX_MODS> mods;
        std::size_t quantidade_de_mods;
    };

    std::vector<std::unique_ptr<no>> nos;
    std::vector<no*> raizes;

    no* criar_no(tipo_chave chave, no* proximo);
    static no* ler_proximo(const no* atual, tipo_versao versao);
    void atribuir_proximo(no* atual, no* destino, tipo_versao versao);
    void definir_raiz(no* nova_raiz, tipo_versao versao);
};

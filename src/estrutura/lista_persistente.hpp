#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

using tipo_chave = std::uint32_t;
using tipo_versao = std::uint32_t;

class lista_persistente {
    public:
        lista_persistente();

        tipo_versao versao_atual() const;

        void incluir(tipo_chave chave);
        void remover(tipo_chave chave);

        std::optional<tipo_chave> sucessor(
            tipo_chave chave,
            tipo_versao versao
        ) const;

        std::vector<tipo_chave> listar(tipo_versao versao) const;

    private:
        // Cada inclusão ou remoção gera exatamente uma versão; com no máximo 1000 delas,
        // somadas à versão 0 vazia, há no máximo 1001 versões.
        static constexpr std::size_t MAXIMO_DE_VERSOES = 1001;

        // Numa lista simplesmente encadeada cada nó é apontado apenas pelo predecessor ou pela raiz.
        static constexpr std::size_t P = 1;
        static constexpr std::size_t MAX_MODS = 2 * P;

        enum class campo {
            proximo 
        };

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

        no* criar_no(
            tipo_chave chave, 
            no* proximo
        );

        static no* ler_proximo(
            const no* atual, 
            tipo_versao versao
        );

        void atribuir_proximo(
            no* atual, 
            no* destino, 
            tipo_versao versao
        );

        void definir_raiz(
            no* nova_raiz,
            tipo_versao versao
        );

        tipo_versao criar_versao();
        no* raiz_da_versao(tipo_versao versao) const;

        no* ultimo_menor_que(
            tipo_chave chave,
            tipo_versao versao
        ) const;
};

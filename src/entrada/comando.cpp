#include "entrada/comando.hpp"

#include <limits>
#include <sstream>

namespace {
    std::optional<tipo_comando> identificar_tipo(const std::string& nome) {
        if (nome == "INC") return tipo_comando::inclusao;
        if (nome == "REM") return tipo_comando::remocao;
        if (nome == "SUC") return tipo_comando::sucessor;
        if (nome == "IMP") return tipo_comando::impressao;
        return std::nullopt;
    }

    int quantidade_de_argumentos(tipo_comando tipo) {
        return tipo == tipo_comando::sucessor ? 2 : 1;
    }

    // A leitura direta em std::uint32_t aceitaria "-1" convertendo para um valor enorme.
    std::optional<std::uint32_t> ler_inteiro_nao_negativo(std::istringstream& fluxo) {
        long long valor;
        if (!(fluxo >> valor)) return std::nullopt;
        if (valor < 0 || valor > std::numeric_limits<std::uint32_t>::max()) return std::nullopt;
        return static_cast<std::uint32_t>(valor);
    }
}

std::optional<comando> interpretar_linha(const std::string& linha) {
    std::istringstream fluxo(linha);
    std::string nome;
    fluxo >> nome;

    const std::optional<tipo_comando> tipo = identificar_tipo(nome);
    if (!tipo) return std::nullopt;

    comando resultado{*tipo, 0, 0, linha};

    const std::optional<std::uint32_t> primeiro = ler_inteiro_nao_negativo(fluxo);
    if (!primeiro) return std::nullopt;
    resultado.primeiro_argumento = *primeiro;

    if (quantidade_de_argumentos(*tipo) == 2) {
        const std::optional<std::uint32_t> segundo = ler_inteiro_nao_negativo(fluxo);
        if (!segundo) return std::nullopt;
        resultado.segundo_argumento = *segundo;
    }

    std::string excedente;
    if (fluxo >> excedente) return std::nullopt;

    return resultado;
}

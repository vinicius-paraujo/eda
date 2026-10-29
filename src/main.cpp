#include "entrada/comando.hpp"
#include "despacho/despacho.hpp"

#include <fstream>
#include <iostream>
#include <optional>
#include <string>

namespace {

// Arquivos gerados no Windows terminam linhas com "\r\n", e o "\r" seria ecoado na saída.
void remover_retorno_de_carro(std::string& linha) {
    if (!linha.empty() && linha.back() == '\r') linha.pop_back();
}

bool esta_em_branco(const std::string& linha) {
    return linha.find_first_not_of(" \t") == std::string::npos;
}

}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <arquivo de entrada>\n";
        return 1;
    }

    std::ifstream entrada(argv[1]);
    if (!entrada) {
        std::cerr << "Não foi possível abrir o arquivo " << argv[1] << ".\n";
        return 1;
    }

    lista_persistente lista;
    std::string linha;
    int numero_da_linha = 0;
    while (std::getline(entrada, linha)) {
        ++numero_da_linha;
        remover_retorno_de_carro(linha);
        if (esta_em_branco(linha)) continue;

        const std::optional<comando> atual = interpretar_linha(linha);
        if (!atual) {
            std::cerr << "Linha " << numero_da_linha << " ignorada por não ser um comando válido: "
                      << linha << '\n';
            continue;
        }
        executar(*atual, lista);
    }

    return 0;
}

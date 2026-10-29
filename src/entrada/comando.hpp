#pragma once

#include <cstdint>
#include <optional>
#include <string>

enum class tipo_comando { inclusao, remocao, sucessor, impressao };

struct comando {
    tipo_comando tipo;
    std::uint32_t primeiro_argumento;
    std::uint32_t segundo_argumento;
    std::string linha_original;
};

std::optional<comando> interpretar_linha(const std::string& linha);

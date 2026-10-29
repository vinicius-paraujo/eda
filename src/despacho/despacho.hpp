#pragma once

#include "entrada/comando.hpp"
#include "estrutura/lista_persistente.hpp"

void executar(
    const comando& atual,
    lista_persistente& lista
);

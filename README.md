# Estruturas de Dados Avançadas - Trabalho 01

## Equipe

Marcos Vinicius Pereira Araujo (matrícula 571042)

## Descrição

- **Linguagem de programação utilizada:** C++17, compilado com g++ 13 ou superior (`-std=c++17`)
- **Estrutura implementada:** lista encadeada ordenada com persistência parcial pelo método de node copying, com custo adicional O(1) amortizado por operação e espaço O(1) por modificação
- **Estruturas:**
  - **Lista persistente:**
    - Definida em `src/estrutura/lista_persistente.hpp` e implementada em `src/estrutura/lista_persistente.cpp`
    - Estrutura da lista é `class lista_persistente`
    - Guarda todas as versões da lista ordenada, os nós e a raiz de cada versão
    - O tipo `tipo_chave` (`std::uint32_t`) representa as chaves, inteiros não negativos de 32 bits
    - O tipo `tipo_versao` (`std::uint32_t`) representa os identificadores de versão
    - A constante `MAXIMO_DE_VERSOES` (1001) é o número máximo de versões: a versão 0 vazia e uma por inclusão ou remoção
    - A constante `P` (1) é o número máximo de ponteiros que apontam para um nó; numa lista simplesmente encadeada, apenas o predecessor ou a raiz
    - A constante `MAX_MODS` (`2 * P`) é a capacidade do campo de modificações de cada nó
    - O campo `nos` guarda todos os nós criados e é dono da memória deles
    - O campo `raizes` guarda o primeiro nó de cada versão; `raizes[v]` é a raiz da versão `v`
    - A função `versao_atual()` retorna a versão mais recente
    - A função `incluir(chave)` cria uma nova versão com a chave inserida em ordem, aceitando repetições
    - A função `remover(chave)` cria uma nova versão sem uma ocorrência da chave; se a chave não existir, a nova versão é igual à anterior
    - A função `sucessor(chave, versao)` retorna a menor chave estritamente maior que `chave` na versão indicada, ou nada se não houver
    - A função `listar(versao)` retorna as chaves da versão indicada em ordem crescente
    - A função `criar_versao()` cria uma versão com a mesma raiz da anterior e retorna seu identificador
    - A função `raiz_da_versao(versao)` retorna a raiz da versão indicada
    - A função `ultimo_menor_que(chave, versao)` retorna o último nó com chave menor que `chave` na versão indicada, ou nulo se a posição for o início da lista; é usada pela inclusão e pela remoção para achar o predecessor
    - A função `criar_no(chave, proximo)` cria um nó com os campos originais e atualiza o ponteiro de retorno do nó seguinte
    - A função `ler_proximo(no, versao)` retorna o valor do campo `proximo` do nó na versão indicada, aplicando as modificações feitas até essa versão
    - A função `atribuir_proximo(no, destino, versao)` altera o campo `proximo` do nó na versão indicada: registra uma modificação se houver espaço; caso contrário, cria uma cópia do nó com o novo valor e redireciona o predecessor ou a raiz para a cópia, o que pode se propagar em cadeia
    - A função `definir_raiz(no, versao)` altera a raiz da versão indicada

  - **Nó persistente:**
    - Definida em `src/estrutura/lista_persistente.hpp`, como tipo interno de `lista_persistente`
    - Estrutura do nó é `struct no`
    - Guarda um elemento da lista e o histórico de alterações do seu campo `proximo`
    - O campo `chave` corresponde ao valor armazenado, que não muda depois da criação
    - O campo `proximo` corresponde ao valor original do ponteiro para o nó seguinte, definido na criação
    - O campo `ponteiro_retorno` corresponde ao nó que aponta para este na versão mais recente
    - O campo `mods[]` corresponde às modificações do campo `proximo`, em ordem de versão, com no máximo `MAX_MODS` entradas
    - O campo `quantidade_de_mods` corresponde ao número de posições ocupadas em `mods[]`

  - **Modificação:**
    - Definida em `src/estrutura/lista_persistente.hpp`, como tipo interno de `lista_persistente`
    - Estrutura da modificação é `struct mod`
    - Guarda uma alteração de campo no formato (versão, campo, valor)
    - O campo `versao` corresponde à versão em que a alteração passa a valer
    - O campo `campo_modificado` corresponde ao campo alterado, do tipo `enum class campo`, cujo único valor é `proximo`
    - O campo `valor` corresponde ao novo valor do ponteiro

  - **Comando:**
    - Definida em `src/entrada/comando.hpp` e implementada em `src/entrada/comando.cpp`
    - Estrutura do comando é `struct comando`, com o tipo `enum class tipo_comando` (`inclusao`, `remocao`, `sucessor`, `impressao`)
    - Guarda uma linha de entrada já interpretada
    - O campo `tipo` corresponde ao comando (`INC`, `REM`, `SUC` ou `IMP`)
    - O campo `primeiro_argumento` corresponde ao `N` de `INC N`, `REM N` e `SUC N M`, ou ao `M` de `IMP M`
    - O campo `segundo_argumento` corresponde ao `M` de `SUC N M`
    - O campo `linha_original` corresponde ao texto da linha, impresso antes do resultado de `SUC` e `IMP`
    - A função `interpretar_linha(linha)` converte uma linha em `comando`, ou indica que a linha é inválida (comando desconhecido, argumento ausente, negativo, maior que 32 bits ou excedente)
    - A função `identificar_tipo(nome)` converte o nome do comando em `tipo_comando`
    - A função `quantidade_de_argumentos(tipo)` retorna quantos argumentos o comando exige
    - A função `ler_inteiro_nao_negativo(fluxo)` lê um argumento e verifica se está entre 0 e 4294967295

  - **Despacho:**
    - Definido em `src/despacho/despacho.hpp` e implementado em `src/despacho/despacho.cpp`
    - Executa um `comando` sobre a lista persistente e escreve a saída
    - A função `executar(comando, lista)` aciona a inclusão ou a remoção, ou imprime a linha do comando e o resultado da consulta
    - A função `executar_sucessor(comando, lista)` imprime o sucessor, `inf` se não houver, ou a mensagem `Versão M não existe; a versão mais recente é V.` se a versão ainda não existir
    - A função `executar_impressao(comando, lista)` imprime as chaves da versão separadas por vírgula e espaço, usando a versão mais recente se a versão pedida não existir

  - **Programa principal:**
    - Implementado em `src/main.cpp`
    - A função `main` abre o arquivo recebido como argumento, lê cada linha, interpreta e executa o comando; linhas inválidas são informadas na saída de erro e ignoradas
    - A função `remover_retorno_de_carro(linha)` remove o `\r` final de arquivos com quebra de linha do Windows
    - A função `esta_em_branco(linha)` identifica linhas vazias, que são ignoradas

## Considerações
O trabalho foi construído a partir de diferentes fontes, mas, principalmente as notas de aula do professor e dos vídeos publicados no Youtube que falam sobre persistência (Inclusive, essa playlist salvou minha vida). Encontrei alguns materiais do MIT que também trazem algumas implementações, então foi relativamente tranquilo de implementar a partir desse material.

Sobre os módulos do projeto: `estrutura`, `despacho` e `entrada`. O que acontece respectivamente: estrutura contém a lista persistente. Despacho recebe um comando válido, aciona a lista e escreve a saída. O módulo de entrada recebe uma linha do arquivo, converte em um comando e rejeita linhas inválidas.

### Mudança em relação as saídas
O enunciado define que a saída de `IMP` separa os elementos por vírgula e espaço (`1, 5, 13`), mas as saídas esperadas dos testes disponibilizados separam apenas por espaço (`1 5 13`). 

Escolhi seguir o enunciado. Por isso, as cópias dos testes em `testes/teste01.out` a `testes/teste05.out` têm apenas essa alteração. Nas linhas de resultado de `IMP`, cada espaço entre elementos foi substituído por vírgula e espaço. As entradas e todas as demais linhas são idênticas às originais.

Comparando a saída do programa com os arquivos originais, as únicas diferenças são essas linhas.

### Quanto de IA tem nisso tudo?
Se eu fosse professor, nos tempos que estamos, provavelmente seria a primeira pergunta que eu faria. Sendo honesto, depois de estudar as notas de aula, materiais, e pensar bastante, ainda tive algumas dificuldades em traduzir algumas ideias da persistência parcial, bem como pseudocódigo encontrado, em implementações concretas em C++. Também essa parte mais chata do README de descrever o que as funções fazem. Nesse sentido, afirmo que houve cerca de 20% de uso de IA nesse trabalho.

Como não foram apresentadas restrições quanto ao uso, acho de bom tom afirmar.

## Testes
```
make build
make run INPUT=testes/teste01.in
```

Para executar todos os casos de `testes/` e comparar cada saída com o `.out` correspondente:

```
make test
```

O comando imprime `ok` ou `falhou` para cada entrada e termina com erro se algum caso falhar.

### Casos de teste

| Caso | Origem | O que verifica |
|---|---|---|
| `teste01` | Professor | Inclusões crescentes e impressão de cada versão |
| `teste02` | Professor | Inclusão no meio da lista e impressão de cada versão |
| `teste03` | Professor | Inclusões decrescentes e sucessor de chave ausente em versões antigas |
| `teste04` | Professor | Inclusões decrescentes seguidas de remoções e sucessor em versão antiga |
| `teste05` | Professor | Operações mistas, remoção de chave ausente, impressão de versão inexistente |
| `teste06` | Próprio | Chaves repetidas, remoção de uma única ocorrência, remoção em lista vazia, versão vazia |
| `teste07` | Próprio | Sucessor e impressão em versões inexistentes, inclusive com a estrutura vazia |
| `teste08` | Próprio | Modificações repetidas no mesmo nó, que forçam cópias de nós e troca de raiz |
| `teste09` | Próprio | Chaves nos limites: 0, 65535 (maior valor de 16 bits), 65536 e 4294967295 |
| `teste10` | Próprio | Limite de 1000 inclusões e remoções, com consultas espalhadas pelas 1001 versões |

### Casos próprios
Como houve liberdade para a criação de novos casos de testes, segue a descrição de cada um deles.

- (`teste06`) **Chaves repetidas**: o enunciado permite repetição e exige que a remoção retire apenas uma ocorrência, mas nenhum teste do professor inclui a mesma chave duas vezes.
- (`teste07`) **Versões inexistentes**: o enunciado define a impressão na versão mais recente, mas não define o comportamento do sucessor. Neste trabalho, `SUC` em versão inexistente imprime a linha do comando seguida da mensagem `Versão M não existe; a versão mais recente é V.`, e a execução continua.
- (`teste08`) **Cópia de nós**: verifica as versões após cópias sucessivas de nós e troca de raiz.
- (`teste09`) **Chaves de pelo menos 16 bits**: a maior chave dos testes do professor é 97.
- (`teste10`) **1001 versões**: o maior teste do professor tem 54 inclusões e remoções. A saída esperada deste caso foi gerada por um modelo de referência independente, que guarda uma cópia completa da lista em cada versão.

As saídas esperadas de `teste06` a `teste09` foram escritas à mão, sem usar o programa.

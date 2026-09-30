# Rastreabilidade de Requisitos

| ID | Requisito | Fonte | Como atenderemos | Status |
|---|---|---|---|---|
| REQ-01 | Implementação em C | Enunciado Projeto 1 | Todo o código escrito em ANSI C (`compilador.c`) | Fundação implementada |
| REQ-LEX-01 | Analisador léxico manual (sem Flex) | Decisão do grupo | Implementação de autômatos via código em `obterToken()` | Planejado |
| REQ-LEX-02 | Token deve armazenar tipo, linha, e atributo | Enunciado Projeto 1 | Estrutura `Token` no `compilador.c` com enum, linha, e union | Fundação implementada |
| REQ-LEX-03 | Saída de tokens no console e arquivo (`Linha# TOKEN \| ATRIB`) | Enunciado Projeto 1 | Função dedicada de impressão no fluxo do léxico | Planejado |
| REQ-LEX-04 | Parada e mensagem no erro léxico | Enunciado Projeto 1 | Verificação de estado não final/erro em `obterToken()` | Planejado |
| REQ-SIN-01 | Analisador sintático manual LL(1) (sem Bison) | Decisão do grupo | Funções mutuamente recursivas no `compilador.c` | Planejado |
| REQ-SIN-02 | Parada e mensagem no erro sintático | Enunciado Projeto 1 | Função de `match()` ou semelhante validando token | Planejado |
| REQ-SIN-03 | Parser consome através de `nextToken()` | Enunciado Projeto 1 | Função `nextToken()` que chama o léxico `obterToken()` | Fundação implementada |
| REQ-GER-01 | Nome dos 3 integrantes no código | Enunciado Projeto 1 | Comentário no topo do arquivo principal | Validado |
| REQ-GER-02 | Leitura de arquivo via CLI | Enunciado Projeto 1 | `main` trata `argv[1]` para abrir o arquivo | Validado |
| REQ-GER-03 | Compilar sem warnings com comando especificado | Enunciado Projeto 1 | Uso estrito de tipos, checagens e flags de GCC | Validado |

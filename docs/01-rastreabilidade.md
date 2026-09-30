# Rastreabilidade de Requisitos

| ID | Requisito | Fonte | Como atenderemos | Status |
|---|---|---|---|---|
| REQ-01 | Implementação em C | Enunciado | Todo o código escrito em ANSI C (`compilador.c`) | Planejado |
| REQ-LEX-01 | Analisador léxico manual (sem Flex) | Enunciado | Implementação de autômatos via código em `obterToken()` | Planejado |
| REQ-LEX-02 | Token deve armazenar tipo, linha, e atributo | Enunciado | Estrutura `Token` no `compilador.c` com enum, linha, e union | Implementado |
| REQ-LEX-03 | Saída de tokens no console e arquivo (`Linha# TOKEN \| ATRIB`) | Enunciado | Função dedicada de impressão no fluxo do léxico | Planejado |
| REQ-LEX-04 | Parada e mensagem no erro léxico | Enunciado | Verificação de estado não final/erro em `obterToken` | Planejado |
| REQ-SIN-01 | Analisador sintático manual LL(1) (sem Bison) | Enunciado | Funções mutuamente recursivas no `compilador.c` | Planejado |
| REQ-SIN-02 | Parada e mensagem no erro sintático | Enunciado | Função de `match()` ou semelhante validando token | Planejado |
| REQ-SIN-03 | Parser consome através de `nextToken()` | Enunciado | Função `nextToken()` que chama o léxico | Implementado |
| REQ-GER-01 | Nome dos 3 integrantes no código | Enunciado | Comentário no topo do arquivo principal | Implementado |
| REQ-GER-02 | Leitura de arquivo via CLI | Enunciado | `main` trata `argv[1]` para abrir o arquivo | Implementado |
| REQ-GER-03 | Compilar sem warnings com comando especificado | Enunciado | Uso estrito de tipos, checagens e flags de GCC | Implementado |

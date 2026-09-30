# Requisitos do Projeto 1 — Fase 1 (Análise Léxica e Sintática)

Disciplina: Compiladores — Profa. Daniela Cunha
Grupo: Gabriel Nottoli Buck, Julia Andrade, Joao vitor rocha miranda
Linguagem analisada: **MiniVisualg** (subconjunto simplificado do Visualg)
Linguagem de implementação: **C**

> **Fonte deste documento.** Tudo aqui foi reconstruído a partir do *Contexto Mestre*
> entregue ao início do projeto (resumo do enunciado, das aulas e do Anexo I).
> O PDF original do enunciado **não** foi consultado nesta sessão. Antes da entrega,
> cada linha marcada `ENUNCIADO` deve ser conferida contra o PDF (ver `plano.md`, Fase A,
> item de conferência).

## Legenda de origem

| Etiqueta | Significado |
|---|---|
| `ENUNCIADO` | Exigência do enunciado do projeto. |
| `AULA` | Conteúdo/orientação das aulas; esperado, mas não necessariamente formal no enunciado. |
| `GRUPO` | Decisão do grupo (registrada em `decisoes.md`). Nunca atribuída à professora. |
| `AMBIGUIDADE` | Material contraditório ou lacunar; registrada em `decisoes.md` (seção "Ambiguidades"). |

## Etapas e pesos

| Etapa | Peso | Conteúdo | Origem |
|---|---|---|---|
| 1 | 20% | Expressões regulares + Gramática Livre de Contexto | ENUNCIADO |
| 2 | 40% | Analisador léxico | ENUNCIADO |
| 3 | 40% | Analisador sintático | ENUNCIADO |

**Não entregar uma etapa zera o projeto** (ENUNCIADO). As três etapas são obrigatórias.

## Requisitos

### Geral / entrega

| ID | Requisito | Origem |
|---|---|---|
| REQ-01 | Implementar análise léxica e análise sintática do MiniVisualg. | ENUNCIADO |
| REQ-02 | Implementação em linguagem C. | ENUNCIADO |
| REQ-03 | O programa MiniVisualg analisado está em um único arquivo-fonte; sem pacotes; indentação sem significado. | ENUNCIADO |
| REQ-04 | As definições e estruturas dos exemplos do Anexo I são as **únicas** suportadas e devem ser seguidas rigorosamente. | ENUNCIADO |
| REQ-05 | Os nomes dos três integrantes aparecem em comentário no topo do arquivo-fonte. | ENUNCIADO |
| REQ-06 | Entrega = código + documentação, ou código muito bem comentado. | ENUNCIADO |
| REQ-07 | Existe um `readme.txt` com: até que parte o trabalho foi concluído; como executar; bugs/erros conhecidos; opcionalmente decisões de design. | ENUNCIADO |
| REQ-08 | Entrega conservadora do grupo: `compilador.c`, `documentacao.pdf`, `readme.txt`. | GRUPO |

### Compilação e execução

| ID | Requisito | Origem |
|---|---|---|
| REQ-10 | Comando de referência: `gcc -Wall -Wno-unused-result -g -Og compilador.c -o compilador` (MinGW + VS Code). | ENUNCIADO |
| REQ-11 | Não compila ou não executa → nota 0. Warnings geram penalidade. Retorno final adequado. | ENUNCIADO |
| REQ-12 | Meta operacional: 0 erros, 0 warnings. Nenhuma afirmação de "compila"/"passa" sem evidência de terminal. | GRUPO |
| REQ-13 | Nome do arquivo MiniVisualg vem da linha de comando (`argc/argv`), sem caminho fixo; falha de abertura é tratada. | ENUNCIADO (nome por linha de comando) + GRUPO (detalhes) |

### Análise léxica

| ID | Requisito | Origem |
|---|---|---|
| REQ-20 | Cada token é armazenado em um `struct` com campos de tipos diferentes (tipo, linha, atributo quando aplicável). | ENUNCIADO |
| REQ-21 | O `struct Token` do slide é referência conceitual (tipo, linha, `union` de atributo); não copiar cegamente. | AULA |
| REQ-22 | Ler o programa, reconhecer tokens, **mostrar na tela** e **gerar arquivo** com os mesmos tokens. | ENUNCIADO |
| REQ-23 | Formato de saída: `NúmeroDaLinha# NomeToken \| Atributo` (ex.: `11# ID \| 1`). Nem todo token precisa de atributo significativo. | ENUNCIADO (formato) + GRUPO (quando há atributo) |
| REQ-24 | Erro léxico: exibir `ERRO LÉXICO`, a linha e a sequência incorreta; **finalizar todo o processamento**. Sem recuperação sofisticada. | ENUNCIADO |
| REQ-25 | Ignorar espaço em branco e comentários; controlar número de linha. | AULA |
| REQ-26 | Tabela de símbolos desde a análise léxica. Escopo do grupo: só identificadores; 1ª ocorrência cria índice, demais reutilizam. | AULA (TS) + GRUPO (escopo) |
| REQ-27 | Não basta "comparar com uma lista" para tokens de classe (ID, inteiro, real, string). | AULA |

### Análise sintática

| ID | Requisito | Origem |
|---|---|---|
| REQ-30 | Análise sintática **descendente**. | ENUNCIADO |
| REQ-31 | Erro sintático: exibir `ERRO SINTÁTICO`, o token incorreto e a linha; pode informar esperado/encontrado; **finalizar o processo**. | ENUNCIADO |
| REQ-32 | Léxico e sintático interagem sob demanda: o sintático chama `nextToken()`, que chama `obterToken()` do léxico. | AULA |
| REQ-33 | Parser **descendente recursivo preditivo LL(1)**, uma função por não-terminal, `lookahead` + `consome()`. | GRUPO (baseado fortemente nas aulas; **não** é exigência literal) |
| REQ-34 | Registrar/gerar a derivação (árvore de derivação) sem montar uma AST complexa. | AULA (expectativa; ver AMB-07) |
| REQ-35 | Sem análise semântica e sem geração de código nesta fase. | GRUPO |

### Etapa 1 — documentos formais

| ID | Requisito | Origem |
|---|---|---|
| REQ-40 | Apresentar as expressões regulares da linguagem. | ENUNCIADO |
| REQ-41 | Apresentar a GLC correspondente. | ENUNCIADO |
| REQ-42 | Gramática sem recursão à esquerda e fatorada; FIRST/FOLLOW/nullable calculados; verificação LL(1). | AULA + GRUPO (verificação explícita) |

## Restrições de processo do grupo (todas as sessões)

Origem: `GRUPO`.

1. Nunca implementar requisito inexistente silenciosamente; lacuna → registrar ambiguidade.
2. Nunca usar Visualg externo como fonte de verdade.
3. Nunca afirmar que compila/passa sem executar.
4. Nunca alterar `main` diretamente; nunca fazer merge sem autorização; nunca `push --force`.
5. A branch `dev` não é fonte de verdade e não deve ser copiada.
6. Comentários explicam decisões, não narram linha a linha; código defensável oralmente em ~30 s por decisão.
7. Rastreabilidade: **material → regra → código → teste**.

## Prioridades (ordem)

1. Aderência ao enunciado → 2. aderência à abordagem da professora → 3. correção →
4. clareza → 5. capacidade de defender oralmente → 6. originalidade técnica onde houver liberdade.

Se uma decisão criativa prejudicar a aderência, é descartada.

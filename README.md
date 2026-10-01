# Compiladores — Projeto 1: MiniVisualg (Análise Léxica e Sintática)

Projeto da disciplina de **Compiladores** (Profa. Daniela Cunha).

**Grupo:** Gabriel Nottoli Buck · Julia Andrade · Joao vitor rocha miranda

Implementação, em **C**, das duas primeiras etapas do front-end de um compilador para o
**MiniVisualg**, subconjunto simplificado do Visualg definido exclusivamente pelos exemplos do Anexo I
do enunciado.

## Estado atual

**Fases A, A.1, B, C, D, E, F e G concluídas.** O `compilador.c` implementa o analisador léxico e o
analisador sintático descendente recursivo preditivo LL(1), integrados sob demanda. A próxima fase é a
**H (auditoria completa de integração)**.

| Etapa do enunciado | Peso | Situação |
|---|---|---|
| 1 — ERs + GLC | 20% | **concluída documentalmente e validada**: ERs (`docs/especificacao-lexica.md`), GLC (`docs/gramatica.md`), FIRST/FOLLOW e LL(1) (`docs/analise-ll1.md`) |
| 2 — Analisador léxico | 40% | **implementado e testado** (`compilador.c`): suíte léxica 9/9 + 9/9, e os 44 arquivos sintáticos tokenizam sem erro léxico. Ver `docs/arquitetura.md` e `docs/testes.md` §16 |
| 3 — Analisador sintático | 40% | **implementado e testado** (`compilador.c`): 19/19 válidos aceitos, 20/20 erros detectados no primeiro token/linha esperados e 5/5 limitações aceitas; árvore textual em `arvore.txt` |

## Abordagem (decisões do grupo)

- C puro, sem Flex/Bison/Yacc nem bibliotecas externas (`DEC-02`).
- Scanner caractere a caractere: lexemas fixos em tabela declarativa + reconhecedores próprios para
  tokens de classe (ID, inteiro, real, string) (`DEC-05`).
- Parser descendente recursivo preditivo LL(1), uma função por não-terminal (`DEC-03`).
- Léxico e sintático integrados sob demanda: `nextToken()` → `obterToken()`. Isto é **requisito do enunciado**
  (REQ-32, REQ-28); o restante do desenho é do grupo (`DEC-04`).
- Primeiro erro léxico ou sintático encerra o processamento (`DEC-10`). O código de retorno nesse caso
  **ainda não está decidido** (`AMB-13`); o código usa **0 provisoriamente** (`DEC-55`), numa única constante.
- Léxico: fonte lida em bytes (`DEC-48`), lexemas e tabela de símbolos dinâmicos (`DEC-49`, `DEC-50`), saída em
  `tokens.txt` (`DEC-52`), token emitido dentro de `obterToken()` (`DEC-56`).
- Léxico case-sensitive; 50 nomes de token; `<` e `>` isolados e `.` isolado são erro léxico
  (`DEC-13`, `DEC-16`, `DEC-24`).
- Gramática: sub-rotinas antes do principal (`DEC-28`); `OU` aceito no mesmo nível de `E`
  (`DEC-31`, `DEC-32`); sem subtração, `-` só em `passo -2` (`DEC-33`, `DEC-34`).

## Compilação e execução

```
gcc -Wall -Wno-unused-result -g -Og compilador.c -o compilador
./compilador programa.alg          # Linux/macOS/MinGW shell
.\compilador.exe programa.alg      # Windows
```

O nome do arquivo MiniVisualg é sempre recebido por linha de comando. O programa mostra os tokens na tela,
grava a mesma listagem lexical em `tokens.txt` e escreve a árvore de derivação textual em `arvore.txt`
(ambos no diretório atual e não versionados). Em erro sintático, `arvore.txt` fica parcial e contém
`<ERRO SINTATICO>`.
Compilado e testado com `gcc 15.2.0` (MSYS2 mingw64): 0 erros e 0 warnings. O comando oficial, sem
`-std`, funciona; o código usa recursos padronizados em C11 (`_Static_assert`). O GCC testado também o
aceitou em modos GNU anteriores compatíveis, como `-std=gnu99`, por extensão do compilador.

## Documentação (`docs/`)

| Arquivo | Conteúdo |
|---|---|
| [requisitos.md](docs/requisitos.md) | Requisitos rastreáveis, com origem (enunciado / aula / grupo). |
| [especificacao-minivisualg.md](docs/especificacao-minivisualg.md) | Inventário do que o Anexo I confirma, menciona ou não cobre. |
| [especificacao-lexica.md](docs/especificacao-lexica.md) | **Contrato léxico congelado (Fase B):** tokens, ERs, atributos, formato de saída, erros. |
| [gramatica.md](docs/gramatica.md) | **GLC congelada (Fase C):** 91 produções em BNF, sem recursão à esquerda, fatorada; cobertura do Anexo I; derivações manuais. |
| [analise-ll1.md](docs/analise-ll1.md) | **Validação LL(1) (Fase D):** nullable, FIRST, FOLLOW, SELECT, conflitos, tabela preditiva e mapa de decisão do parser. |
| [testes.md](docs/testes.md) | **Suíte de testes (Fase E):** matrizes, pontos de falha esperados, cobertura de P01–P91, terminais, `LEX-nn` e DEC. Arquivos em [`testes/`](testes/). |
| [arquitetura.md](docs/arquitetura.md) | **Arquitetura dos analisadores (Fases F/G):** lexer, parser LL(1), lookahead, ownership, SELECT, erros e árvore. |
| [decisoes.md](docs/decisoes.md) | Decisões do grupo e ambiguidades do material em aberto. |
| [plano.md](docs/plano.md) | Fases A–J e critérios de saída. |

## Política de branches

- `main`: versão consolidada e fonte de verdade do projeto.
- `dev`: tentativa anterior, mantida só como histórico. **Não é fonte de verdade** e não é usada.

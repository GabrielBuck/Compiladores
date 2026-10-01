# Plano de desenvolvimento

Cada fase só começa com autorização explícita do grupo. Não se pula fase.
Toda afirmação de validação (compilou, teste passou) exige evidência de terminal real.

| Fase | Tema | Entrega principal | Etapa do enunciado |
|---|---|---|---|
| A | Auditoria do repositório + requisitos — **CONCLUÍDA** | `docs/` iniciais, README, readme.txt | — |
| A.1 | Conferência contra o PDF oficial — **CONCLUÍDA** após o commit desta fase | correções factuais em `docs/`, README, readme.txt | — |
| B | Especificação lexical formal — **CONCLUÍDA** | `docs/especificacao-lexica.md` (ERs, tokens, atributos, formato de saída) | 1 (20%) |
| C | Gramática Livre de Contexto — **CONCLUÍDA** | `docs/gramatica.md` | 1 (20%) |
| D | FIRST / FOLLOW / nullable + verificação LL(1) — **CONCLUÍDA** | `docs/analise-ll1.md` (+ resumo em `gramatica.md` §19) | 1 → 3 |
| E | Casos de teste derivados da gramática — **CONCLUÍDA** | `testes/` + `docs/testes.md` | 2 e 3 |
| F | Analisador léxico | `obterToken()`, TS, saída tela/arquivo | 2 (40%) |
| G | Analisador sintático | funções por não-terminal, `nextToken()`, derivação | 3 (40%) |
| H | Integração | `compilador.c` único, `argc/argv`, entrega | 2 + 3 |
| I | Tratamento adversarial de erros | testes de erro léxico/sintático, linha correta | 2 + 3 |
| J | Documentação final e apresentação | `documentacao.pdf`, `readme.txt` final, roteiro oral | todas |

## Fase A — Auditoria + requisitos  — CONCLUÍDA

- Clonar do zero, criar `claude/projeto1` a partir de `main`, **sem** tocar na `dev`.
- Registrar requisitos, escopo, decisões e ambiguidades.
- **Saída:** `docs/requisitos.md`, `docs/especificacao-minivisualg.md`, `docs/decisoes.md`,
  `docs/plano.md`, `README.md`, `readme.txt`.
- **Fora do escopo:** lexer, parser, gramática definitiva, `compilador.c` funcional.

### Fase A.1 — conferência contra o PDF oficial  — CONCLUÍDA após este commit

A Fase A partiu do Contexto Mestre e registrou que o PDF ainda precisava ser conferido. A conferência foi
feita externamente pelo grupo e aplicada aqui. Principais correções:

- **REQ-32:** `nextToken()` → `obterToken()` é requisito do ENUNCIADO (Objetivo), não só das aulas.
- **REQ-20/21:** `struct` de token é ENUNCIADO (Etapa 2); a Figura 2 é ENUNCIADO (referência ilustrativa);
  não copiá-la cegamente é GRUPO (REQ-29).
- **REQ-28 (novo):** usar os nomes de módulos sugeridos (Etapas 2 e 3).
- **REQ-09 (novo):** nota de cada etapa depende de documentação e apresentação.
- **REQ-14 (novo):** lexemas separados por espaço é ENUNCIADO e contradiz o Anexo I (AMB-01).
- Declaração com vários IDs (`nome, sobrenome: caractere`) e atribuição a vetor (`nomes[1] <- "Ana"`):
  passaram de SEM EVIDÊNCIA / PARCIAL para **CONFIRMADO**.
- `-`: menos unário CONFIRMADO; subtração binária SEM EVIDÊNCIA. `OU`: mencionado/definido textualmente.
- AMB-10 decidida de forma conservadora; **AMB-11** (Figura 2 vs. operadores) e **AMB-12** (`ID` vs.
  `IDENTIFICADOR`) criadas; preocupações operacionais OP-01 (CRLF) e OP-02 (gcc) registradas.
- **Não resolvido de propósito:** AMB-05 (procedimentos), que continua visualmente ambígua no PDF.

Itens `ENUNCIADO` sem a marca ✔PDF em `requisitos.md` continuam vindo do Contexto Mestre.

## Fase B — Especificação lexical  — CONCLUÍDA

- **Saída:** `docs/especificacao-lexica.md` (contrato léxico congelado; regras `LEX-01`…`LEX-18`),
  `decisoes.md` com DEC-13…DEC-27, AMB-13 e AMB-14.
- **Decidido:** case-sensitive (AMB-02); `<`/`>` isolados = erro léxico (AMB-03); `OU` = token reservado
  próprio (parte lexical de AMB-04); `-` = `TOKEN_MENOS`, número sem sinal (parte lexical de AMB-06);
  nome impresso `ID` (AMB-12); AMB-11 resolvida (Figura 2 = formato, Anexo I = vocabulário).
- **Contrato:** 50 nomes de token (49 impressos); atributos por token; saída `linha# NOME [| atributo]`;
  `TOKEN_EOF` interno e não impresso.
- **Correção prévia:** DEC-10 não fixa mais o exit status (AMB-13 aberta).
- **Continuam abertas:** AMB-04 (gramática), AMB-05, AMB-06 (subtração binária), AMB-07, AMB-08 (Fase F),
  AMB-13, AMB-14.
- **Fora do escopo cumprido:** nenhum código C, nenhum scanner, nenhuma gramática.

## Fase C — GLC  — CONCLUÍDA

- **Saída:** `docs/gramatica.md` — 91 produções (P01–P91) em BNF, 50 não-terminais, os 49 terminais do
  contrato léxico; `decisoes.md` com DEC-28…DEC-46.
- **Critérios verificados:** GLC completa; sem recursão à esquerda (direta ou indireta); prefixos comuns
  fatorados (comando por `ID`, primário por `ID`, procedimento com/sem parâmetros, listas, programa
  com/sem rotinas); tabela produção → evidência; cobertura de todas as famílias do Anexo I (§13);
  10 derivações manuais (§14); formas rejeitadas documentadas (§15).
- **Resolvido:** AMB-04 (OU aceito, mesmo nível de E), AMB-05 (rotinas antes do principal; VAR
  conservadora), AMB-06 (sem subtração; `MENOS` só no passo), AMB-09 (fatoração).
- **Fora do escopo cumprido:** nenhum código C, nenhum parser/lexer, nenhuma tabela FIRST/FOLLOW.

## Fase D — FIRST/FOLLOW e LL(1)  — CONCLUÍDA

- **Saída:** `docs/analise-ll1.md` (IDs `LL1-01`…`LL1-19`); resumo em `gramatica.md` §19; DEC-47.
- **Resultado:** 17 anuláveis; FIRST/FOLLOW dos 50 não-terminais; SELECT das 91 produções; 24
  não-terminais com alternativas, 85 pares comparados, **0 conflitos**; tabela preditiva com 281 células,
  nenhuma duplicada. **A GLC é LL(1); nenhuma produção foi alterada.**
- Pontos quentes da Fase C verificados formalmente: `<cauda_comando_id>`, `<cauda_primario>`, caudas de
  expressão, fim das listas sem quebra de linha.
- **Fora do escopo cumprido:** nenhum código, nenhum parser/lexer, nenhum teste executável.
- **Critério de saída:** gramática LL(1) sem conflitos — **atendido** (sem emenda).

## Fase E — Testes derivados  — CONCLUÍDA

- **Saída:** `testes/` (62 testes: 9 LX-V com golden, 9 LX-E com `.erro.txt`, 19 SY-V, 20 SY-E, 5 SY-L;
  `manifest.tsv`; `README.md`) e `docs/testes.md`.
- **Critérios atendidos:** matriz pronta; goldens prontos; cobertura de P01–P91 por SY-V; os 49 terminais em
  golden léxico e em SY-V; `LEX-01`…`LEX-18` cobertas; validação descartável (tokenizador de referência +
  LL(1) estrito) sem divergências. Sem código; P01–P91 e o contrato léxico inalterados.
- Lacuna encontrada e registrada: linha do `TOKEN_EOF` (AMB-14).
- Plano original da fase (registro):
- Base: `especificacao-lexica.md` (regras `LEX-nn`, §18 casos-limite), `gramatica.md` (§13 cobertura,
  §15 rejeitadas) e `analise-ll1.md` (§13 mapa de decisão: cada entrada da tabela é um caso positivo; cada
  célula vazia relevante, um caso de erro sintático).
- Programas válidos (um por construção confirmada) e inválidos (um por tipo de erro esperado).
- Casos léxicos: lexemas com/sem espaço, `..` entre inteiros, string não fechada, caractere inválido,
  comentário no EOF sem `\n`, acentos.
- Casos sintáticos: falta de `fimse`, `entao` ausente, parêntese não fechado etc.
- Cada teste aponta para o `REQ-`/`DEC-`/`AMB-` que o motiva.

## Fase F — Léxico  — PRÓXIMA

- `obterToken()`, scanner por caractere, tabela de fixos, reconhecedores de classe, TS, contador de linhas.
- Saída em tela e em arquivo, mesmo conteúdo.
- **Antes da primeira compilação:** ver `decisoes.md`, OP-02 (testar `C:/msys64/mingw64/bin/gcc.exe`, ajustar
  o PATH só na sessão, `gcc --version`). Nada é instalado sem autorização.
- **Critério de saída:** compila com o comando de referência, 0 warnings; os 9 goldens `.tokens.txt` e os 9
  `.erro.txt` de `testes/lexico/` conferidos (comparação literal, tolerando só CRLF/LF).

## Fase G — Sintático

- Uma função por não-terminal, `lookahead`, `consome()`, `nextToken()`.
- Erro sintático com token e linha; decisão final sobre derivação (DEC-11 / AMB-07).
- **Critério de saída:** compila sem warnings; testes sintáticos executados.

## Fase H — Integração

- Junção em um único `compilador.c` (nomes dos integrantes no topo), `argc/argv`, tratamento de falha
  de abertura, códigos de retorno.
- Verificar o comando exato: `gcc -Wall -Wno-unused-result -g -Og compilador.c -o compilador`.

## Fase I — Erros adversariais

- Entradas malformadas, arquivo vazio, EOF no meio de construção, caracteres estranhos, linhas longas,
  CRLF vs LF, acentos em ANSI e UTF-8.
- Verificar que todo erro termina o processo com mensagem + linha corretas.

## Fase J — Documentação final

- `documentacao.pdf` (ERs, GLC, FIRST/FOLLOW, decisões, testes), `readme.txt` final
  (até onde foi concluído, como executar, bugs conhecidos), roteiro de apresentação.
- Conferência: cada integrante consegue explicar qualquer decisão em ~30 s.

## Regras de git (todas as fases)

- Commits pequenos e descritivos; sem `push --force`; sem tocar `main` ou `dev`; sem merge sem autorização.
- Antes de editar código: `git status` + `git pull`. Depois: compilar, testar, `git diff`, revisar, commit, push.

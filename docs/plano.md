# Plano de desenvolvimento

Cada fase só começa com autorização explícita do grupo. Não se pula fase.
Toda afirmação de validação (compilou, teste passou) exige evidência de terminal real.

| Fase | Tema | Entrega principal | Etapa do enunciado |
|---|---|---|---|
| A | Auditoria do repositório + requisitos — **CONCLUÍDA** | `docs/` iniciais, README, readme.txt | — |
| A.1 | Conferência contra o PDF oficial — **CONCLUÍDA** após o commit desta fase | correções factuais em `docs/`, README, readme.txt | — |
| B | Especificação lexical formal | `docs/especificacao-lexica.md` (ERs, tokens, atributos, formato de saída) | 1 (20%) |
| C | Gramática Livre de Contexto | `docs/gramatica.md` | 1 (20%) |
| D | FIRST / FOLLOW / nullable + verificação LL(1) | seção em `gramatica.md` | 1 → 3 |
| E | Casos de teste derivados da gramática | `testes/` + `docs/testes.md` | 2 e 3 |
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

## Fase B — Especificação lexical

- **Entrada:** AMB-01, 02, 03, 04, 06, 11 e 12 resolvidas (ou explicitamente adiadas com justificativa);
  AMB-08 só até o nível "STRING é conteúdo textual opaco" (política operacional fica para a Fase F).
- **Conteúdo:** ER de cada token de classe; tabela de lexemas fixos; convenção de atributo por token;
  formato exato de `NúmeroDaLinha# NomeToken | Atributo`; definição dos membros da `union` do `Token`;
  política de erro léxico (qual sequência é exibida).
- **Critério de saída:** cada lexema dos exemplos do Anexo I é classificado sem ambiguidade;
  cada regra tem ID rastreável para teste.

## Fase C — GLC

- **Entrada:** AMB-04, 05 e 06 (subtração binária) decididas. AMB-10 já está decidida (parênteses
  obrigatórios em `se`/`enquanto`, conservador). Declaração com lista de IDs e atribuição/`leia` com
  elemento de vetor são requisitos confirmados da gramática.
- **Conteúdo:** gramática derivando todas as expressões-teste de `especificacao-minivisualg.md` §5 e
  todos os exemplos do Anexo I; sem produção para nada sem evidência.
- **Critério de saída:** sem recursão à esquerda, fatorada, cada produção com comentário de origem.

## Fase D — FIRST/FOLLOW e LL(1)

- Calcular nullable, FIRST, FOLLOW; montar a tabela preditiva; procurar conflitos.
- **Ponto de atenção conhecido:** AMB-09 (comandos iniciados por ID).
- **Critério de saída:** gramática LL(1) sem conflitos, ou conflitos resolvidos na gramática.

## Fase E — Testes derivados

- Programas válidos (um por construção confirmada) e inválidos (um por tipo de erro esperado).
- Casos léxicos: lexemas com/sem espaço, `..` entre inteiros, string não fechada, caractere inválido,
  comentário no EOF sem `\n`, acentos.
- Casos sintáticos: falta de `fimse`, `entao` ausente, parêntese não fechado etc.
- Cada teste aponta para o `REQ-`/`DEC-`/`AMB-` que o motiva.

## Fase F — Léxico

- `obterToken()`, scanner por caractere, tabela de fixos, reconhecedores de classe, TS, contador de linhas.
- Saída em tela e em arquivo, mesmo conteúdo.
- **Antes da primeira compilação:** ver `decisoes.md`, OP-02 (testar `C:/msys64/mingw64/bin/gcc.exe`, ajustar
  o PATH só na sessão, `gcc --version`). Nada é instalado sem autorização.
- **Critério de saída:** compila com o comando de referência, 0 warnings; testes da Fase E (léxicos) executados.

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

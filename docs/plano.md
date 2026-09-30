# Plano de desenvolvimento

Cada fase só começa com autorização explícita do grupo. Não se pula fase.
Toda afirmação de validação (compilou, teste passou) exige evidência de terminal real.

| Fase | Tema | Entrega principal | Etapa do enunciado |
|---|---|---|---|
| A | Auditoria do repositório + requisitos | `docs/` iniciais, README, readme.txt | — |
| B | Especificação lexical formal | `docs/especificacao-lexica.md` (ERs, tokens, atributos, formato de saída) | 1 (20%) |
| C | Gramática Livre de Contexto | `docs/gramatica.md` | 1 (20%) |
| D | FIRST / FOLLOW / nullable + verificação LL(1) | seção em `gramatica.md` | 1 → 3 |
| E | Casos de teste derivados da gramática | `testes/` + `docs/testes.md` | 2 e 3 |
| F | Analisador léxico | `obterToken()`, TS, saída tela/arquivo | 2 (40%) |
| G | Analisador sintático | funções por não-terminal, `nextToken()`, derivação | 3 (40%) |
| H | Integração | `compilador.c` único, `argc/argv`, entrega | 2 + 3 |
| I | Tratamento adversarial de erros | testes de erro léxico/sintático, linha correta | 2 + 3 |
| J | Documentação final e apresentação | `documentacao.pdf`, `readme.txt` final, roteiro oral | todas |

## Fase A — Auditoria + requisitos  (esta sessão)

- Clonar do zero, criar `claude/projeto1` a partir de `main`, **sem** tocar na `dev`.
- Registrar requisitos, escopo, decisões e ambiguidades.
- **Saída:** `docs/requisitos.md`, `docs/especificacao-minivisualg.md`, `docs/decisoes.md`,
  `docs/plano.md`, `README.md`, `readme.txt`.
- **Pendência herdada:** conferir todas as linhas `ENUNCIADO` e o inventário do Anexo I contra o
  **PDF original**; este trabalho partiu do Contexto Mestre, não do PDF.
- **Fora do escopo:** lexer, parser, gramática definitiva, `compilador.c` funcional.

## Fase B — Especificação lexical

- **Entrada:** AMB-01, 02, 03, 04, 06, 08 resolvidas (ou explicitamente adiadas com justificativa).
- **Conteúdo:** ER de cada token de classe; tabela de lexemas fixos; convenção de atributo por token;
  formato exato de `NúmeroDaLinha# NomeToken | Atributo`; definição dos membros da `union` do `Token`;
  política de erro léxico (qual sequência é exibida).
- **Critério de saída:** cada lexema dos exemplos do Anexo I é classificado sem ambiguidade;
  cada regra tem ID rastreável para teste.

## Fase C — GLC

- **Entrada:** AMB-04, 05, 06, 10 decididas.
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

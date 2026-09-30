# Decisões e ambiguidades

Formato de cada item: **Problema · Alternativas · Decisão · Justificativa · Impacto**.
Origem das decisões: `GRUPO`, salvo onde indicado. Nenhuma decisão aqui é atribuída à professora.

Estados: **DECIDIDA** (vale desde já) · **PROVISÓRIA** (vale até a fase indicada) · **EM ABERTO**
(nada foi decidido; nenhum código deve assumir um lado).

---

# Parte 1 — Decisões do grupo

## DEC-01 · Recomeço a partir da `main`  — DECIDIDA

- **Problema:** uma tentativa anterior com outro agente deixou a branch `dev`.
- **Alternativas:** (a) continuar da `dev`; (b) cherry-pick seletivo; (c) recomeçar da `main`.
- **Decisão:** (c). Trabalho na branch `claude/projeto1`, criada a partir da `main`
  (`fce8754`). Sem merge, sem cópia de arquivos, sem reaproveitar arquitetura ou decisões da `dev`.
- **Justificativa:** a `dev` não é fonte de verdade; queremos decisões que o grupo consiga defender.
- **Impacto:** `dev` permanece no GitHub só como histórico. Se algo coincidir com ela, será por
  derivar das mesmas fontes (enunciado/aulas), não por cópia.

## DEC-02 · C puro, sem bibliotecas externas nem geradores  — DECIDIDA

- **Problema:** como implementar léxico e sintático.
- **Alternativas:** Flex/Bison/Yacc; bibliotecas de parsing; C puro manual.
- **Decisão:** C puro, biblioteca padrão apenas.
- **Justificativa:** o enunciado pede C; o objetivo é construir o que a disciplina ensinou
  (AFDs, análise descendente), o que geradores escondem.
- **Impacto:** scanner e parser escritos à mão; arquivo único `compilador.c` na entrega (REQ-08).

## DEC-03 · Parser descendente recursivo preditivo LL(1)  — DECIDIDA

- **Problema:** o enunciado exige análise *descendente*, sem fixar a técnica.
- **Alternativas:** descendente recursivo com backtracking; descendente preditivo LL(1) recursivo;
  LL(1) dirigido por tabela com pilha.
- **Decisão:** descendente recursivo **preditivo LL(1)**, uma função por não-terminal,
  `lookahead` de 1 token + `consome()`.
- **Justificativa:** é a abordagem apresentada na aula de implementação. **Isto é decisão do grupo,
  não exigência literal do enunciado.**
- **Impacto:** a gramática precisa passar por eliminação de recursão à esquerda e fatoração; conflitos
  LL(1) são resolvidos na gramática, não com backtracking (ver AMB-09).

## DEC-04 · Integração sob demanda `nextToken()` → `obterToken()`  — DECIDIDA

- **Problema:** como ligar léxico e sintático.
- **Alternativas:** léxico roda inteiro e grava lista de tokens; léxico sob demanda.
- **Decisão:** o sintático chama `nextToken()`, que chama `obterToken()` do léxico. O léxico continua
  mostrando e gravando cada token no momento em que é produzido.
- **Justificativa:** é o modelo conceitual das aulas; evita duas aplicações desconectadas.
- **Impacto:** o `lookahead` do parser é o único buffer de tokens; um erro léxico encerra o processo
  mesmo no meio do parsing. Ainda é preciso definir na Fase H como a saída do léxico e a do parser
  se intercalam (ou se a listagem de tokens é feita em modo separado).

## DEC-05 · Scanner caractere a caractere, híbrido  — DECIDIDA

- **Problema:** o enunciado diz que os lexemas estão separados por espaço, mas o Anexo I traz
  `leia(nome)`, `nomes[1]`, `somar(10, 5)` (ver AMB-01).
- **Alternativas:** `strtok(linha, " ")`; cadeia gigante de `if/else if`; scanner por caractere.
- **Decisão:** scanner caractere a caractere, com duas partes:
  1. **lexemas fixos** (palavras reservadas, operadores, pontuação) em **tabela declarativa**;
  2. **tokens de classe** (ID, inteiro, real, string) com reconhecedores próprios,
     conceitualmente pequenos AFDs.
- **Justificativa:** funciona com e sem espaços; responde à provocação da aula ("só comparar com
  uma lista basta?"); evita magic strings espalhadas.
- **Impacto:** a mesma rotina serve a entradas "com espaço" e "coladas". Precisa de 1–2 caracteres
  de lookahead de entrada (`<` vs `<-`/`<>`/`<=`; `.` vs `..`).

## DEC-06 · Struct `Token` com `union` de atributo  — DECIDIDA (forma); campos PROVISÓRIOS até a Fase B

- **Problema:** o enunciado exige `struct` com campos de tipos diferentes.
- **Decisão:** `Token { TokenName tipo; int linha; union { índice na TS; valor inteiro; valor real;
  código de operador } atributo; }`, com `enum` para tipos internos. A estrutura do slide é só referência.
- **Justificativa:** atende REQ-20/21; o conjunto exato de membros depende dos tokens reais.
- **Impacto:** os membros definitivos e a convenção "qual token tem atributo" saem na Fase B.

## DEC-07 · Tabela de símbolos mínima  — DECIDIDA

- **Problema:** o que guardar na TS.
- **Decisão:** só **identificadores**. 1ª ocorrência cria entrada e devolve índice; repetições reutilizam.
  Palavras reservadas ficam em catálogo estático separado, fora da TS. Sem informação semântica.
- **Justificativa:** TS é tema das aulas; análise semântica está fora desta fase.
- **Impacto:** `ID | índice`. Constantes (número/string) não entram na TS.

## DEC-08 · Comentário só `//`  — DECIDIDA

- **Decisão:** `//` vai até quebra de linha ou EOF e é descartado antes do parser.
  `/* ... */` **não** é implementado.
- **Justificativa:** o Anexo I só usa `//`; REQ-04 proíbe completar por analogia com C.

## DEC-09 · String fechada na mesma linha  — PROVISÓRIA (confirmar na Fase B)

- **Problema:** sem evidência de escapes ou strings multilinha.
- **Decisão:** STRING = `"` … `"` sem quebra de linha. Aberta e sem fechar até `\n`/EOF → `ERRO LÉXICO`.
  Sem escapes. As aspas não geram token próprio.
- **Impacto:** `"` dentro de string não é representável; aceitável porque não há exemplo que exija.

## DEC-10 · Erro encerra o processamento  — DECIDIDA

- **Decisão:** primeiro erro léxico ou sintático imprime a mensagem exigida e termina o processo
  (código de retorno diferente de zero). Sem recuperação.
- **Justificativa:** REQ-24 e REQ-31.
- **Impacto:** o parser não precisa de sincronização; o teste de erro verifica mensagem + linha.

## DEC-11 · Derivação registrada sem AST  — PROVISÓRIA (Fase G)

- **Problema:** árvore de derivação é expectativa das aulas, pouco explícita no enunciado (AMB-07).
- **Decisão provisória:** o parser é projetado para *poder* registrar a derivação (por ex. imprimindo
  a produção usada em cada função de não-terminal), sem construir AST. Se vai ser obrigatório na
  entrega, decide-se na Fase G.
- **Impacto:** cada função de não-terminal tem um único ponto onde a produção escolhida é conhecida.

## DEC-12 · Documentação com rastreabilidade  — DECIDIDA

- **Decisão:** manter `docs/` em Markdown no repositório e `documentacao.pdf` + `readme.txt` para a
  entrega. Cada regra do material recebe ID (`REQ-`, `AMB-`, `DEC-`) referenciado em código e testes.
- **Impacto:** cadeia material → regra → código → teste verificável.

---

# Parte 2 — Ambiguidades do material

Nenhuma delas pode ser resolvida consultando o Visualg externo. Todas dependem de decisão do grupo
(idealmente conferida com a professora) **antes** da fase indicada.

## AMB-01 · Espaço entre lexemas vs. exemplos adjacentes — EM ABERTO (tratamento técnico: DEC-05)

- **Problema:** o enunciado diz "todos os lexemas estão separados por um espaço"; o Anexo I tem
  `leia(nome)`, `escreval("Olá, mundo!")`, `nomes[1]`, `somar(10, 5)`.
- **Alternativas:** (a) exigir espaço e rejeitar os exemplos; (b) aceitar os dois estilos.
- **Direção já tomada:** (b), por DEC-05. Em aberto apenas: *programas de teste sem espaço são
  considerados válidos?* (assumimos sim).
- **Impacto:** fixa o desenho do scanner (Fase F).

## AMB-02 · Case sensitivity — EM ABERTO (uso provisório: lexemas exatamente como no Anexo I)

- **Problema:** o material não diz se `SE` = `se`, `Mod` = `MOD`.
- **Alternativas:** case-sensitive; case-insensitive; misto (reservadas insensíveis, IDs sensíveis).
- **Uso provisório:** casar apenas a grafia exata do Anexo I (inclusive `MOD` e `E` em maiúsculas).
- **Impacto:** afeta tabela de reservadas e testes. Decidir na Fase B.
- **Risco anotado:** `E` é identificador válido pela ER candidata de ID; só a regra de prioridade
  de reservada o impede. Em case-insensitive, `e` também viraria operador lógico.

## AMB-03 · `<` e `>` isolados — EM ABERTO (nada assumido)

- **Problema:** há `<-`, `<>`, `<=`, `>=`; não há exemplo de `<` ou `>` sozinhos.
- **Alternativas:** (a) **não** aceitar → `ERRO LÉXICO` se aparecerem; (b) aceitar como relacionais.
- **Posição atual:** não incluir. `<` só existe como prefixo de token composto.
- **Impacto:** tabela de operadores; casos de teste de erro léxico. Decidir na Fase B.

## AMB-04 · `OU` apenas mencionado — EM ABERTO

- **Problema:** há um comentário "O OU, basta um ser verdadeiro", mas nenhum uso executável.
- **Alternativas:** (a) entra como operador; (b) fica fora (vira ID, que é estranho); (c) reservado
  para não virar ID, mas sem produção na gramática.
- **Posição atual:** não transformar em ID silenciosamente. Decidir conscientemente na Fase B/C.
- **Impacto:** gramática de expressões (nível lógico) e tabela de reservadas.

## AMB-05 · Estrutura/posição dos procedimentos — EM ABERTO

- **Problema:** a formatação dos slides mistura três exemplos; em um deles os procedimentos aparecem
  sem `var`, em outro após `algoritmo` e antes de `inicio`, e funções aparecem *antes* de `var`.
  Não é possível congelar uma única estrutura sem escolher.
- **Sabemos:** existem `procedimento`, `fimprocedimento`; sem parâmetros não usa parênteses nem na
  declaração nem na chamada; com parâmetros, cada um é `nome: tipo`; funções têm `: tipo` de retorno.
- **Perguntas abertas:** (1) `var` é opcional? (2) sub-rotinas ficam entre `algoritmo` e `var`, ou
  também depois? (3) podem ser misturadas funções e procedimentos em qualquer ordem?
- **Impacto:** símbolo inicial da GLC (Fase C).

## AMB-06 · Sinal negativo vs. menos binário — EM ABERTO (proposta a validar)

- **Problema:** `passo -2` confirma o caractere `-`; não há `n1 - n2`.
- **Alternativas:** (a) léxico produz `NUM_INT` negativo; (b) léxico produz `MENOS` e `NUM_INT(2)`, e a
  gramática trata sinal unário; (c) (b) e ainda aceitar `-` binário.
- **Proposta:** (b) para o token; se o `-` binário será aceito na gramática é a parte aberta.
  (a) é desaconselhada: quebra `a -2` se um dia houver subtração.
- **Impacto:** ER de número, gramática de expressão. Decidir na Fase B/C.

## AMB-07 · Árvore de derivação — EM ABERTO (ver DEC-11)

- **Problema:** as aulas listam "gerar árvore de derivação" como tarefa do parser; a lista formal da
  Etapa 3 do enunciado não é tão clara.
- **Classificação:** expectativa das aulas, **não** requisito inequívoco do enunciado.
- **Impacto:** formato da saída do parser. Decidir na Fase G.

## AMB-08 · Codificação de caracteres (acentos) — EM ABERTO

- **Problema:** os exemplos têm acentos em strings e comentários (`"Olá, mundo!"`, `"João"`,
  `// A área de variáveis está vazia`). Arquivo pode estar em UTF-8 ou Windows-1252; o console do
  MinGW/Windows pode exibir diferente.
- **Alternativas:** tratar bytes ≥ 0x80 como caracteres comuns *dentro* de string e comentário,
  e como `ERRO LÉXICO` fora deles; ou aceitar também em identificadores.
- **Posição atual:** bytes ≥ 0x80 só dentro de string/comentário; ID segue ASCII.
- **Impacto:** reconhecedores de STRING/comentário; testes com arquivos UTF-8 e ANSI. Fase B/F.

## AMB-09 · Comando iniciado por identificador (risco LL(1) antecipado) — EM ABERTO

- **Problema:** atribuição (`x <- ...`), atribuição a vetor (`v[i] <- ...`) e chamada de procedimento
  (`linha_decorativa`, `mostrar_erro(...)`) **começam todos com ID**; chamada sem parênteses é
  indistinguível de um ID solto.
- **Alternativas:** fatorar à esquerda em `ID` + cauda (`<-` | `[` ... | `(` ... | vazio);
  ou lookahead de 2 tokens (sairia de LL(1)).
- **Posição atual:** resolver por fatoração (mantém LL(1)); confirmar com FIRST/FOLLOW na Fase D.
- **Impacto:** é o primeiro conflito que a verificação LL(1) deve examinar.

## AMB-10 · Parênteses obrigatórios em `se`/`enquanto`? — EM ABERTO

- **Problema:** todos os exemplos têm `se (cond) entao` e `enquanto (cond) faca`; `para` não tem.
- **Alternativas:** parênteses fazem parte da sintaxe do comando; ou são só parênteses de expressão
  (e `se x = 1 entao` também seria aceito).
- **Posição atual:** tratar como parte do comando (regra mais restritiva, consistente com REQ-04).
- **Impacto:** produções de `se`/`enquanto`; casos de erro sintático. Fase C.

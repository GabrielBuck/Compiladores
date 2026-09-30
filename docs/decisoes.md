# Decisões e ambiguidades

Formato de cada item: **Problema · Alternativas · Decisão · Justificativa · Impacto**.
Origem das decisões: `GRUPO`, salvo onde indicado. Nenhuma decisão aqui é atribuída à professora.

> **Conferência (Fase A.1).** Itens marcados **✔PDF** foram reconciliados com o PDF oficial do Projeto 1
> (conferido externamente pelo grupo). Os demais vêm do Contexto Mestre e seguem a conferir.

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
- **Justificativa:** a integração **e os nomes `nextToken()` / `obterToken()` são requisito do próprio
  enunciado** (REQ-32, ENUNCIADO — Objetivo ✔PDF; REQ-28, nomes de módulos sugeridos, Etapas 2 e 3 ✔PDF).
  As aulas apenas reforçam o modelo. O que é *decisão do grupo* é o restante do desenho: o léxico
  mostrar/gravar cada token na hora em que o produz, e o `lookahead` do parser ser o único buffer.
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

- **Problema:** o enunciado exige um registro (`struct`) com campos de tipos diferentes (REQ-20 ✔PDF) e
  oferece na Figura 2 uma estrutura ilustrativa com tipo, linha e `union` de atributos (REQ-21 ✔PDF).
- **Alternativas:** copiar a Figura 2 literalmente; usar `struct` sem `union`; adaptar a Figura 2 aos
  tokens reais.
- **Decisão (GRUPO, REQ-29):** adaptar. `Token { TokenName tipo; int linha; union { índice na TS;
  valor inteiro; valor real; código de operador } atributo; }`, com `enum` para tipos internos.
- **Separação de origens:** que exista `struct` de token é **ENUNCIADO — Etapa 2**; que a Figura 2 mostre
  esse formato é **ENUNCIADO — Figura 2** (referência ilustrativa); que *não* a copiemos cegamente é
  **GRUPO**.
- **Justificativa:** a Figura 2 orienta o *formato*; os membros reais dependem dos tokens do MiniVisualg
  (ver AMB-11).
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

- **Problema:** o enunciado afirma "Considerar que todos os lexemas no código fonte estão separados por
  um espaço" (REQ-14, ENUNCIADO ✔PDF), mas o Anexo I tem `leia(nome)`, `escreval("Olá, mundo!")`,
  `nomes[1]`, `somar(10, 5)` (contradição confirmada na A.1). **A contradição é do material e não é
  escondida aqui.**
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
- **Posição atual:** não incluir (Anexo I — Operadores confirma só `=`, `<>`, `>=`, `<=`). `<` e `>`
  só existem como prefixo de token composto; ambos ficam **em aberto / não suportados provisoriamente**.
- **Não usar a Figura 2 para decidir:** ela ilustra códigos de operador relacionais que não coincidem
  com os do Anexo I (ver AMB-11).
- **Impacto:** tabela de operadores; casos de teste de erro léxico. Decidir na Fase B.

## AMB-04 · `OU` apenas mencionado — EM ABERTO

- **Problema:** o Anexo I (Operadores lógicos) traz os comentários "O E exige que os DOIS lados sejam
  verdadeiros" e "O OU, basta um ser verdadeiro", e usa `E` em `podeBrincar <- (idade >= 12) E (altura
  >= 1.50)`. `OU` **não** aparece em nenhuma expressão executável.
- **Classificação:** `E` = confirmado por uso; `OU` = mencionado/definido textualmente. A distinção é
  mantida; `OU` não é esquecido.
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
- **Conferência A.1:** a disposição visual da página de PROCEDIMENTOS é de fato inconsistente; **não
  resolver ainda**. Fica para a Fase C.
- **Impacto:** símbolo inicial da GLC (Fase C).

## AMB-06 · Sinal negativo vs. menos binário — EM ABERTO (direção proposta a validar)

- **Problema:** `para i de 10 ate 0 passo -2 faca` (Anexo I — Repetição) confirma o caractere `-` e a
  forma negativa/unária. Não existe `n1 - n2` em nenhum exemplo.
- **Situação por forma:** sinal negativo / menos unário = **CONFIRMADO** (`-2`); subtração binária =
  **SEM EVIDÊNCIA**. Não adicionar subtração binária por simetria com `+`.
- **Alternativas:** (a) léxico produz `NUM_INT` negativo; (b) léxico produz `MENOS` e `NUM_INT(2)`, e a
  gramática trata a forma negativa; (c) (b) e ainda aceitar `-` binário.
- **Direção:** (b). (a) é desaconselhada: quebraria `a -2` caso um dia houvesse subtração. Tratar a
  forma negativa na gramática é sintaxe, não análise semântica.
- **Impacto:** ER de número, gramática de expressão e de `passo`. Decidir na Fase B/C.

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
- **Fatos (A.1):** strings e comentários oficiais têm acentos; os identificadores usados como evidência
  são ASCII; o scanner não pode quebrar os exemplos por causa de bytes de strings/comentários.
- **Escopo por fase:** na **Fase B** basta definir que STRING transporta **conteúdo textual opaco** entre
  aspas, dentro das limitações escolhidas. A política operacional exata (UTF-8 vs. Windows-1252, como
  tratar bytes ≥ 0x80) fica para a **Fase F**. Nenhuma política completa é inventada agora.
- **Posição atual (provisória):** bytes ≥ 0x80 só dentro de string/comentário; ID segue ASCII.
- **Impacto:** reconhecedores de STRING/comentário; testes com arquivos UTF-8 e ANSI.

## AMB-09 · Comando iniciado por identificador (risco LL(1) antecipado) — EM ABERTO

- **Problema:** atribuição (`x <- ...`), atribuição a vetor (`v[i] <- ...`) e chamada de procedimento
  (`linha_decorativa`, `mostrar_erro(...)`) **começam todos com ID**; chamada sem parênteses é
  indistinguível de um ID solto.
- **Alternativas:** fatorar à esquerda em `ID` + cauda (`<-` | `[` ... | `(` ... | vazio);
  ou lookahead de 2 tokens (sairia de LL(1)).
- **Conferência A.1:** o Anexo I contém `nomes[1] <- "Ana"`, `leia(notas[i])` e `nomes[2]`; logo atribuição
  indexada e referência indexada são CONFIRMADAS e entram no problema. O alvo de atribuição e o
  argumento de `leia` têm forma simples (`ID`) ou indexada (`ID [ expressão ]`).
- **Posição atual:** resolver por fatoração (mantém LL(1)); confirmar com FIRST/FOLLOW na Fase D.
- **Impacto:** é o primeiro conflito que a verificação LL(1) deve examinar.

## AMB-10 · Parênteses em `se` e `enquanto` — DECIDIDA (decisão conservadora, revisável)

- **Problema:** se a condição de `se`/`enquanto` poderia dispensar parênteses.
- **Fatos (Anexo I — Controle e Repetição):** **todos** os exemplos usam `se ( expressão ) entao` e
  `enquanto ( expressão ) faca`. Nenhum exemplo as escreve sem parênteses. `para` não tem parênteses.
- **Decisão (GRUPO):** estrutura **SUPORTADA** somente
  - `se ( expressão ) entao`
  - `enquanto ( expressão ) faca`

  Os parênteses fazem parte do comando no MiniVisualg deste projeto.
- **Justificativa:** o princípio do Projeto 1 é conservador: só as estruturas dos exemplos são suportadas
  (REQ-04). **Não** afirmamos nada sobre o que o Visualg verdadeiro aceita; isso é irrelevante aqui.
- **Natureza:** decisão conservadora baseada no Anexo I, **não** regra universal do Visualg.
  Pode ser revisada por instrução posterior da professora.
- **Impacto:** produções de `se`/`enquanto` (Fase C); `se x = 1 entao` passa a ser erro sintático.

## AMB-11 · Figura 2 de Token vs. operadores do MiniVisualg — EM ABERTO (direção decidida)

- **Problema:** a estrutura ilustrativa da Figura 2 (ENUNCIADO ✔PDF) usa um enum de operadores
  relacionais com códigos que parecem didáticos/genéricos e não coincidem perfeitamente com os operadores
  concretos do Anexo I: `=`, `<>`, `<=`, `>=`.
- **Alternativas:** (a) tomar o enum da Figura 2 como vocabulário da linguagem; (b) tomar a Figura 2 só
  como formato da struct/union e definir o vocabulário a partir do Anexo I.
- **Decisão atual (GRUPO):** (b). A Figura 2 orienta o **formato** da struct/union (REQ-21, DEC-06); **não**
  redefine nem amplia o vocabulário lexical do Anexo I. Em particular, não serve de argumento para aceitar
  `<` ou `>` isolados (AMB-03).
- **Impacto:** o `enum` de operadores é montado na Fase B a partir do Anexo I.
- **Pode depender da professora:** se a Figura 2 deve ser seguida ao pé da letra.

## AMB-12 · Nome do token de identificador: `ID` ou `IDENTIFICADOR` — EM ABERTO

- **Problema:** o próprio enunciado exemplifica o formato de saída com `11# IDENTIFICADOR | 1` e
  `11# ID | 1` (REQ-23 ✔PDF). Os dois nomes aparecem.
- **Alternativas:** (a) `ID`; (b) `IDENTIFICADOR`; (c) outra nomenclatura uniforme para todos os tokens.
- **Posição atual:** nada decidido. Ambos os exemplos são ilustrativos; o vocabulário de nomes de token é
  definido na **Fase B** e registrado lá.
- **Impacto:** `enum TokenName`, saída do léxico, testes.

---

# Parte 3 — Preocupações operacionais (não são decisões de linguagem)

## OP-01 · Fim de linha CRLF/LF

O aviso do Git (LF → CRLF) é do ambiente Git, não requisito da linguagem nem elemento da gramática.
Quando houver scanner, ele deve contar linhas corretamente em Windows: `\r\n` é tratado como **uma**
quebra de linha lógica. Registrado como preocupação de **teste** (Fases E e I), não como decisão
arquitetural.

## OP-02 · `gcc` neste computador

O `gcc` não está no PATH. Uma instalação MSYS2 pode existir em `C:\msys64\mingw64\bin\gcc.exe`.
**Nada foi instalado.** Antes da primeira compilação (a partir da Fase F), executar:

```powershell
Test-Path "C:\msys64\mingw64\bin\gcc.exe"
# se True:
$env:PATH += ";C:\msys64\mingw64\bin"
gcc --version
```

e só então compilar com o comando de referência. A alteração de PATH vale só para a sessão do terminal.

---

# Parte 4 — O que ainda pode depender da professora

Itens que o material não resolve e que o grupo só decide por conta própria se não houver resposta:

| Item | Por que depende |
|---|---|
| AMB-01 espaço entre lexemas | A contradição é do material; nosso tratamento técnico (DEC-05) aceita os dois estilos. |
| AMB-02 case sensitivity | O material não diz. |
| AMB-03 `<` e `>` isolados | Sem exemplo; a Figura 2 sugere outra coisa (AMB-11). |
| AMB-04 `OU` na gramática | Definido só em comentário. |
| AMB-05 estrutura de procedimentos | Página visualmente inconsistente. |
| AMB-06 subtração binária | Sem exemplo. |
| AMB-07 árvore de derivação obrigatória? | Aulas pedem; enunciado não é claro. |
| AMB-10 parênteses em `se`/`enquanto` | Decidido de forma conservadora; revisável. |
| AMB-11 seguir a Figura 2 literalmente? | Enunciado a apresenta como ilustrativa. |
| AMB-12 nome do token (`ID`/`IDENTIFICADOR`) | O enunciado usa ambos nos exemplos. |

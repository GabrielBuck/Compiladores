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
- **Decisão:** (c). Trabalho reiniciado a partir da `main` no commit `fce8754`.
  Sem cópia de arquivos e sem reaproveitar arquitetura ou decisões da `dev`.
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

## DEC-06 · Struct `Token` com `union` de atributo  — DECIDIDA (contrato conceitual fechado na Fase B; layout em memória na Fase F)

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
- **Impacto (Fase B):** o contrato conceitual — campos `type`, `line`, `lexeme`, atributo por tipo — e a
  convenção de quais tokens têm atributo estão fechados em `especificacao-lexica.md` §14–§15. O layout em
  memória (union, buffers) fica para a Fase F.

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

## DEC-09 · String fechada na mesma linha  — DECIDIDA (confirmada na Fase B)

- **Problema:** sem evidência de escapes ou strings multilinha.
- **Decisão:** STRING = `"` … `"` sem quebra de linha. Aberta e sem fechar até `\n`/EOF → `ERRO LÉXICO`.
  Sem escapes. As aspas não geram token próprio.
- **Impacto:** `"` dentro de string não é representável; aceitável porque não há exemplo que exija.
- **Fase B:** ER `"[^"\r\n]*"`; conteúdo opaco; sem escapes; ver `especificacao-lexica.md` §4.5 (LEX-07).

## DEC-10 · Erro encerra o processamento  — DECIDIDA (sem fixar código de retorno)

- **Problema:** o que fazer ao encontrar erro léxico ou sintático.
- **Decisão:** o **primeiro erro encerra imediatamente o processamento**, depois de apresentar a mensagem
  exigida. Sem recuperação.
- **O que NÃO está decidido:** o **código numérico de saída** do processo nesse caso. A versão anterior
  deste item dizia "diferente de zero"; foi **retirada** por ser prematura (ver AMB-13). A questão será
  resolvida nas Fases H/I.
- **Justificativa:** REQ-24 e REQ-31 (encerrar). REQ-11 (retorno final adequado; penalização) impede
  assumir por convenção `return 1`.
- **Impacto:** o parser não precisa de sincronização; o teste de erro verifica mensagem + linha. O
  critério de aprovação do teste de erro **não** inclui o exit status até a AMB-13 ser resolvida.

## DEC-11 · Derivação registrada sem AST  — DECIDIDA na Fase G (detalhada por DEC-61/DEC-62)

- **Problema:** árvore de derivação é expectativa das aulas, pouco explícita no enunciado (AMB-07).
- **Decisão:** o parser registra a derivação textual, sem construir AST.
- **Impacto:** cada função de não-terminal registra a produção escolhida; formato e destino estão em
  DEC-61/DEC-62.

## DEC-12 · Documentação com rastreabilidade  — DECIDIDA

- **Decisão:** manter `docs/` em Markdown no repositório e `documentacao.pdf` + `readme.txt` para a
  entrega. Cada regra do material recebe ID (`REQ-`, `AMB-`, `DEC-`) referenciado em código e testes.
- **Impacto:** cadeia material → regra → código → teste verificável.

## Decisões da Fase B — contrato léxico (DEC-13 a DEC-27)

Todas são `GRUPO`. Detalhe e ERs em `especificacao-lexica.md`. Nenhuma é atribuída à professora; as
marcadas "revisável" podem mudar se a professora esclarecer o contrário.

## DEC-13 · Léxico case-sensitive  — DECIDIDA (Fase B; revisável)

- **Problema:** o material não diz se `SE` = `se`, `Mod` = `MOD` (AMB-02).
- **Alternativas:** case-sensitive; case-insensitive; misto.
- **Decisão:** **case-sensitive em tudo.** Reservadas e operadores-palavra só na grafia do Anexo I
  (`algoritmo`, `MOD`, `E`, `OU` …). `ALGORITMO`, `mod`, `e` são **IDs**. IDs também são sensíveis:
  `nome` ≠ `Nome`.
- **Justificativa:** o material não define caixa; o projeto manda seguir rigorosamente as estruturas
  apresentadas; aceitar variações ampliaria silenciosamente o subconjunto (REQ-04).
- **Impacto:** catálogo alfabético com comparação exata; testes `ALGORITMO`/`mod` → `ID`. (LEX-01)

## DEC-14 · `OU` é token reservado próprio, distinto de `E`  — DECIDIDA (parte lexical de AMB-04)

- **Problema:** `OU` é definido só em comentário do Anexo I; `E` é usado em expressão.
- **Alternativas:** deixar `OU` virar ID; agrupar `E`/`OU` em uma classe; token próprio cada.
- **Decisão:** `OU` é lexema **reservado**, nunca ID, com `TOKEN_OU`; `E` tem `TOKEN_E`. **Sem** agrupamento
  em `TOKEN_OP_LOG`.
- **Justificativa:** preserva a informação do material sem fingir evidência de uso executável; não obriga
  o parser a aceitar ambos automaticamente.
- **Impacto:** se `OU` entra na gramática é decisão **sintática da Fase C** (AMB-04 segue parcialmente
  aberta). (LEX-14)

## DEC-15 · `-` é `TOKEN_MENOS`; número não tem sinal  — DECIDIDA (parte lexical de AMB-06)

- **Problema:** `passo -2` confirma `-` unário; não há `n1 - n2`.
- **Decisão:** `-` é `TOKEN_MENOS`, separado de `TOKEN_MAIS` e de qualquer classe. `NUM_INT = [0-9]+`;
  `-2` = `MENOS` `NUM_INT(2)`. A categoria lexical **não implica** subtração binária.
- **Impacto:** subtração binária permanece SEM EVIDÊNCIA; a gramática (Fase C) trata a forma negativa e
  decide sobre `-` binário. (AMB-06 segue parcialmente aberta.) (LEX-04)

## DEC-16 · `<` e `>` isolados não são tokens  — DECIDIDA (AMB-03 no léxico; revisável)

- **Problema:** só existem `<-`, `<>`, `<=`, `>=` no Anexo I.
- **Decisão:** `<` só vale como `<-`, `<>` ou `<=`; `>` só como `>=`. Caso contrário, **erro léxico** na
  sequência `<` ou `>`.
- **Justificativa:** escopo conservador baseado no Anexo I; **não** é afirmação sobre o Visualg completo.
- **Impacto:** `OP_REL` só tem `EQ NE LE GE`; sem `OP_LT`/`OP_GT`. (LEX-11)

## DEC-17 · Nome impresso do identificador: `ID`  — DECIDIDA (AMB-12)

- **Decisão:** nome textual `ID`; interno `TOKEN_ID`.
- **Justificativa:** o enunciado usa `ID` e `IDENTIFICADOR` nos exemplos (ambos aceitáveis); `ID` é
  conciso e coincide com a terminologia das aulas. **Não** se afirma que `IDENTIFICADOR` seria incorreto.

## DEC-18 · Relacionais agrupados em `TOKEN_OP_REL` + atributo  — DECIDIDA

- **Decisão:** `=` `<>` `<=` `>=` → `OP_REL` com atributo `EQ` `NE` `LE` `GE` (enum interno `OP_EQ`…).
- **Justificativa:** coerente com a ideia de classe + atributo da Figura 2 (formato, não vocabulário;
  AMB-11) e com os exemplos da disciplina; simplifica o parser.
- **Impacto:** sem `LT`/`GT` (DEC-16).

## DEC-19 · Multiplicativos agrupados em `TOKEN_OP_MULT` + atributo  — DECIDIDA

- **Decisão:** `*` `/` `\` `MOD` → `OP_MULT` com atributo `MUL` `DIV_REAL` `DIV_INT` `MOD`.
- **Justificativa:** na futura gramática os quatro ocupam o mesmo nível; a distinção fica no atributo.
- **Impacto:** `MOD` não tem token próprio; é `OP_MULT` reconhecido pelo catálogo alfabético.

## DEC-20 · Uma palavra reservada = um token; `leia/escreva/escreval` são reservadas  — DECIDIDA

- **Decisão:** as 31 palavras reservadas têm **tipo de token próprio** (`TOKEN_KW_…`), sem atributo; **não**
  existe `TOKEN_KEYWORD` único com atributo. `leia`, `escreva` e `escreval` **são reservadas** (não podem
  ser IDs).
- **Justificativa:** parser descendente legível (um `switch`/`if` por token); fecha a pendência de
  `especificacao-minivisualg.md`.
- **Impacto:** 31 + 19 demais = 50 nomes de token (§3 da especificação léxica).

## DEC-21 · STRING preserva o valor textual  — DECIDIDA

- **Decisão:** `TOKEN_STRING` carrega o texto literal; a listagem mostra `STRING | "Ana"`.
- **Impacto:** o `Token` precisa guardar o texto; o layout de memória fica para a Fase F.

## DEC-22 · Saída sem ` | atributo` para token sem atributo  — DECIDIDA (interpretação do grupo)

- **Decisão:** formato `linha# NOME` quando não há atributo; `linha# NOME | atributo` quando há. Nunca
  `| NULL`, `| NONE` ou `| 0`.
- **Justificativa:** o enunciado diz "valores correspondentes, se necessário" (REQ-23 ✔PDF); a omissão
  no caso sem atributo é **interpretação do grupo**, não texto do enunciado.

## DEC-23 · `TOKEN_EOF` é interno e não é impresso  — DECIDIDA

- **Decisão:** `TOKEN_EOF` participa da interface scanner/parser, mas não é átomo do arquivo-fonte e
  não aparece na listagem de saída.

## DEC-24 · Maximal munch e regra do ponto  — DECIDIDA

- **Decisão:** o scanner devolve o lexema válido mais longo sem consumir o que é do próximo token. Depois
  de dígitos: `.`+dígito → real; `..` → `INTERVALO`; outro `.` → inteiro termina e o `.` isolado é erro.
  `.` só é válido como `..`.
- **Impacto:** `vetor[1..4]` = `NUM_INT` `INTERVALO` `NUM_INT`. Exige olhar 2 caracteres adiante; mecanismo
  na Fase F. (LEX-06, LEX-10)

## DEC-25 · Whitespace e contagem de linhas  — DECIDIDA

- **Decisão:** whitespace = espaço, tab, LF, CR. A linha incrementa **só em `\n`**; CRLF = uma quebra
  lógica; CR isolado é whitespace e não conta. Linha do token = linha do primeiro caractere.
- **Impacto:** OP-01 (teste CRLF) passa a ter regra explícita. (LEX-09)

## DEC-26 · Formato e princípio da mensagem de erro léxico  — DECIDIDA (formato: proposta do grupo)

- **Decisão:** `ERRO LÉXICO - linha <n> - sequência: <sequência>`; a sequência é a **menor** que identifica
  o erro, sem lexemas válidos anteriores; em string não fechada, da aspa de abertura até o ponto de
  detecção.
- **Justificativa:** o enunciado exige os três dados; o formato numa linha é do grupo.
- **Impacto:** onde imprimir e o exit status seguem abertos (AMB-13, AMB-14). (LEX-18)

## DEC-27 · ER de ID: só ASCII, `_` não inicia  — DECIDIDA

- **Decisão:** `ID = [A-Za-z][A-Za-z0-9_]*`; sem letras acentuadas nem `_` inicial.
- **Justificativa:** sem exemplo que exija; os 16 identificadores conhecidos casam (ver especificação
  léxica §4.1). (LEX-02)

## Decisões da Fase C — gramática (DEC-28 a DEC-46)

Todas são `GRUPO`. Produções citadas (`Pnn`) estão em `gramatica.md` §6. Nenhuma é atribuída à
professora; as marcadas "revisável" podem mudar por instrução dela.

## DEC-28 · Sub-rotinas antes do principal  — DECIDIDA (resolve AMB-05, com DEC-29)

- **Problema:** a página de PROCEDIMENTOS do Anexo I intercala visualmente dois exemplos; não dá para
  reproduzi-la literalmente.
- **Alternativas:** (a) rotinas em qualquer posição; (b) rotinas entre `algoritmo` e `var`/`inicio`;
  (c) copiar a disposição do slide.
- **Decisão:** (b). Sub-rotinas, quando existem, ficam **depois de `ALGORITMO STRING`** e **antes de
  `VAR`/`INICIO` principal** (P03).
- **Justificativa:** cruza as duas evidências seguras: o comentário "Você os declara antes do início
  principal do programa" e o formato das funções (`RotinasComRetorno`: `funcao` antes de `var`). **Não** é
  afirmação sobre o Visualg completo.
- **Impacto:** rotina depois de `var` é erro sintático.

## DEC-29 · Seção `VAR` conservadora  — DECIDIDA (resolve AMB-05, com DEC-28)

- **Problema:** tornar `var` simplesmente opcional aceitaria `algoritmo "X" inicio … fimalgoritmo`, forma
  que não aparece nos exemplos comuns.
- **Decisão:** `VAR` é **obrigatória** quando não há sub-rotina (P02) e **opcional** quando há ≥ 1 (P03,
  P05/P06). Depois de `VAR`, zero ou mais declarações (P08/P09), o que cobre o `var` vazio do
  `PrimeiroPasso`.
- **Justificativa:** acomoda a página de procedimentos (sem `var` aparente) sem liberar a ausência de
  `var` de forma indiscriminada.
- **Impacto:** `algoritmo "X" inicio … fimalgoritmo` é rejeitado.

## DEC-30 · Lista de rotinas  — DECIDIDA

- **Decisão:** zero ou mais rotinas (uma ou mais no caminho P03); cada uma é procedimento ou função, em
  qualquer ordem (P21–P24).
- **Justificativa:** repetir as duas formas declarativas evidenciadas não cria construção nova. O Anexo I
  **não** mostra um programa misturando procedimento e função; a alternativa "um tipo só por programa"
  exigiria listas duplicadas sem base no material.

## DEC-31 · `OU` aceito sintaticamente  — DECIDIDA (fecha AMB-04)

- **Decisão:** `OU` é operador lógico da gramática (P71).
- **Justificativa:** é definido textualmente pelo próprio Anexo I ("O OU, basta um ser verdadeiro") e já é
  token (DEC-14); excluí-lo deixaria uma construção ensinada impossível sintaticamente. Não foi inventado
  externamente.

## DEC-32 · `E` e `OU` no mesmo nível  — DECIDIDA

- **Decisão:** mesma precedência, na mesma cauda (P70/P71); expressões mistas seguem a ordem de leitura;
  parênteses agrupam de outra forma.
- **Justificativa:** o material não diz qual liga primeiro; **não** se inventa que `E` precede `OU`.

## DEC-33 · Sem subtração binária  — DECIDIDA (fecha AMB-06)

- **Decisão:** `MENOS` **não** aparece na gramática de expressões, nem binário (`a - b`) nem unário
  (`-x`, `-1`). `<expr_aditiva_cauda>` só tem `MAIS`.
- **Justificativa:** não há `n1 - n2` no Anexo I; não se completa por simetria com `+` (REQ-04).

## DEC-34 · `MENOS` restrito ao passo  — DECIDIDA

- **Decisão:** `<passo_opcional> -> PASSO <numero_passo> | ε`; `<numero_passo> -> NUM_INT | MENOS NUM_INT`
  (P59–P62).
- **Justificativa:** o **único** uso de `-` no Anexo I é `passo -2`. Aceita também o positivo natural
  `passo 2`; rejeita `passo x`, `passo -x`, `passo 1.5`.

## DEC-35 · Hierarquia de expressões  — DECIDIDA

- **Decisão:** do mais baixo ao mais alto: lógico (`E`, `OU`) → relacional (`OP_REL`, **no máximo um**,
  não encadeável) → aditivo (`MAIS`) → multiplicativo (`OP_MULT`) → primário (P68–P91).
- **Justificativa:** `v MOD 2 = 0` sustenta `MOD` antes da relação; o restante é **decisão estrutural do
  grupo** para uma GLC clara e preditiva, coerente com as categorias ensinadas. O Anexo I **não** prova a
  tabela inteira.
- **Impacto:** `a = b = c` é rejeitado.

## DEC-36 · Fatoração do comando iniciado por `ID`  — DECIDIDA (resolve AMB-09 na gramática)

- **Decisão:** `<cmd_id> -> ID <cauda_comando_id>`, com a cauda `ATRIBUICAO …` | `ABRE_COL … FECHA_COL
  ATRIBUICAO …` | `ABRE_PAR … FECHA_PAR` | `ε` (P44–P48).
- **Alternativa rejeitada:** quatro alternativas `<comando> -> ID …` (prefixo comum; não é LL(1)) ou
  lookahead de 2 tokens (sairia de LL(1)).
- **Impacto:** a forma `ε` faz qualquer `ID` isolado ter a forma de chamada sem parâmetros (limitação
  sintática, `gramatica.md` §16). A verificação formal é da Fase D.

## DEC-37 · Fatoração do primário iniciado por `ID`  — DECIDIDA

- **Decisão:** `<primario> -> ID <cauda_primario>`, com `ABRE_COL … FECHA_COL` | `ABRE_PAR … FECHA_PAR` | `ε`
  (P82, P89–P91). `leia` usa `<referencia> -> ID <indice_opcional>`, sem a forma de chamada.
- **Justificativa:** mesmo motivo de DEC-36. As três caudas são separadas porque aceitam conjuntos
  diferentes.

## DEC-38 · `RETORNE` como comando geral  — DECIDIDA (ampliação assumida)

- **Problema:** o Anexo I só mostra `retorne` dentro de função (inclusive dentro de `se` numa função).
- **Alternativas:** (a) duplicar toda a família de blocos e comandos para funções; (b) aceitar `RETORNE`
  onde qualquer comando é aceito.
- **Decisão:** (b), P43/P64.
- **Justificativa:** (a) dobraria a gramática só para uma restrição **contextual/semântica**.
- **Impacto:** `retorne` no programa principal ou num procedimento é sintaticamente aceito. **Limitação
  documentada**, não escondida (`gramatica.md` §10, §16).

## DEC-39 · Listas de comandos não vazias  — DECIDIDA

- **Decisão:** `<lista_comandos> -> <comando> <lista_comandos_cauda>` (≥ 1 comando) no principal, em `se`,
  `senao`, `para`, `enquanto` e nas rotinas. Não existe comando vazio.
- **Justificativa:** nenhum bloco vazio aparece no Anexo I.

## DEC-40 · Função exige parâmetro; retorno é `<tipo_simples>`  — DECIDIDA

- **Decisão:** `FUNCAO ID ABRE_PAR <lista_parametros> FECHA_PAR DOIS_PONTOS <tipo_simples> …` (P28).
  `funcao f(): inteiro` é rejeitado.
- **Justificativa:** as duas funções do Anexo I têm parâmetros. O retorno aceita os quatro tipos simples
  (o Anexo I mostra `inteiro` e `logico`); restringir a esses dois criaria uma categoria de tipo
  artificial.

## DEC-41 · Procedimento sem parâmetros sem parênteses  — DECIDIDA

- **Decisão:** declaração `PROCEDIMENTO ID <bloco> …` e chamada `ID` (P25, P27, P48). `procedimento p()` e
  `p()` como chamada sem argumento são rejeitados.
- **Justificativa:** o próprio Anexo I diz que procedimentos sem parâmetros não usam parênteses nem na
  declaração nem na chamada.

## DEC-42 · Limites de vetor só `NUM_INT`  — DECIDIDA

- **Decisão:** `VETOR ABRE_COL NUM_INT INTERVALO NUM_INT FECHA_COL DE <tipo_simples>` (P16).
- **Justificativa:** só há literais inteiros (`1..3`, `1..4`); `ID`, expressão, negativo ou real não têm
  evidência.

## DEC-43 · Limites de `para` como `<expressao>`  — DECIDIDA (revisável)

- **Problema:** o Anexo I só usa literais (`1`, `5`, `10`, `0`) em `de … ate …`.
- **Alternativas:** (a) `NUM_INT`; (b) `NUM_INT | ID`; (c) `<expressao>`.
- **Decisão:** (c) (P58).
- **Justificativa:** `de … ate …` recebe **valores**, e o valor na linguagem é a categoria `<expressao>`;
  não cria token nem construção nova; não afeta LL(1). Limite negativo continua impossível (DEC-33).
  A assimetria com o `passo` (DEC-34) é intencional: o passo é o único lugar onde o sinal aparece.
- **Impacto:** `para i de 1 ate n faca` é aceito.

## DEC-44 · Aridade de `leia`, `escreva`/`escreval` e chamadas  — DECIDIDA

- **Decisão:** `leia` com **um** alvo (`ID` ou `ID[…]`) (P49–P52); `escreva`/`escreval` e chamadas com
  **um ou mais** argumentos (P53, P54, P65–P67, P47, P90).
- **Justificativa:** nenhum exemplo de `leia(a, b)`, `escreval()` ou chamada vazia.

## DEC-45 · Parâmetro tipado simples  — DECIDIDA

- **Decisão:** `<parametro> -> ID DOIS_PONTOS <tipo_simples>` (P32).
- **Justificativa:** sem evidência de parâmetro sem tipo, vetor como parâmetro, referência ou valor padrão.

## DEC-46 · `TOKEN_EOF` fora da GLC  — DECIDIDA

- **Decisão:** a gramática termina em `FIMALGORITMO` (P01). Consumir `TOKEN_EOF` depois do programa é
  detalhe do parser (Fase G); no FOLLOW (Fase D) ele entra só como o marcador `$`.
- **Justificativa:** `TOKEN_EOF` não é lexema do programa (DEC-23).

## Decisão da Fase D — validação LL(1)

## DEC-47 · GLC P01–P91 validada como LL(1)  — DECIDIDA (Fase D)

- **Problema:** a Fase C projetou a GLC para LL(1) e só a inspecionou qualitativamente; o parser preditivo
  (DEC-03) precisa de prova.
- **Método:** nullable, FIRST e FOLLOW por ponto fixo sobre as 91 produções publicadas; SELECT de cada
  produção; comparação de todos os pares de alternativas; tabela preditiva. Cálculo manual conferido por
  verificador descartável fora do repositório (0 divergências). Detalhe: `analise-ll1.md`.
- **Resultado:** 17 anuláveis; 24 não-terminais com alternativas, 85 pares, **todas as interseções de
  SELECT vazias**; tabela preditiva com 281 células, **nenhuma duplicada**. `$` só em
  FOLLOW(`<programa>`) e em nenhum SELECT.
- **Decisão:** a GLC da Fase C é aceita **sem emendas**; P01–P91 permanecem idênticas às do commit `a712477`.
- **Impacto na implementação (Fase G):** uma função por não-terminal; **1 token de lookahead** basta;
  sem backtracking; decisões dadas pelo mapa de `analise-ll1.md` §13. `$` corresponde a `TOKEN_EOF`
  (DEC-46). Se a gramática mudar (ex.: resposta da professora sobre AMB-05), a análise LL(1) inteira deve
  ser refeita e registrada.

## Decisões da Fase F — implementação do analisador léxico (DEC-48 a DEC-56)

Todas são `GRUPO`. Detalhe de implementação em `arquitetura.md`. Nenhuma altera o contrato léxico da Fase B.

## DEC-48 · Fonte em bytes; política operacional de encoding  — DECIDIDA (resolve AMB-08 operacionalmente)

- **Problema:** os exemplos têm acentos em strings e comentários, e o arquivo pode estar em UTF-8 ou Latin-1.
- **Decisão:** a fonte é aberta com `"rb"` e lida **por bytes**. Bytes ≥ 0x80 dentro de `STRING` e de comentário
  são conteúdo opaco; **fora** deles são erro léxico. Para o erro, um UTF-8 **bem formado** de 2–4 bytes é
  reportado inteiro (`ç` = `C3 A7`); senão só o byte. ID continua ASCII.
- **Justificativa:** aceita `"João"` em qualquer das duas codificações sem tocar na ER de ID; o erro mostra o
  caractere e não bytes soltos.
- **Impacto:** **não** é suporte a Unicode. A validação UTF-8 é estrutural, sem verificar formas supérfluas.

## DEC-49 · Lexemas alocados dinamicamente  — DECIDIDA

- **Decisão:** cada token possui o seu `lexeme`, montado em um `LexemeBuffer` de capacidade dobrada; `liberarToken()`
  libera. **Sem limite fixo** de tamanho. Falha de memória é falha **operacional**, não erro léxico.
- **Justificativa:** o enunciado não fornece limite; impor `MAX_LEXEME` criaria uma regra que a linguagem não tem
  (fecha parte de AMB-14).

## DEC-50 · Tabela de símbolos dinâmica, busca linear  — DECIDIDA

- **Decisão:** vetor dinâmico de nomes; 1ª ocorrência duplica o nome e devolve `posição + 1`; busca linear.
- **Justificativa:** projeto pequeno, fácil de defender; a **ordem de inserção** já é o atributo esperado nos
  goldens (DEC-07). Sem limite arbitrário de identificadores.

## DEC-51 · Leitor com lookahead próprio, sem múltiplos `ungetc`  — DECIDIDA

- **Decisão:** `Scanner` com vetor de lookahead de 4 bytes; `peekChar(n)` não consome nem altera a linha.
- **Justificativa:** o padrão C só garante um pushback e a regra do ponto (`1..4`) precisa olhar 2 bytes, e a
  validação UTF-8, até 3.

## DEC-52 · `tokens.txt` como arquivo de saída  — DECIDIDA (resolve parte de AMB-14)

- **Problema:** o enunciado exige um arquivo de tokens mas não dá o nome (REQ-22).
- **Decisão:** `tokens.txt` no diretório de trabalho atual, aberto com `"wb"` e sobrescrito a cada execução; entra
  no `.gitignore` (os goldens `*.tokens.txt` **não** são ignorados).
- **Justificativa:** nome simples e previsível, fácil de executar e de comparar.

## DEC-53 · Erro léxico espelhado na tela e no arquivo  — DECIDIDA

- **Decisão:** `ERRO LÉXICO - linha <n> - sequência: <sequência>` (DEC-26) vai a **stdout e a `tokens.txt`**, depois
  dos tokens já emitidos. Erros **operacionais** vão a stderr.
- **Justificativa:** o arquivo reproduz tudo o que o analisador mostrou; a distinção erro de fonte × operacional
  fica explícita.

## DEC-54 · Linha do `TOKEN_EOF`  — DECIDIDA (resolve parte de AMB-14)

- **Decisão:** `TOKEN_EOF.line` = linha lógica do cursor ao observar o fim, depois de descartar whitespace e
  comentários. Conteúdo na linha 4 seguido de `\n` → EOF na 5; sem `\n` final → na 4.
- **Justificativa:** é a posição do cursor após consumir toda a entrada; servirá a "fim de arquivo inesperado".
- **Verificação:** harness descartável com 8 casos de borda, incluindo CRLF e arquivo vazio.

## DEC-55 · `EXIT_SOURCE_ERROR` provisoriamente 0  — DECIDIDA (provisória; **AMB-13 continua aberta**)

- **Decisão:** erro léxico **identificado** → imprime, encerra e sai com **0**; falhas operacionais → ≠ 0. Tudo em
  **uma constante** (`#define EXIT_SOURCE_ERROR 0`).
- **Justificativa:** o critério de avaliação penaliza finalização ≠ 0; enquanto a professora não esclarece, esta é a
  escolha que não pune o grupo. **Não** resolve AMB-13: é provisória, centralizada para troca em uma linha.

## DEC-56 · Token emitido dentro de `obterToken()`  — DECIDIDA

- **Decisão:** `obterToken()` emite o token (tela + arquivo, exceto EOF) **antes** de devolvê-lo.
- **Justificativa:** quando o parser chamar `obterToken()` sob demanda (Fase G), a listagem exigida continuará
  saindo automaticamente. Resolve a dúvida de DEC-04 sobre onde a listagem é produzida.

## Decisões da Fase G — implementação do analisador sintático (DEC-57 a DEC-63)

Todas são `GRUPO`. A gramática P01–P91 e os conjuntos SELECT não foram alterados.

## DEC-57 · Lookahead único e ownership  — DECIDIDA

- **Decisão:** o parser mantém um `Token lookahead` e um `bool lookahead_valido`. `nextToken()` é a única
  função sintática que chama `obterToken()`; antes do avanço, libera o token atual e o marca inválido.
- **Justificativa:** implementa diretamente REQ-32/DEC-03 e deixa o dono de cada lexema inequívoco.
- **Impacto:** sem fila e sem pré-tokenização; a folha da árvore é escrita antes do avanço; cleanup normal ou
  de erro libera o lookahead, inclusive `TOKEN_EOF`, exatamente uma vez.

## DEC-58 · Produção ε somente pelo SELECT formal  — DECIDIDA

- **Decisão:** cada uma das 17 produções ε tem teste explícito do conjunto SELECT publicado em
  `analise-ll1.md`; token fora das alternativas aplicáveis gera erro imediatamente.
- **Justificativa:** evita esconder um token inválido como ε e preserva o primeiro erro formal da tabela LL(1).
- **Impacto:** não existe `default -> ε`; C, F e X são predicados nomeados e as caudas de expressão estendem
  esses conjuntos exatamente como P72, P75, P78, P81 e P91 exigem.

## DEC-59 · Formato do erro sintático  — DECIDIDA

- **Decisão:** `ERRO SINTÁTICO - linha <n> - token: <NOME> - esperado: <...>`.
- **Justificativa:** contém os dados exigidos por REQ-31 e acrescenta expectativa útil sem alterar o token.
- **Impacto:** o nome é a classe lexical (`OP_REL`, não o atributo `GE`); EOF aparece como `EOF`.

## DEC-60 · Erro sintático somente em stdout  — DECIDIDA

- **Decisão:** a mensagem sintática vai a stdout e não a `tokens.txt`.
- **Justificativa:** `tokens.txt` é a saída lexical; misturar um diagnóstico do parser quebraria esse contrato.
- **Impacto:** stdout contém os tokens já solicitados e, por último, o erro; o arquivo contém apenas os tokens.

## DEC-61 · Árvore de derivação textual em pré-ordem  — DECIDIDA (resolve AMB-07)

- **Decisão:** gerar uma representação textual indentada, em pré-ordem e derivação mais à esquerda, com
  `<nao_terminal> [Pnn]`, terminais e `ε`. Em erro, acrescentar `<ERRO SINTATICO>` à árvore parcial.
- **Justificativa:** atende à expectativa pedagógica das aulas sem criar AST ou estrutura semântica.
- **Impacto:** a árvore é escrita diretamente durante o parsing; não há `struct Node` nem ponteiro de lexema
  guardado depois de `nextToken()`.

## DEC-62 · `arvore.txt` separado  — DECIDIDA

- **Decisão:** a árvore vai para `arvore.txt` no diretório de trabalho, aberto com `"wb"` e sobrescrito a
  cada execução; o arquivo entra no `.gitignore`.
- **Justificativa:** preserva stdout como listagem lexical/diagnóstico e `tokens.txt` como artefato lexical.
- **Impacto:** programa válido produz árvore completa; programa inválido produz árvore parcial marcada.

## DEC-63 · `main` executa lexer e parser integrados  — DECIDIDA

- **Decisão:** substituir o driver lexical temporário por `executarAnaliseSintatica()`: abrir a árvore,
  `nextToken()`, `programa()`, `exigirFimArquivo()`, liberar lookahead e fechar a árvore.
- **Justificativa:** cumpre a integração sob demanda já fixada em REQ-32/DEC-04.
- **Impacto:** `FIMALGORITMO` não basta se houver lixo depois; `TOKEN_EOF` é exigido e não é avançado. Erro
  sintático usa o mesmo `EXIT_SOURCE_ERROR` provisório de AMB-13; falhas operacionais continuam não zero.

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

## AMB-02 · Case sensitivity — DECIDIDA na Fase B (DEC-13; revisável se a professora esclarecer)

> **Resolução (Fase B):** léxico case-sensitive. Texto original preservado abaixo como histórico.

- **Problema:** o material não diz se `SE` = `se`, `Mod` = `MOD`.
- **Alternativas:** case-sensitive; case-insensitive; misto (reservadas insensíveis, IDs sensíveis).
- **Uso provisório:** casar apenas a grafia exata do Anexo I (inclusive `MOD` e `E` em maiúsculas).
- **Impacto:** afeta tabela de reservadas e testes. Decidir na Fase B.
- **Risco anotado:** `E` é identificador válido pela ER candidata de ID; só a regra de prioridade
  de reservada o impede. Em case-insensitive, `e` também viraria operador lógico.

## AMB-03 · `<` e `>` isolados — DECIDIDA na Fase B para o léxico (DEC-16; revisável)

> **Resolução (Fase B):** `<` e `>` isolados **não** são tokens; viram erro léxico. Só `<-`, `<>`, `<=`, `>=`.
> Texto original preservado abaixo como histórico.

- **Problema:** há `<-`, `<>`, `<=`, `>=`; não há exemplo de `<` ou `>` sozinhos.
- **Alternativas:** (a) **não** aceitar → `ERRO LÉXICO` se aparecerem; (b) aceitar como relacionais.
- **Posição atual:** não incluir (Anexo I — Operadores confirma só `=`, `<>`, `>=`, `<=`). `<` e `>`
  só existem como prefixo de token composto; ambos ficam **em aberto / não suportados provisoriamente**.
- **Não usar a Figura 2 para decidir:** ela ilustra códigos de operador relacionais que não coincidem
  com os do Anexo I (ver AMB-11).
- **Impacto:** tabela de operadores; casos de teste de erro léxico. Decidir na Fase B.

## AMB-04 · `OU` apenas mencionado — DECIDIDA (léxico: Fase B, DEC-14; gramática: Fase C, DEC-31/DEC-32)

> **Resolução parcial (Fase B):** `OU` é **token reservado próprio** (`TOKEN_OU`), nunca ID, distinto de `E`.
> **Resolução final (Fase C):** `OU` é aceito como operador lógico, no mesmo nível de `E` (P70/P71).

- **Problema:** o Anexo I (Operadores lógicos) traz os comentários "O E exige que os DOIS lados sejam
  verdadeiros" e "O OU, basta um ser verdadeiro", e usa `E` em `podeBrincar <- (idade >= 12) E (altura
  >= 1.50)`. `OU` **não** aparece em nenhuma expressão executável.
- **Classificação:** `E` = confirmado por uso; `OU` = mencionado/definido textualmente. A distinção é
  mantida; `OU` não é esquecido.
- **Alternativas:** (a) entra como operador; (b) fica fora (vira ID, que é estranho); (c) reservado
  para não virar ID, mas sem produção na gramática.
- **Posição atual:** não transformar em ID silenciosamente. Decidir conscientemente na Fase B/C.
- **Impacto:** gramática de expressões (nível lógico) e tabela de reservadas.

## AMB-05 · Estrutura/posição dos procedimentos — DECIDIDA na Fase C (DEC-28, DEC-29)

> **Resolução (Fase C):** sub-rotinas (zero ou mais, procedimentos e funções) ficam depois de
> `ALGORITMO STRING` e antes de `VAR`/`INICIO` principal; `VAR` é obrigatória sem sub-rotina e opcional
> com sub-rotina. Respostas às perguntas abaixo: (1) `var` é opcional **só** quando há sub-rotina;
> (2) sub-rotinas **só** antes de `var`; (3) sim, podem ser misturadas (DEC-30). Texto original
> preservado abaixo como histórico.

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

## AMB-06 · Sinal negativo vs. menos binário — DECIDIDA (léxico: Fase B, DEC-15; gramática: Fase C, DEC-33/DEC-34)

> **Resolução parcial (Fase B):** `-` é `TOKEN_MENOS`; número não tem sinal (`-2` = `MENOS` `NUM_INT(2)`).
> **Resolução final (Fase C):** **sem** subtração binária e **sem** menos unário em expressão; `MENOS` só
> aparece em `<numero_passo> -> MENOS NUM_INT` (P62).

- **Problema:** `para i de 10 ate 0 passo -2 faca` (Anexo I — Repetição) confirma o caractere `-` e a
  forma negativa/unária. Não existe `n1 - n2` em nenhum exemplo.
- **Situação por forma:** sinal negativo / menos unário = **CONFIRMADO** (`-2`); subtração binária =
  **SEM EVIDÊNCIA**. Não adicionar subtração binária por simetria com `+`.
- **Alternativas:** (a) léxico produz `NUM_INT` negativo; (b) léxico produz `MENOS` e `NUM_INT(2)`, e a
  gramática trata a forma negativa; (c) (b) e ainda aceitar `-` binário.
- **Direção:** (b). (a) é desaconselhada: quebraria `a -2` caso um dia houvesse subtração. Tratar a
  forma negativa na gramática é sintaxe, não análise semântica.
- **Impacto:** ER de número, gramática de expressão e de `passo`. Decidir na Fase B/C.

## AMB-07 · Árvore de derivação — RESOLVIDA na Fase G (DEC-61/DEC-62)

> **Resolução (Fase G):** árvore textual indentada em `arvore.txt`, escrita em pré-ordem, com Pxx,
> terminais e `ε`; parcial e marcada em erro. Não há AST nem análise semântica.

- **Problema:** as aulas listam "gerar árvore de derivação" como tarefa do parser; a lista formal da
  Etapa 3 do enunciado não é tão clara.
- **Classificação:** expectativa das aulas, **não** requisito inequívoco do enunciado.
- **Impacto:** formato da saída do parser definido pelas decisões acima.

## AMB-08 · Codificação de caracteres (acentos) — RESOLVIDA OPERACIONALMENTE na Fase F (DEC-48)

> **Resolução (Fase F):** o scanner lê **bytes**; bytes ≥ 0x80 são conteúdo opaco dentro de STRING e comentário
> e erro léxico fora deles; ID continua ASCII. **Não** é suporte geral a Unicode. Texto da Fase B abaixo,
> preservado como histórico.

> **Fase B:** STRING e comentário tratam o conteúdo como **texto opaco**; `"Olá, mundo!"` e `"João"` devem
> ser aceitos; IDs são ASCII. Como os bytes são lidos (UTF-8 vs. Windows-1252) fica para a Fase F.

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

## AMB-09 · Comando iniciado por identificador (risco LL(1) antecipado) — RESOLVIDA (Fase C, DEC-36/DEC-37) e CONFIRMADA formalmente (Fase D, DEC-47)

> **Resolução (Fase C):** fatoração `ID <cauda_comando_id>` (comando) e `ID <cauda_primario>` (expressão).
> **Confirmação (Fase D):** SELECT(P45..P47) = `ATRIBUICAO`, `ABRE_COL`, `ABRE_PAR`; SELECT(P48) = C ∪ F
> (15 tokens); interseções vazias. Idem para P89–P91 (`analise-ll1.md` §9.1–§9.3).

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

## AMB-11 · Figura 2 de Token vs. operadores do MiniVisualg — RESOLVIDA na Fase B (decisão já registrada)

> **Resolução (Fase B):** Figura 2 = **formato/orientação**; Anexo I = **vocabulário**. Aplicado em
> DEC-06, DEC-16, DEC-18 e `especificacao-lexica.md` §14. Segue passível de revisão se a professora
> disser que a Figura 2 deve ser seguida ao pé da letra.

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

## AMB-12 · Nome do token de identificador: `ID` ou `IDENTIFICADOR` — DECIDIDA na Fase B (DEC-17: `ID`)

> **Resolução (Fase B):** nome impresso `ID`, interno `TOKEN_ID`. Texto original preservado abaixo.

- **Problema:** o próprio enunciado exemplifica o formato de saída com `11# IDENTIFICADOR | 1` e
  `11# ID | 1` (REQ-23 ✔PDF). Os dois nomes aparecem.
- **Alternativas:** (a) `ID`; (b) `IDENTIFICADOR`; (c) outra nomenclatura uniforme para todos os tokens.
- **Posição atual:** nada decidido. Ambos os exemplos são ilustrativos; o vocabulário de nomes de token é
  definido na **Fase B** e registrado lá.
- **Impacto:** `enum TokenName`, saída do léxico, testes.

## AMB-13 · Código de retorno em execução com erro léxico/sintático — EM ABERTO (valor provisório no código: DEC-55)

> **Fase F:** o código usa `EXIT_SOURCE_ERROR = 0` **provisoriamente** (DEC-55), centralizado numa linha, porque
> o critério de avaliação penaliza retorno ≠ 0. **A ambiguidade NÃO está resolvida**: continua a recomendação de
> confirmar com a professora antes da entrega.

- **Problema:** o enunciado diz duas coisas que podem entrar em tensão:
  (A) ao ocorrer erro léxico/sintático, apresentar a mensagem exigida e **finalizar todo o processo**;
  (B) nos critérios de avaliação, o programa que **não finalizar com retorno igual a 0** sofre penalização
  (REQ-11). Se o próprio teste contém um erro proposital, qual exit status é o esperado?
- **Fixado:**
  - entrada **válida** termina com retorno **0**;
  - erro léxico/sintático **obrigatoriamente encerra** o processamento após a mensagem (DEC-10).
- **NÃO decidido:** o exit status específico do caso de erro. **Não** assumir `return 1` por convenção
  sem considerar o critério de avaliação.
- **Alternativas:** (a) sair com 0 após reportar o erro (o "tratamento" do erro foi bem-sucedido); (b) sair com
  código ≠ 0 (convenção Unix); (c) o que a professora indicar.
- **Ação:** idealmente **confirmar com a professora antes da entrega**; decisão final nas Fases H/I.
- **Impacto:** testes de erro não verificam exit status até lá; `main` de `compilador.c`.

## AMB-14 · Detalhes operacionais da saída léxica — PARCIALMENTE RESOLVIDA na Fase F

> **Resolvido na Fase F:** (1) arquivo de saída = `tokens.txt` (DEC-52); (2) erro léxico em stdout **e**
> `tokens.txt`, erros operacionais em stderr (DEC-53); (3) limites: lexemas e TS sem limite fixo, graças às
> estruturas dinâmicas (DEC-49, DEC-50); estouro de `long long`/`double` é falha de **representação do
> implementador**, não regra da linguagem, e sai com status ≠ 0; (4) linha do `TOKEN_EOF` = posição lógica do
> cursor (DEC-54). **Pode permanecer em aberto:** detalhes da saída integrada ao parser, se houver. Texto
> original abaixo, preservado como histórico.

- **Problema:** o material não define, e esta fase não congela: (1) o **nome/caminho do arquivo de saída**
  de tokens; (2) **onde** a mensagem de erro é impressa (stdout/stderr) e se também entra no arquivo de
  saída; (3) **limites**: estouro de `NUM_INT` além do inteiro do C, comprimento máximo de ID/STRING.
- **Fixado:** o **conteúdo** das mensagens e da listagem (DEC-22, DEC-26); `NUM_INT` é `[0-9]+` sem limite
  lexical declarado.
- **Acrescentado na Fase E:** (4) a **linha atribuída ao `TOKEN_EOF`** quando o arquivo termina com `\n`: a
  última linha com conteúdo ou a seguinte? Afeta a linha reportada num erro sintático "fim de arquivo
  inesperado". O teste SY-E19 evita a dúvida terminando sem `\n` final; a decisão fica para a Fase F/G.
- **Ação:** decidir na Fase F (nenhum desses pontos altera o vocabulário nem a gramática).

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
| AMB-02 case sensitivity | **Decidida (DEC-13, conservadora), revisável:** o material não diz; só muda se a professora esclarecer. |
| AMB-03 `<` e `>` isolados | **Decidida no léxico (DEC-16), revisável:** sem exemplo; a Figura 2 sugere outra coisa (AMB-11). |
| AMB-04 `OU` na gramática | **Decidida (DEC-31/DEC-32):** aceito, no mesmo nível de `E`. Revisável se a professora restringir. |
| AMB-05 estrutura de procedimentos | **Decidida (DEC-28/DEC-29)** por reconstrução; a página continua visualmente inconsistente. Boa candidata a confirmar com a professora. |
| AMB-06 subtração binária | **Decidida (DEC-33/DEC-34):** sem subtração; `MENOS` só no passo. Revisável. |
| DEC-38 `retorne` fora de função | Aceito sintaticamente (limitação contextual documentada). |
| DEC-43 limites de `para` | `<expressao>` em vez de literal; revisável. |
| AMB-07 árvore de derivação obrigatória? | **Resolvida (DEC-61/DEC-62):** árvore textual em `arvore.txt`, sem AST. |
| AMB-10 parênteses em `se`/`enquanto` | Decidido de forma conservadora; revisável. |
| AMB-11 seguir a Figura 2 literalmente? | **Resolvida (formato vs. vocabulário)**, revisável: o enunciado a apresenta como ilustrativa. |
| AMB-12 nome do token (`ID`/`IDENTIFICADOR`) | **Decidida (DEC-17: `ID`).** O enunciado usa ambos nos exemplos; não se afirma que o outro esteja errado. |
| AMB-13 código de retorno em erro | O enunciado exige encerrar **e** penaliza retorno ≠ 0. **Confirmar com a professora.** O código usa 0 provisoriamente (DEC-55). |

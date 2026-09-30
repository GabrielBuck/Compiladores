# Especificação léxica do MiniVisualg — contrato da Fase B

> **Status: CONTRATO LÉXICO CONGELADO (Fase B).** Este é o documento que `obterToken()` implementará
> na Fase F e de onde a Fase C (GLC) tirará seus terminais. Nenhum código existe ainda. Qualquer
> alteração de vocabulário depois desta fase exige emenda registrada em `decisoes.md`.
>
> **Fonte de verdade:** Anexo I do PDF oficial + requisitos confirmados (`requisitos.md`).
> Nada vem do Visualg completo. Onde faltou evidência, a lacuna está documentada, não preenchida.
>
> **Origem das regras.** Cada regra abaixo tem um ID `LEX-nn` (rastreável até teste na Fase E) e a
> origem dela: `ENUNCIADO`, `ANEXO I`, `AULA`, `GRUPO`. As regras decididas nesta fase são `GRUPO`,
> salvo indicação. **Nenhuma decisão aqui é atribuída à professora.**

## 1. Objetivo

Definir, sem ambiguidade, o que o analisador léxico reconhece e como se comporta:

- quais tokens existem e como cada um é nomeado na saída;
- quais sequências formam cada token (ERs e lexemas fixos);
- o que **não** é suportado e vira `ERRO LÉXICO`;
- quais tokens carregam atributo e qual;
- como palavras reservadas se distinguem de IDs;
- como operadores compostos, strings, comentários, espaços e linhas são tratados;
- o formato da saída textual.

Fica **fora** deste documento: a gramática (Fase C), a política operacional de codificação de bytes
(Fase F), o layout de memória do `struct` (Fase F) e qualquer código C.

## 2. Princípios da especificação

1. **Anexo I é o vocabulário.** Só entra o que o Anexo I evidencia ou define textualmente
   (REQ-04). Sem completar famílias por simetria (`-` binário, `<`, `>`, `NAO`, `^` …).
2. **Case-sensitive** em tudo, sem exceção (LEX-01).
3. **Determinismo:** cada entrada tem exatamente uma tokenização ou um erro léxico. O scanner escolhe
   sempre o lexema válido mais longo (maximal munch, §12).
4. **Híbrido (DEC-05):** lexemas fixos em catálogo declarativo + reconhecedores próprios (conceitualmente
   pequenos AFDs) para ID, inteiro, real e string. Responde a REQ-27.
5. **O scanner aceita os exemplos oficiais, com ou sem espaço** ao redor de pontuação (REQ-14 × AMB-01,
   DEC-05). A contradição do material continua registrada; aqui só se define o comportamento.
6. **Primeiro erro encerra** o processamento (DEC-10). Sem recuperação.
7. **Léxico não é sintaxe.** Que `OU` exista como token não implica que a gramática o aceite; que `-`
   exista como token não implica subtração binária.

## 3. Vocabulário lexical

O MiniVisualg tem **50 nomes de token internos**, dos quais **49 são impressos** (`TOKEN_EOF` não é
impresso, §27/§16). A tabela é a **fonte única** de nomes: documentação, testes e código devem usar
exatamente estes nomes.

Convenção: nome interno = `TOKEN_…` (futuro `enum TokenName`); **nome impresso** = o que aparece no
arquivo de saída. Nas palavras reservadas, nome impresso = lexema em maiúsculas, sem o prefixo.

### 3.A Controle

| Token interno | Nome impresso | Lexema / classe | Atributo |
|---|---|---|---|
| `TOKEN_EOF` | *(não impresso)* | fim da entrada (não é lexema do programa) | — |

### 3.B Tokens de classe (LEX-02, 04, 05, 07)

| Token interno | Nome impresso | Lexema / classe (ER) | Atributo |
|---|---|---|---|
| `TOKEN_ID` | `ID` | `[A-Za-z][A-Za-z0-9_]*`, exceto lexemas do catálogo alfabético | índice na TS |
| `TOKEN_NUM_INT` | `NUM_INT` | `[0-9]+` | valor inteiro |
| `TOKEN_NUM_REAL` | `NUM_REAL` | `[0-9]+\.[0-9]+` | valor real |
| `TOKEN_STRING` | `STRING` | `"[^"\r\n]*"` | texto literal (§23) |

### 3.C Palavras reservadas estruturais (31 — um token distinto cada; LEX-03)

Nenhuma tem atributo.

| Token interno | Nome impresso | Lexema |
|---|---|---|
| `TOKEN_KW_ALGORITMO` | `ALGORITMO` | `algoritmo` |
| `TOKEN_KW_VAR` | `VAR` | `var` |
| `TOKEN_KW_INICIO` | `INICIO` | `inicio` |
| `TOKEN_KW_FIMALGORITMO` | `FIMALGORITMO` | `fimalgoritmo` |
| `TOKEN_KW_INTEIRO` | `INTEIRO` | `inteiro` |
| `TOKEN_KW_REAL` | `REAL` | `real` |
| `TOKEN_KW_CARACTERE` | `CARACTERE` | `caractere` |
| `TOKEN_KW_LOGICO` | `LOGICO` | `logico` |
| `TOKEN_KW_VERDADEIRO` | `VERDADEIRO` | `verdadeiro` |
| `TOKEN_KW_FALSO` | `FALSO` | `falso` |
| `TOKEN_KW_LEIA` | `LEIA` | `leia` |
| `TOKEN_KW_ESCREVA` | `ESCREVA` | `escreva` |
| `TOKEN_KW_ESCREVAL` | `ESCREVAL` | `escreval` |
| `TOKEN_KW_SE` | `SE` | `se` |
| `TOKEN_KW_ENTAO` | `ENTAO` | `entao` |
| `TOKEN_KW_SENAO` | `SENAO` | `senao` |
| `TOKEN_KW_FIMSE` | `FIMSE` | `fimse` |
| `TOKEN_KW_PARA` | `PARA` | `para` |
| `TOKEN_KW_DE` | `DE` | `de` |
| `TOKEN_KW_ATE` | `ATE` | `ate` |
| `TOKEN_KW_PASSO` | `PASSO` | `passo` |
| `TOKEN_KW_FACA` | `FACA` | `faca` |
| `TOKEN_KW_FIMPARA` | `FIMPARA` | `fimpara` |
| `TOKEN_KW_ENQUANTO` | `ENQUANTO` | `enquanto` |
| `TOKEN_KW_FIMENQUANTO` | `FIMENQUANTO` | `fimenquanto` |
| `TOKEN_KW_VETOR` | `VETOR` | `vetor` |
| `TOKEN_KW_PROCEDIMENTO` | `PROCEDIMENTO` | `procedimento` |
| `TOKEN_KW_FIMPROCEDIMENTO` | `FIMPROCEDIMENTO` | `fimprocedimento` |
| `TOKEN_KW_FUNCAO` | `FUNCAO` | `funcao` |
| `TOKEN_KW_FIMFUNCAO` | `FIMFUNCAO` | `fimfuncao` |
| `TOKEN_KW_RETORNE` | `RETORNE` | `retorne` |

### 3.D Operadores-palavra

| Token interno | Nome impresso | Lexema | Atributo |
|---|---|---|---|
| `TOKEN_E` | `E` | `E` | — |
| `TOKEN_OU` | `OU` | `OU` | — |
| *(`MOD` é `TOKEN_OP_MULT`, ver 3.E)* | | `MOD` | `OP_MOD` |

### 3.E Operadores simbólicos e agrupados

| Token interno | Nome impresso | Lexemas | Atributo (enum interno → impresso) |
|---|---|---|---|
| `TOKEN_OP_REL` | `OP_REL` | `=` `<>` `<=` `>=` | `OP_EQ`→`EQ`, `OP_NE`→`NE`, `OP_LE`→`LE`, `OP_GE`→`GE` |
| `TOKEN_OP_MULT` | `OP_MULT` | `*` `/` `\` `MOD` | `OP_MUL`→`MUL`, `OP_DIV_REAL`→`DIV_REAL`, `OP_DIV_INT`→`DIV_INT`, `OP_MOD`→`MOD` |
| `TOKEN_MAIS` | `MAIS` | `+` | — |
| `TOKEN_MENOS` | `MENOS` | `-` | — |
| `TOKEN_ATRIBUICAO` | `ATRIBUICAO` | `<-` | — |

### 3.F Pontuação

| Token interno | Nome impresso | Lexema |
|---|---|---|
| `TOKEN_ABRE_PAR` | `ABRE_PAR` | `(` |
| `TOKEN_FECHA_PAR` | `FECHA_PAR` | `)` |
| `TOKEN_ABRE_COL` | `ABRE_COL` | `[` |
| `TOKEN_FECHA_COL` | `FECHA_COL` | `]` |
| `TOKEN_DOIS_PONTOS` | `DOIS_PONTOS` | `:` |
| `TOKEN_VIRGULA` | `VIRGULA` | `,` |
| `TOKEN_INTERVALO` | `INTERVALO` | `..` |

**Não existem** `TOKEN_ASPAS`, `TOKEN_COMENTARIO`, `TOKEN_LT`, `TOKEN_GT`, `TOKEN_NAO`.

Contagem: 1 + 4 + 31 + 2 + 2 + 3 + 7 = **50**. Nomes impressos são todos distintos entre si
(`REAL` é a palavra reservada; `NUM_REAL` é o número).

## 4. Tokens de classe e ERs

Notação: `[a-z]` classe de caracteres; `*` zero ou mais; `+` um ou mais; `\.` ponto literal.

### 4.1 ID — LEX-02  (ANEXO I + GRUPO)

```
ID = [A-Za-z][A-Za-z0-9_]*
```

- Só letras ASCII no início; depois letras, dígitos e `_`.
- **Não** aceita `_` como primeiro caractere (sem evidência) — `_x` é erro léxico em `_`.
- **Não** aceita letra acentuada (sem exemplo que exija) — `João` como identificador gera `Jo` e depois
  erro no `ã`.
- Sensível à caixa: `nome` e `Nome` são IDs diferentes.

Conferência contra os exemplos conhecidos (todos casam com a ER e nenhum é reservado):

| Lexema | Casa? | | Lexema | Casa? |
|---|---|---|---|---|
| `nome` | sim | | `linha_decorativa` | sim (contém `_`) |
| `sobrenome` | sim | | `mostrar_erro` | sim (contém `_`) |
| `portaAberta` | sim | | `somar` | sim |
| `ehMaiorDeIdade` | sim | | `eh_par` | sim (contém `_`) |
| `senhaDigitada` | sim | | `resultado` | sim |
| `podeBrincar` | sim | | `n1`, `n2` | sim (dígito no fim) |
| `contador` | sim | | `v`, `i` | sim (uma letra) |

Classificação (LEX-03, ordem fixa): (1) reconhecer o lexema pela ER; (2) consultar o **catálogo
alfabético** (§6); (3) se constar, devolver o token correspondente; (4) senão `TOKEN_ID`; (5) `TOKEN_ID`
consulta/insere na TS (§13).

### 4.2 NUM_INT — LEX-04  (ANEXO I + GRUPO)

```
NUM_INT = [0-9]+
```

Exemplos do Anexo I: `0 1 2 3 4 5 10 12 14 18 1234`. **O sinal não pertence ao número**: `-2` é
`MENOS` `NUM_INT(2)`, nunca `NUM_INT(-2)` (resolve a parte lexical de AMB-06; DEC-15).

### 4.3 NUM_REAL — LEX-05  (ANEXO I + GRUPO)

```
NUM_REAL = [0-9]+\.[0-9]+
```

Exemplos: `1.60`, `1.50`. **Não** suportados (sem evidência): `.5`, `1.`, `1,5`.

### 4.4 Regra do ponto depois de dígitos — LEX-06

Depois de reconhecer uma sequência de dígitos, o scanner olha o que vem a seguir, **sem consumir**:

| Seguinte | Resultado |
|---|---|
| `.` seguido de **dígito** | continua: forma `NUM_REAL` |
| `..` | o inteiro terminou; o próximo token é `INTERVALO` |
| `.` seguido de qualquer outra coisa (inclui fim de linha e EOF) | o inteiro terminou; o `.` fica para o próximo token e, por ser um `.` isolado, será erro léxico (§17) |
| qualquer outro caractere | o inteiro terminou |

Consequências:

- `vetor[1..4]` → `NUM_INT(1)` `INTERVALO` `NUM_INT(4)`. **Nunca** um real malformado.
- `1.5` → `NUM_REAL(1.5)`; `1.` → `NUM_INT(1)` e depois erro em `.`.
- `1...4` → `NUM_INT(1)` `INTERVALO` e depois erro em `.`.
- `1.5.3` → `NUM_REAL(1.5)` e depois erro em `.` (um `.` seguido de dígito **fora** de um número não
  inicia nada: `.` só é válido como `..`).

Nota para a implementação: decidir entre "real" e "inteiro + `..`" exige enxergar **dois** caracteres à
frente do fim dos dígitos (`.` e o seguinte). Isso será tratado na Fase F (§12); aqui só se exige o
comportamento.

### 4.5 STRING — LEX-07  (ANEXO I + GRUPO)

```
STRING = "[^"\r\n]*"
```

- Delimitada por aspas duplas; as aspas são consumidas pelo scanner e **não** geram token próprio.
- O conteúdo é **texto opaco**: pode ter espaços, pontuação, `//`, acentos, `\`, `'`.
  `"http://x"` é **uma** STRING (o `//` é conteúdo).
- **Sem escapes:** `\"`, `\n`, `\t`, `\\` **não** são sequências especiais; a `\"` fecha a string na aspa.
- `""` (vazia) é uma STRING válida.
- STRING aberta e não fechada antes de `\n`, `\r\n` ou EOF → `ERRO LÉXICO` (§17).
- Exemplos oficiais que devem ser aceitos: `"PrimeiroPasso"`, `"Olá, mundo!"`, `"João"`,
  `"Digite seu nome: "`.
- Acentos: o conteúdo é opaco para a classificação (AMB-08). A leitura concreta dos bytes é decidida
  na Fase F.

## 5. Lexemas fixos

Dois catálogos declarativos (DEC-05), ambos consultados por **correspondência exata**:

| Catálogo | Conteúdo | Quando é consultado |
|---|---|---|
| **Alfabético** (34 lexemas) | as 31 palavras reservadas (§6) + `MOD`, `E`, `OU` (§7) | depois que o reconhecedor de ID leu um lexema completo |
| **Simbólico** (17 lexemas) | operadores (§7) e pontuação (§8) | pelo primeiro caractere não-letra/dígito/aspas, com maior casamento (§12) |

Cada entrada do catálogo liga: **lexema → token interno → atributo (se houver)**. O mapeamento completo
está na tabela do §3; o catálogo em código deve ser uma tabela de dados, sem `if/else` em cadeia.

## 6. Palavras reservadas — LEX-03

As 31 palavras abaixo são reservadas, **sensíveis à caixa** (LEX-01) e **não entram na TS**.
Evidência de todas: Anexo I, uso executável.

| Grupo | Lexemas | Exemplo do Anexo I |
|---|---|---|
| Estrutura do programa | `algoritmo` `var` `inicio` `fimalgoritmo` | `algoritmo "PrimeiroPasso"` |
| Tipos | `inteiro` `real` `caractere` `logico` | `altura: real`, `portaAberta: logico` |
| Valores lógicos | `verdadeiro` `falso` | `portaAberta <- verdadeiro`, `retorne falso` |
| Entrada e saída | `leia` `escreva` `escreval` | `leia(nome)`, `escreva("Bem-vindo ao ")` |
| Condicional | `se` `entao` `senao` `fimse` | `se (chovendo = verdadeiro) entao` |
| Repetição `para` | `para` `de` `ate` `passo` `faca` `fimpara` | `para i de 10 ate 0 passo -2 faca` |
| Repetição `enquanto` | `enquanto` `fimenquanto` | `enquanto (contador <= 5) faca` |
| Vetor | `vetor` | `notas: vetor[1..4] de real` |
| Sub-rotinas | `procedimento` `fimprocedimento` `funcao` `fimfuncao` `retorne` | `funcao somar(a: inteiro, b: inteiro): inteiro` |

Consequências:

- `leia`, `escreva` e `escreval` são **reservadas** (decisão desta fase; fechava a pendência deixada em
  `especificacao-minivisualg.md`). Não podem ser usadas como identificador.
- `de` e `ate` têm forma de ID, mas a reserva tem prioridade (LEX-03).
- **Nenhuma palavra fora desta lista** é reservada. Ex.: `repita`, `escolha`, `nao` **não** são reservadas;
  seriam IDs.
- Por ser case-sensitive, `Algoritmo` e `ALGORITMO` são **IDs**, não a palavra reservada (lexicalmente
  válidos; a sintaxe os rejeitará onde a palavra reservada fosse exigida).

## 7. Operadores

### 7.1 Operadores-palavra — LEX-01, LEX-14

| Lexema | Token | Evidência |
|---|---|---|
| `MOD` | `TOKEN_OP_MULT` com atributo `OP_MOD` | **Confirmado por uso:** `v MOD 2 = 0` |
| `E` | `TOKEN_E` | **Confirmado por uso:** `(idade >= 12) E (altura >= 1.50)` |
| `OU` | `TOKEN_OU` | **Mencionado/definido textualmente** ("O OU, basta um ser verdadeiro"); nenhuma expressão executável o usa |

- São reconhecidos **exatamente** nesta grafia. `mod`, `Mod`, `e`, `ou` são **IDs**.
- `OU` é **lexema reservado próprio**, **nunca ID**. É `TOKEN_OU`, distinto de `TOKEN_E`, para que a Fase C
  possa decidir **conscientemente** se ambos entram na gramática sem perder a distinção entre "confirmado
  por uso" e "apenas mencionado". **Aceitar `OU` numa expressão é decisão sintática e fica para a Fase C**
  (DEC-14; parte lexical de AMB-04).

### 7.2 Operadores simbólicos

| Lexema | Token | Atributo | Evidência |
|---|---|---|---|
| `<-` | `TOKEN_ATRIBUICAO` | — | `nome <- "Florêncio"` |
| `+` | `TOKEN_MAIS` | — | `n1 + n2` |
| `-` | `TOKEN_MENOS` | — | `passo -2` (uso confirmado: sinal negativo/unário) |
| `*` | `TOKEN_OP_MULT` | `OP_MUL` | `n1 * n2` |
| `/` | `TOKEN_OP_MULT` | `OP_DIV_REAL` | `n1 / n2` |
| `\` | `TOKEN_OP_MULT` | `OP_DIV_INT` | `n1 \ n2` |
| `=` | `TOKEN_OP_REL` | `OP_EQ` | `senhaDigitada = 1234` |
| `<>` | `TOKEN_OP_REL` | `OP_NE` | `nome <> "João"` |
| `<=` | `TOKEN_OP_REL` | `OP_LE` | `contador <= 5` |
| `>=` | `TOKEN_OP_REL` | `OP_GE` | `idade >= 18` |

### 7.3 Agrupamentos e por que existem

- **`TOKEN_OP_REL`** (`= <> <= >=`): uma classe com atributo, como a Figura 2 sugere para operadores
  relacionais. Só quatro valores: **não** há `OP_LT` nem `OP_GT` (DEC-16, DEC-18).
- **`TOKEN_OP_MULT`** (`* / \ MOD`): uma classe com atributo, porque na futura gramática os quatro ocupam
  o mesmo nível estrutural; a distinção específica fica preservada no atributo (DEC-19).
- **`TOKEN_MAIS`** separado: `+` não pertence a nenhuma das duas classes.
- **`TOKEN_MENOS`** separado de `TOKEN_MAIS` e fora de qualquer classe: o uso confirmado de `-` é unário
  (`-2`); a categoria lexical não deve **implicar** subtração binária, que continua **SEM EVIDÊNCIA**
  (AMB-06; DEC-15).
- **`TOKEN_E` / `TOKEN_OU`** não são agrupados (§7.1).

## 8. Pontuação

| Lexema | Token | Evidência |
|---|---|---|
| `(` `)` | `TOKEN_ABRE_PAR` `TOKEN_FECHA_PAR` | `leia(nome)`, `(idade >= 12)` |
| `[` `]` | `TOKEN_ABRE_COL` `TOKEN_FECHA_COL` | `nomes[1]`, `vetor[1..3]` |
| `:` | `TOKEN_DOIS_PONTOS` | `idade: inteiro` |
| `,` | `TOKEN_VIRGULA` | `somar(10, 5)`, `nome, sobrenome: caractere` |
| `..` | `TOKEN_INTERVALO` | `vetor[1..4]` |

Sem atributo. `.` **isolado** não é token (§17). Aspas não geram token. `//` não gera token (§9).

## 9. Comentários — LEX-08

```
COMENTARIO = //[^\r\n]*
```

- Começa com `//` (fora de string) e vai até **antes** da quebra de linha, ou até EOF.
- É **descartado**: não gera token, não chega ao parser. O conteúdo **não** é processado como código
  (pode ter aspas, símbolos inválidos, acentos).
- A quebra de linha que o termina **não** é consumida pelo comentário: ela segue a regra do §10 e a
  linha seguinte é contada corretamente.
- Comentário no último trecho do arquivo, sem `\n` final, é válido.
- **Não** existe `/* … */` (sem evidência; DEC-08). `/*` é `/` (`OP_MULT`) seguido de `*` (`OP_MULT`),
  lexicamente válido e sintaticamente inútil.
- `//` dentro de STRING é conteúdo; `"` dentro de comentário é conteúdo.

## 10. Whitespace e linhas — LEX-09

- **Whitespace** = espaço (`' '`), tabulação (`\t`), LF (`\n`) e CR (`\r`). Não gera token; só separa.
- **Contagem de linhas:** começa em 1; **incrementa em cada `\n`**.
  - LF → uma nova linha.
  - **CRLF → uma** nova linha lógica (o `\r` é whitespace e não conta).
  - CR isolado (sem LF) é whitespace e **não** incrementa a linha.
- A **linha de um token** é a linha do seu **primeiro caractere**.
- Outros caracteres de controle (`\f`, `\v`, NUL…) **não** são whitespace: são caracteres inválidos (§17).
- Relação com o enunciado: o enunciado manda considerar lexemas separados por espaço (REQ-14); o Anexo I
  não obedece (AMB-01). O scanner **não depende** de espaços: `leia(nome)` e `leia ( nome )` tokenizam
  igual. Indentação não tem significado (REQ-03).

## 11. Estratégia de reconhecimento

Scanner **caractere a caractere** (DEC-05). Decisão pelo **primeiro caractere** do próximo lexema, depois
de pular whitespace e comentários:

| Primeiro caractere | Ação |
|---|---|
| whitespace | consumir e continuar |
| `A-Z` `a-z` | reconhecedor de **ID**; depois classificação pelo catálogo alfabético (§4.1) |
| `0-9` | reconhecedor de **número** (§4.2–4.4) |
| `"` | reconhecedor de **STRING** (§4.5) |
| `/` | se o seguinte é `/`: **comentário** (§9); senão, `/` é `OP_MULT`/`OP_DIV_REAL` |
| qualquer outro | **catálogo simbólico** com maior casamento (§12); sem casamento → **erro léxico** |
| fim da entrada | `TOKEN_EOF` |

Reconhecedores de classe, como AFDs (estados conceituais):

- **ID:** `q0 --letra--> q1`; `q1 --letra|dígito|_--> q1`; qualquer outro → aceita em `q1`.
- **Número:** `q0 --dígito--> qInt`; `qInt --dígito--> qInt`; em `qInt`, olhar adiante (§4.4):
  `.`+dígito → `qPonto --dígito--> qReal`; `qReal --dígito--> qReal`; demais casos aceita como inteiro.
- **STRING:** `q0 --"--> qCorpo`; `qCorpo --qualquer exceto " \r \n--> qCorpo`; `qCorpo --"--> aceita`;
  `qCorpo --\r|\n|EOF--> erro` (string não fechada).
- **Comentário:** `q0 --/--> qBarra --/--> qCom`; `qCom --qualquer exceto \r \n--> qCom`;
  `qCom --\r|\n|EOF--> descarta` (volta ao laço principal).

Esta tabela é o "porquê" de não bastar comparar com uma lista (REQ-27, **AULA**): ID, inteiro, real e
string são **classes infinitas**.

## 12. Maximal munch — LEX-10

**Regra:** a cada passo, o scanner devolve o **lexema válido mais longo** que começa na posição atual,
**sem consumir** caracteres que pertençam ao token seguinte. Se nenhum lexema válido começa ali, é erro
léxico na posição atual.

| Entrada | Tokenização | Observação |
|---|---|---|
| `<=` | `OP_REL(LE)` | não `<` + `=` |
| `<-` | `ATRIBUICAO` | |
| `<>` | `OP_REL(NE)` | |
| `<` seguido de outro caractere | **erro léxico em `<`** | `<` isolado não é token |
| `>=` | `OP_REL(GE)` | |
| `>` seguido de outro caractere | **erro léxico em `>`** | `>` isolado não é token |
| `..` | `INTERVALO` | |
| `...` | `INTERVALO`, depois erro em `.` | o mais longo válido é `..` |
| `.` isolado | **erro léxico em `.`** | |
| `//` | comentário | |
| `/` seguido de não-`/` | `OP_MULT(DIV_REAL)` | |
| `1..4` | `NUM_INT(1)` `INTERVALO` `NUM_INT(4)` | regra do §4.4 |
| `1.5` | `NUM_REAL(1.5)` | |
| `12abc` | `NUM_INT(12)` `ID(abc)` | sem erro léxico: a sintaxe rejeita |
| `abc12` | `ID(abc12)` | dígito continua o ID |
| `a<-2` | `ID` `ATRIBUICAO` `NUM_INT` | sem ambiguidade: `<` isolado não existe |
| `a<>b`, `x<=3`, `n>=0` | normais | sem espaço também funciona |

**Lookahead de entrada.** Até aqui: 1 caractere basta para `<`, `>`, `/`; decidir entre `NUM_REAL`
e `NUM_INT` + `..` exige enxergar **2 caracteres** depois dos dígitos. A Fase F escolherá o mecanismo
(`ungetc` tem garantia de 1; `peek` em buffer é alternativa). **Esta fase não determina a implementação
em C**; só exige o comportamento da tabela.

## 13. Tabela de símbolos — LEX-15  (AULA + DEC-07)

- **Só identificadores** (`TOKEN_ID`). Não contém palavras reservadas, strings, números, tipos, escopo,
  endereço ou valores (tudo isso é análise posterior).
- Chave: o lexema **exato** (case-sensitive): `nome` ≠ `Nome`.
- **1ª ocorrência:** insere e devolve índice; **ocorrências seguintes:** reutilizam o mesmo índice.
- Índices começam em **1** e são **estáveis** (nunca mudam, nunca são reaproveitados), coerente com o
  exemplo do enunciado `11# ID | 1`.

```
nome       -> ID | 1
sobrenome  -> ID | 2
nome       -> ID | 1
```

- Nomes de sub-rotinas (`somar`, `linha_decorativa`) também são `TOKEN_ID` e também entram na TS: o
  léxico não distingue variável de sub-rotina (isso seria semântica).

## 14. Estrutura conceitual de Token — LEX-16  (ENUNCIADO — Etapa 2 + GRUPO)

**Sem C.** Contrato lógico do futuro `struct` (REQ-20):

| Campo conceitual | Conteúdo | Para quê |
|---|---|---|
| `type` | um dos 50 `TokenName` (§3) | o parser decide por ele |
| `line` | linha do primeiro caractere (§10) | saída; erro sintático (REQ-31) |
| `lexeme` | texto exato lido da entrada | diagnóstico; o valor textual da STRING |
| `attribute` | conteúdo dependente do `type` (abaixo) | saída; valor do número; TS |

O `type` **determina** qual variante do atributo é válida, então **não é preciso um discriminador
separado**.

| `type` | Variante do atributo |
|---|---|
| `TOKEN_ID` | índice na TS |
| `TOKEN_NUM_INT` | valor inteiro |
| `TOKEN_NUM_REAL` | valor real |
| `TOKEN_STRING` | texto literal |
| `TOKEN_OP_REL` | subtipo relacional (`EQ` `NE` `LE` `GE`) |
| `TOKEN_OP_MULT` | subtipo multiplicativo (`MUL` `DIV_REAL` `DIV_INT` `MOD`) |
| demais | **nenhum** atributo significativo |

**Como isto deriva da Figura 2** (ENUNCIADO — Figura 2, referência ilustrativa; REQ-21): mantemos a
ideia central — **token = tipo + linha + atributo variável** — e a ideia de **classe de token + atributo**
para operadores relacionais. **Adaptamos** ao MiniVisualg (REQ-29, DEC-06): (1) o atributo tem variantes
para o que o Anexo I realmente exige — índice de TS, inteiro, real, texto, subtipo relacional e subtipo
multiplicativo; (2) acrescentamos o `lexeme`; (3) as palavras reservadas têm **um tipo de token cada**, sem
atributo, para deixar o parser legível. O **vocabulário** vem do Anexo I, não da Figura 2 (AMB-11).
O layout de memória (union, buffers) é decisão da Fase F.

## 15. Política de atributos — LEX-16

| Token | Atributo | Valor | Como sai na listagem |
|---|---|---|---|
| `ID` | índice na TS | inteiro ≥ 1 | `ID \| 1` |
| `NUM_INT` | valor inteiro | o número | `NUM_INT \| 18` |
| `NUM_REAL` | valor real | o número | `NUM_REAL \| 1.60` (lexema como escrito) |
| `STRING` | texto literal | conteúdo entre aspas | `STRING \| "Ana"` (aspas incluídas) |
| `OP_REL` | subtipo | `EQ` `NE` `LE` `GE` | `OP_REL \| GE` |
| `OP_MULT` | subtipo | `MUL` `DIV_REAL` `DIV_INT` `MOD` | `OP_MULT \| MOD` |
| todos os demais | — | **nenhum** | sem `\|` |

Regras:

- Números saem **como escritos** no código (`1.60`, não `1.6`; `007` permanece `007`): o valor numérico é
  guardado à parte, mas a listagem mostra o lexema.
- **STRING preserva o valor textual** (DEC-21): a listagem distingue `STRING | "Ana"` de `STRING | "João"`.
  Como a STRING é o texto entre aspas e a listagem mostra o lexema com aspas, a saída é idêntica ao lexema.
- Tokens sem atributo **não** imprimem `| NULL`, `| NONE` nem `| 0` (DEC-22).

## 16. Formato de saída — LEX-17  (ENUNCIADO ✔PDF no formato; GRUPO na convenção)

Formato do enunciado (REQ-23): `Número da Linha do Átomo# NomeToken | Atributo`.

**Convenção do grupo** ("valor correspondente, se necessário"):

```
<linha># <NOME>                     (token sem atributo)
<linha># <NOME> | <atributo>        (token com atributo)
```

- `#` colado à linha; um espaço antes do nome; ` | ` (espaço, barra, espaço) antes do atributo.
- O sufixo ` | atributo` é **omitido** quando o token não tem atributo significativo. Esta é
  **interpretação do grupo**, baseada em "se necessário"; o enunciado não define o caso sem atributo.
- **Uma linha por token**, na ordem da entrada. A **mesma** listagem vai para a tela e para o arquivo
  (REQ-22). `TOKEN_EOF` **não** é impresso (§27).
- Nome impresso = coluna "Nome impresso" do §3 (`ID`, não `IDENTIFICADOR`; DEC-17).
- Whitespace e comentários não aparecem.

Exemplos:

```
1# ALGORITMO
1# STRING | "PrimeiroPasso"
4# ID | 1
4# ATRIBUICAO
4# NUM_INT | 18
5# OP_REL | GE
```

## 17. Erros léxicos — LEX-13, LEX-18

O enunciado exige (REQ-24): a mensagem `ERRO LÉXICO`, a **linha** e a **sequência incorreta**, e o
**encerramento** do processamento. Sem recuperação.

### 17.1 Situações que são erro léxico

| Situação | Sequência reportada |
|---|---|
| Caractere que não inicia nenhum token (`@ # $ % & ! ? ; ' { } ^ ~ \| _ …`) | o próprio caractere |
| `<` não seguido de `-`, `>` ou `=` | `<` |
| `>` não seguido de `=` | `>` |
| `.` que não forma `..` (`.` isolado, `1.`, `.5`, o `.` sobrando em `1...4`) | `.` |
| Identificador iniciando em `_` | `_` |
| Letra acentuada ou qualquer byte ≥ 0x80 **fora** de string/comentário | o caractere inválido (política exata de bytes: Fase F, AMB-08) |
| Caractere de controle que não é whitespace | o caractere |
| STRING não fechada antes de `\n`, `\r\n` ou EOF | da aspa de abertura até o ponto onde a falta é detectada (exclui o terminador de linha) |

**Princípio da sequência:** a **menor** sequência que identifica o erro de forma útil, **sem** incorporar
lexemas válidos anteriores. Por isso `abc @` reporta `@` (e não `abc @`). Lembrete: nem tudo que parece
estranho é erro léxico; `:=`, por exemplo, é `:` válido seguido de `=` válido, e a rejeição é sintática.

### 17.2 Formato da mensagem (proposto pelo grupo)

```
ERRO LÉXICO - linha <n> - sequência: <sequência>
```

O enunciado só exige os três dados; este formato numa linha é decisão do grupo (fácil de verificar em
teste). Onde a mensagem é impressa, se é também gravada no arquivo de saída e o **código de retorno**
do processo ficam **abertos** (AMB-13, AMB-14). Os tokens já emitidos antes do erro continuam na listagem.

## 18. Exemplos de tokenização

Especificação, **não** saída de programa real. Cada exemplo assume TS vazia e linha 1.

**A)** `algoritmo "PrimeiroPasso"`

```
1# ALGORITMO
1# STRING | "PrimeiroPasso"
```

**B)** `nome, sobrenome: caractere`

```
1# ID | 1
1# VIRGULA
1# ID | 2
1# DOIS_PONTOS
1# CARACTERE
```

**C)** `nomes[1] <- "Ana"`

```
1# ID | 1
1# ABRE_COL
1# NUM_INT | 1
1# FECHA_COL
1# ATRIBUICAO
1# STRING | "Ana"
```

**D)** `para i de 10 ate 0 passo -2 faca`

```
1# PARA
1# ID | 1
1# DE
1# NUM_INT | 10
1# ATE
1# NUM_INT | 0
1# PASSO
1# MENOS
1# NUM_INT | 2
1# FACA
```

**E)** `v MOD 2 = 0`

```
1# ID | 1
1# OP_MULT | MOD
1# NUM_INT | 2
1# OP_REL | EQ
1# NUM_INT | 0
```

**F)** `(idade >= 12) E (altura >= 1.50)`

```
1# ABRE_PAR
1# ID | 1
1# OP_REL | GE
1# NUM_INT | 12
1# FECHA_PAR
1# E
1# ABRE_PAR
1# ID | 2
1# OP_REL | GE
1# NUM_REAL | 1.50
1# FECHA_PAR
```

**G)** `vetor[1..4] de real` — o ponto decisivo é `1..4`:

```
1# VETOR
1# ABRE_COL
1# NUM_INT | 1
1# INTERVALO
1# NUM_INT | 4
1# FECHA_COL
1# DE
1# REAL
```

**H) Casos-limite** (entrada → resultado), cada um vira teste na Fase E:

| Entrada | Resultado |
|---|---|
| `leia(nome)` (sem espaços) | `LEIA` `ABRE_PAR` `ID\|1` `FECHA_PAR` |
| `// só comentário` (sem `\n` final) | nenhum token |
| `escreva("a // b")` | `ESCREVA` `ABRE_PAR` `STRING\|"a // b"` `FECHA_PAR` |
| `Algoritmo` / `ALGORITMO` | `ID` (não é a palavra reservada) |
| `mod` / `e` / `ou` | `ID` (não são operadores) |
| `OU` | `OU` (token próprio; **não** é ID) |
| `a <- 1 // x` | `ID` `ATRIBUICAO` `NUM_INT` (comentário descartado) |
| `x @ y` | `ID\|1`, depois **ERRO LÉXICO - linha 1 - sequência: @** |
| `se a < b entao` | `SE` `ID\|1`, depois **ERRO LÉXICO … sequência: <** |
| `se a > b entao` | `SE` `ID\|1`, depois **ERRO LÉXICO … sequência: >** |
| `1.` | `NUM_INT\|1`, depois **ERRO LÉXICO … sequência: .** |
| `escreva("abc` (sem fechar) | `ESCREVA` `ABRE_PAR`, depois **ERRO LÉXICO … sequência: "abc** |
| `_x` | **ERRO LÉXICO … sequência: _** |
| `Olá` como ID | `ID\|1` (`Ol`), depois **ERRO LÉXICO** no `á` |
| `"Olá, mundo!"` | `STRING\|"Olá, mundo!"` |

## 19. Decisões e ambiguidades remanescentes

Decididas nesta fase (detalhe em `decisoes.md`): AMB-02, AMB-03 (léxico), AMB-04 (parte lexical),
AMB-06 (parte lexical), AMB-11, AMB-12. Continuam abertas:

| Item | Situação |
|---|---|
| AMB-04 | `OU` é token; **aceitação sintática** → Fase C |
| AMB-05 | Estrutura de procedimentos → Fase C (totalmente aberta) |
| AMB-06 | **Subtração binária** (sintaxe) → Fase C |
| AMB-07 | Árvore de derivação → Fase G |
| AMB-08 | Política operacional de bytes/acentos → Fase F |
| AMB-01 | Contradição do material; tratamento técnico já definido (DEC-05) |
| AMB-13 | Código de retorno em execução com erro |
| AMB-14 | Detalhes operacionais da saída léxica (nome do arquivo de saída, destino da mensagem de erro, limites numéricos e de tamanho) |

Pontos de risco anotados para a Fase F (não mudam o contrato): estouro de `NUM_INT` além do tipo inteiro
do C; comprimento máximo de ID/string; codificação do arquivo de entrada.

## 20. Contrato congelado para a Fase C

### 20.1 O que a GLC pode assumir

1. O conjunto de **terminais** é exatamente o do §3 (49 tokens impressos + `TOKEN_EOF` para fim de
   entrada). **A GLC não cria terminais novos.** Se faltar um, é emenda ao contrato, não invenção.
2. `TOKEN_OP_REL` cobre `= <> <= >=`; **não há** `<` nem `>` isolados.
3. `TOKEN_OP_MULT` cobre `* / \ MOD`; o subtipo está no atributo.
4. `TOKEN_MENOS` existe; números **não** têm sinal. Como `-` aparece na gramática (sinal, e se haverá
   subtração binária) é decisão da Fase C.
5. `TOKEN_OU` existe e é distinto de `TOKEN_E`; se a gramática aceita `OU` é decisão da Fase C.
6. `TOKEN_ID` serve para variáveis, vetores, parâmetros, funções e procedimentos; o léxico não os
   distingue.
7. `TOKEN_STRING` é um terminal único; aspas e comentários nunca chegam à gramática.
8. `leia`, `escreva`, `escreval` são **palavras reservadas** (terminais `LEIA`, `ESCREVA`, `ESCREVAL`).
9. `..` é `TOKEN_INTERVALO`, e `vetor[1..4]` tokeniza como no exemplo G.
10. Parênteses de `se`/`enquanto` são decisão sintática já tomada (AMB-10); o léxico só fornece
    `ABRE_PAR`/`FECHA_PAR`.

### 20.2 Índice de regras `LEX-nn` (para testes da Fase E)

| ID | Regra | Seção |
|---|---|---|
| LEX-01 | Tudo é case-sensitive | §2, §6, §7 |
| LEX-02 | ER de ID | §4.1 |
| LEX-03 | Reservada tem prioridade sobre ID; catálogo alfabético | §4.1, §6 |
| LEX-04 | NUM_INT sem sinal | §4.2 |
| LEX-05 | NUM_REAL `[0-9]+\.[0-9]+` | §4.3 |
| LEX-06 | Regra do ponto (`1..4`) | §4.4 |
| LEX-07 | STRING, sem escape, opaca | §4.5 |
| LEX-08 | Comentário `//` descartado | §9 |
| LEX-09 | Whitespace e contagem de linhas (CRLF = 1) | §10 |
| LEX-10 | Maximal munch; catálogo simbólico | §12 |
| LEX-11 | `<` e `>` isolados não são tokens (erro léxico); só `<-` `<>` `<=` `>=` | §12, §17 |
| LEX-12 | `/` é operador; `//` inicia comentário | §9, §11 |
| LEX-13 | Caracteres inválidos → erro léxico | §17 |
| LEX-14 | `OU` token próprio; `MOD`/`E` exatos | §7.1 |
| LEX-15 | Tabela de símbolos só de IDs | §13 |
| LEX-16 | Atributos por token | §14, §15 |
| LEX-17 | Formato de saída | §16 |
| LEX-18 | Formato e princípio da mensagem de erro | §17 |

### 20.3 VOCABULÁRIO LÉXICO CONGELADO — FASE B

**A. Tokens internos de controle (1)**
`TOKEN_EOF` — participa da interface scanner/parser; **não** é lexema do programa e **não** é impresso
(DEC-23).

**B. Tokens de classe (4)**
`ID`, `NUM_INT`, `NUM_REAL`, `STRING`

**C. Palavras reservadas (31)**
`algoritmo` `var` `inicio` `fimalgoritmo` · `inteiro` `real` `caractere` `logico` ·
`verdadeiro` `falso` · `leia` `escreva` `escreval` · `se` `entao` `senao` `fimse` ·
`para` `de` `ate` `passo` `faca` `fimpara` · `enquanto` `fimenquanto` · `vetor` ·
`procedimento` `fimprocedimento` · `funcao` `fimfuncao` `retorne`

**D. Operadores-palavra (3 lexemas)**
`MOD` (→ `OP_MULT`/`MOD`) · `E` (→ `E`) · `OU` (→ `OU`; token reservado próprio, aceitação sintática na Fase C)

**E. Operadores simbólicos (10 lexemas, 5 tokens)**
`<-` (`ATRIBUICAO`) · `+` (`MAIS`) · `-` (`MENOS`) ·
`*` `/` `\` (`OP_MULT`: `MUL` `DIV_REAL` `DIV_INT`) ·
`=` `<>` `<=` `>=` (`OP_REL`: `EQ` `NE` `LE` `GE`)

**F. Pontuação (7)**
`(` `)` `[` `]` `:` `,` `..`

**G. Elementos consumidos sem gerar token**
- whitespace (espaço, tab, LF, CR/CRLF)
- comentário `// …` até fim de linha ou EOF
- (as aspas de abertura e fechamento de STRING são consumidas como parte do token STRING)

**H. Sequências explicitamente NÃO suportadas (→ erro léxico, ou sem token)**
- `<` isolado · `>` isolado · `.` isolado
- `/* … */` (vira `/` `*` … sem significado lexical especial)
- strings multilinha e strings sem fechamento
- escapes em string (`\"`, `\n`, `\t`, `\\` não são especiais)
- identificador iniciando por `_`
- identificador acentuado
- reais `.5` e `1.` (e `1,5`)
- operadores sem evidência: `^`, `NAO` (este último, se escrito, é um **ID** por não ser reservado),
  `:=`, `==`, `!=`, `%`, `;`
- maiúsculas/minúsculas alternativas de reservadas e operadores-palavra (`ALGORITMO`, `mod`, `e`, `ou`
  viram **ID**, não erro léxico)

*Observação:* `OU` **não** está na lista H. Ele é reservado e tokenizado (`TOKEN_OU`); só a sua aceitação
sintática fica pendente para a Fase C.

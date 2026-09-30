# Especificação do MiniVisualg — inventário de evidências (rascunho)

> **Status: inventário, não gramática.** Este documento lista o que o Anexo I
> *evidencia*, o que só é *mencionado* e o que *não tem evidência*. Ele alimenta as
> Fases B (especificação lexical) e C (GLC), que ainda não foram autorizadas.
> Nenhuma ER ou regra de produção aqui é definitiva.
>
> **Fonte e conferência.** Nasceu do Contexto Mestre (Fase A) e foi reconciliado com o **Anexo I do PDF
> oficial** na Fase A.1, conferido externamente pelo grupo. Onde a linha cita uma seção do Anexo I
> (Variáveis, Operadores, Controle, Repetição, Vetores, Procedimentos, Funções), a evidência veio
> dessa seção. Linhas sem seção citada continuam vindo do Contexto Mestre.
> Regra do enunciado: só o que está nos exemplos existe. Nada vem do Visualg "de verdade".

## Níveis de evidência

| Nível | Significado |
|---|---|
| **CONFIRMADO** | Aparece em uso executável num exemplo do Anexo I. |
| **MENCIONADO / DEFINIDO TEXTUALMENTE** | Citado em texto ou comentário do Anexo I, sem uso executável. Não é esquecido: a entrada na gramática é decisão consciente (AMB-04). |
| **SEM EVIDÊNCIA** | Não aparece. Não entra no subconjunto sem decisão registrada. |
| **AMBÍGUO** | O material é visualmente inconsistente; ver a AMB indicada. |

## 1. Estrutura geral do programa

| Elemento | Evidência | Nível |
|---|---|---|
| `algoritmo "nome"` ... `fimalgoritmo` | Todos os exemplos | CONFIRMADO |
| Seção `var` seguida de declarações | `nome: caractere`, `idade: inteiro` | CONFIRMADO |
| `var` **vazia** | Exemplo `PrimeiroPasso` (`var` seguido de `inicio`) | CONFIRMADO |
| `inicio` abrindo o bloco principal | Todos | CONFIRMADO |
| Comentário `// ...` até fim de linha/EOF | Ex.: `// A área de variáveis está vazia` | CONFIRMADO |
| Comentário `/* ... */` | Não aparece no Anexo I (só em exemplos gerais de C da disciplina) | SEM EVIDÊNCIA |
| Seção `var` **ausente** | Exemplo de procedimentos (ver AMB-05) | AMBÍGUO |

## 2. Declarações e tipos  (ANEXO I — Variáveis / Vetores)

| Elemento | Exemplo | Nível |
|---|---|---|
| `inteiro` | `idade: inteiro` | CONFIRMADO |
| `real` | `altura: real`; `notas: vetor[1..4] de real` | CONFIRMADO |
| `caractere` | `nome: caractere` | CONFIRMADO |
| `logico` | `portaAberta: logico` | CONFIRMADO |
| **Vários identificadores na mesma declaração** | `nome, sobrenome: caractere` (exemplo `CadastroSimples`) | **CONFIRMADO** |
| Vetor | `nomes: vetor[1..3] de caractere`, `notas: vetor[1..4] de real` | CONFIRMADO |
| Limites de vetor negativos ou não literais | — | SEM EVIDÊNCIA |

**Consequência para a Fase C (sem escrever produção ainda):** a declaração precisa comportar
*um ou mais identificadores separados por vírgula antes de `:`*, seguidos de tipo simples ou de vetor.

## 3. Comandos

| Comando | Exemplo | Nível |
|---|---|---|
| Atribuição `<-` | `nome <- "Florêncio"`, `contador <- contador + 1`, `resultado <- somar(10, 5)` | CONFIRMADO |
| **Atribuição a elemento de vetor** | `nomes[1] <- "Ana"`, `nomes[2] <- "Carlos"`, `nomes[3] <- "João"` (ANEXO I — Vetores) | **CONFIRMADO** |
| `leia(x)` | `leia(nome)` | CONFIRMADO |
| `leia` com elemento de vetor | `leia(notas[i])` (ANEXO I — Vetores) | CONFIRMADO |
| `escreva(...)` | `escreva("Bem-vindo ao ")`, `escreva("Digite seu nome: ")` | CONFIRMADO |
| `escreval(...)` | `escreval("Muito prazer, ", nome, "!")` | CONFIRMADO |
| Lista de argumentos separada por `,` em `escreva/escreval` | `escreva("Olá, ", nome, ". Você tem ", idade, " anos.")` | CONFIRMADO |
| `se ( cond ) entao ... fimse` | `se (chovendo = verdadeiro) entao` (ANEXO I — Controle) | CONFIRMADO |
| `se ... entao ... senao ... fimse` | exemplo de idade | CONFIRMADO |
| `se` aninhado dentro de `senao` | exemplo de idade (`>= 18` / `>= 12`) | CONFIRMADO |
| `para v de a ate b faca ... fimpara` | `para i de 1 ate 5 faca` (ANEXO I — Repetição) | CONFIRMADO |
| `para` com `passo` | `para i de 10 ate 0 passo -2 faca` (`passo` é opcional) | CONFIRMADO |
| `enquanto ( cond ) faca ... fimenquanto` | `enquanto (contador <= 5) faca` (ANEXO I — Repetição) | CONFIRMADO |
| `retorne expr` | `retorne a + b`, `retorne verdadeiro`, `retorne falso` (ANEXO I — Funções) | CONFIRMADO |
| Chamada de procedimento sem parâmetros | `linha_decorativa` (sem parênteses) (ANEXO I — Procedimentos) | CONFIRMADO |
| Chamada de procedimento com parâmetros | `mostrar_erro("...")` | CONFIRMADO |
| `se`/`enquanto` **sem** parênteses em torno da condição | Nenhum exemplo | SEM EVIDÊNCIA (ver AMB-10) |
| `repita ... ate`, `escolha ... caso` | — | SEM EVIDÊNCIA |

### Formas de referência a variável

O Anexo I exige, pelo menos, duas formas de referência a variável, em dois papéis:

| Forma | Como alvo de atribuição / `leia` | Como operando em expressão |
|---|---|---|
| Simples: `ID` | `nome <- ...`, `leia(nome)` | `contador + 1` |
| Indexada: `ID [ expressão ]` | `nomes[1] <- "Ana"`, `leia(notas[i])` | `notas[i]`, `nomes[2]` |

Isto **reforça a AMB-09**: atribuição simples, atribuição indexada e chamada de procedimento
começam todas com `ID`, e a gramática precisará fatorá-las.

## 4. Sub-rotinas  (ANEXO I — Procedimentos / Funções)

| Elemento | Evidência | Nível |
|---|---|---|
| `funcao nome(p: tipo, q: tipo): tipoRetorno` ... `inicio` ... `fimfuncao` | `funcao somar(a: inteiro, b: inteiro): inteiro` | CONFIRMADO |
| Chamada de função dentro de expressão | `somar(10, 5)`, `eh_par(num)` | CONFIRMADO |
| `procedimento nome` (sem parâmetros, **sem** parênteses) ... `inicio` ... `fimprocedimento` | `linha_decorativa` | CONFIRMADO |
| `procedimento nome(p: tipo)` ... `fimprocedimento` | `mostrar_erro(mensagem: caractere)` | CONFIRMADO |
| Posição das sub-rotinas em relação a `var` e ao `inicio` principal | Disposição visual do PDF é inconsistente (confirmado na A.1) | **AMBÍGUO** (AMB-05) |
| Variáveis locais em sub-rotinas | — | SEM EVIDÊNCIA |

## 5. Expressões  (ANEXO I — Operadores)

### Operadores

| Operador | Exemplo | Nível |
|---|---|---|
| `+` | `n1 + n2`, `contador + 1` | CONFIRMADO |
| `*` | `n1 * n2` | CONFIRMADO |
| `/` | `n1 / n2`, `soma / 4` | CONFIRMADO |
| `\` | `n1 \ n2` | CONFIRMADO |
| `MOD` | `v MOD 2 = 0` | CONFIRMADO |
| `-` — presença lexical | `passo -2` | CONFIRMADO |
| Sinal negativo / menos **unário** | `passo -2` | **CONFIRMADO** (pelo uso em `-2`) |
| Subtração **binária** (`n1 - n2`) | não aparece em nenhum exemplo | **SEM EVIDÊNCIA** (AMB-06) |
| `=` | `senhaDigitada = 1234` | CONFIRMADO |
| `<>` | `nome <> "João"` | CONFIRMADO |
| `>=` | `idade >= 18` | CONFIRMADO |
| `<=` | `contador <= 5` | CONFIRMADO |
| `<` isolado | — | SEM EVIDÊNCIA (AMB-03) |
| `>` isolado | — | SEM EVIDÊNCIA (AMB-03) |
| `E` | `podeBrincar <- (idade >= 12) E (altura >= 1.50)` | CONFIRMADO POR USO |
| `OU` | Comentários do Anexo I: "O E exige que os DOIS lados sejam verdadeiros" / "O OU, basta um ser verdadeiro". Nenhuma expressão executável usa `OU`. | **MENCIONADO / DEFINIDO TEXTUALMENTE** (AMB-04) |
| `NAO` (negação) | — | SEM EVIDÊNCIA |
| `^` / potência | — | SEM EVIDÊNCIA |

**Direção arquitetural para o `-` (GRUPO, a formalizar nas Fases B/C):** o scanner produz `MENOS` e
`NUM_INT(2)` separadamente; a forma negativa/unária é tratada na **gramática** quando necessário.
Isto é sintaxe, não análise semântica. Subtração binária **não** é adicionada por simetria com `+`.

### Operandos

| Elemento | Exemplo | Nível |
|---|---|---|
| Inteiro | `0`, `5`, `18`, `1234` | CONFIRMADO |
| Real | `1.60`, `1.50` | CONFIRMADO |
| String entre aspas duplas | `"Olá, mundo!"`, `"João"` | CONFIRMADO |
| `verdadeiro` | `portaAberta <- verdadeiro`, `chovendo <- verdadeiro`, `retorne verdadeiro` | CONFIRMADO |
| `falso` | `retorne falso` | CONFIRMADO |
| Variável simples | `nome`, `contador` | CONFIRMADO |
| Acesso a vetor | `nomes[2]`, `notas[i]` | CONFIRMADO |
| Chamada de função | `eh_par(num)` | CONFIRMADO |
| Parênteses | `(idade >= 12) E (altura >= 1.50)` | CONFIRMADO |

### Formas-teste para o desenho da gramática

Toda gramática proposta na Fase C deve derivar cada linha abaixo
(ou justificar, via AMB, por que não deriva).

Expressões:

```
n1 + n2
n1 * n2
n1 / n2
n1 \ n2
idade >= 18
contador <= 5
senhaDigitada = 1234
nome <> "João"
(idade >= 12) E (altura >= 1.50)
contador + 1
soma / 4
v MOD 2 = 0
somar(10, 5)
eh_par(num)
notas[i]
nomes[2]
passo -2            (sinal negativo em contexto de `para`)
```

Declarações e comandos iniciados por identificador (reforçam AMB-09):

```
nome, sobrenome: caractere
nomes: vetor[1..3] de caractere
nomes[1] <- "Ana"
leia(notas[i])
linha_decorativa
mostrar_erro("...")
```

## 6. Elementos léxicos (candidatos — não definitivos)

### Palavras reservadas com evidência

`algoritmo`, `var`, `inicio`, `fimalgoritmo`,
`inteiro`, `real`, `caractere`, `logico`, `vetor`, `de`,
`leia`, `escreva`, `escreval`,
`se`, `entao`, `senao`, `fimse`,
`para`, `ate`, `passo`, `faca`, `fimpara`,
`enquanto`, `fimenquanto`,
`funcao`, `fimfuncao`, `retorne`,
`procedimento`, `fimprocedimento`,
`verdadeiro`, `falso`,
`MOD`, `E`,
`OU` (**somente mencionado/definido textualmente**, ver AMB-04).

> Observação: `de` e `ate` também casariam com a forma de identificador. Palavra
> reservada tem prioridade sobre ID. Se `leia/escreva/escreval` são reservadas ou nomes
> predefinidos é decisão da Fase B.

### Tokens de classe (ER candidatas, a validar na Fase B)

| Classe | ER candidata | Observação |
|---|---|---|
| ID | `[A-Za-z][A-Za-z0-9_]*` | Conferida contra: `nome`, `sobrenome`, `portaAberta`, `linha_decorativa`, `eh_par`, `n1`, `n2`, `v`, `i`. Identificadores usados como evidência são ASCII. Não há exemplo com `_` inicial. |
| NUM_INT | `[0-9]+` | O sinal `-` é tratado à parte (AMB-06). |
| NUM_REAL | `[0-9]+\.[0-9]+` | Exige dígito após o ponto — necessário para não confundir `1..4` com `1.` seguido de `.4`. |
| STRING | `"` … `"` sem quebra de linha | Conteúdo textual **opaco** entre aspas (inclui acentos). Sem evidência de escapes nem multilinha. Aspas fazem parte do reconhecimento; não geram token próprio. |

### Símbolos

`(`  `)`  `[`  `]`  `:`  `,`  `..`  `<-`  `=`  `<>`  `>=`  `<=`  `+`  `*`  `/`  `\`  `-`

`//` inicia comentário e **não** é enviado ao parser. Aspas delimitam STRING e **não**
geram token separado.

## 7. Fora do escopo (explícito)

Análise semântica (tipos, escopos, declaração prévia), geração de código, otimização,
recuperação de erros, e qualquer construção do Visualg sem evidência no Anexo I.

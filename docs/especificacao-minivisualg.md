# Especificação do MiniVisualg — inventário de evidências (rascunho)

> **Status: inventário, não gramática.** Este documento lista o que o Anexo I
> *evidencia*, o que só é *mencionado* e o que *não tem evidência*. Ele alimenta as
> Fases B (especificação lexical) e C (GLC), que ainda não foram autorizadas.
> Nenhuma ER ou regra de produção aqui é definitiva.
>
> Fonte: Contexto Mestre (não o PDF). Conferir contra o Anexo I original (plano, Fase A).
> Regra do enunciado: só o que está nos exemplos existe. Nada vem do Visualg "de verdade".

## Níveis de evidência

| Nível | Significado |
|---|---|
| **CONFIRMADO** | Aparece em uso executável num exemplo do Anexo I. |
| **MENCIONADO** | Citado em texto/comentário do material, sem uso executável. |
| **SEM EVIDÊNCIA** | Não aparece. Não entra no subconjunto sem decisão registrada. |

## 1. Estrutura geral do programa

| Elemento | Evidência | Nível |
|---|---|---|
| `algoritmo "nome"` ... `fimalgoritmo` | Todos os exemplos | CONFIRMADO |
| Seção `var` seguida de declarações | `nome: caractere`, `idade: inteiro` | CONFIRMADO |
| `var` **vazia** | Exemplo `PrimeiroPasso` (`var` seguido de `inicio`) | CONFIRMADO |
| `inicio` abrindo o bloco principal | Todos | CONFIRMADO |
| Comentário `// ...` até fim de linha/EOF | Ex.: `// A área de variáveis está vazia` | CONFIRMADO |
| Comentário `/* ... */` | Só em exemplos gerais de C da disciplina | SEM EVIDÊNCIA |
| Seção `var` **ausente** | Exemplo de procedimentos (ver AMB-05) | AMBÍGUO |

## 2. Declarações e tipos

| Elemento | Exemplo | Nível |
|---|---|---|
| `inteiro` | `idade: inteiro` | CONFIRMADO |
| `real` | `altura: real` | CONFIRMADO |
| `caractere` | `nome: caractere` | CONFIRMADO |
| `logico` | `portaAberta: logico` | CONFIRMADO |
| Vetor | `nomes: vetor[1..3] de caractere`, `notas: vetor[1..4] de real` | CONFIRMADO |
| Múltiplos nomes por declaração (`a, b: inteiro`) | — | SEM EVIDÊNCIA |
| Limites de vetor negativos ou não literais | — | SEM EVIDÊNCIA |

## 3. Comandos

| Comando | Exemplo | Nível |
|---|---|---|
| Atribuição `<-` | `nome <- "Florêncio"`, `contador <- contador + 1`, `resultado <- somar(10, 5)` | CONFIRMADO |
| Atribuição a elemento de vetor | `nomes[1] <- ...` (acesso `nomes[1]`, `notas[i]` é confirmado; atribuição a posição deve ser conferida no PDF) | PARCIAL — conferir |
| `leia(x)` | `leia(nome)` | CONFIRMADO |
| `escreva(...)` | `escreva("Digite seu nome: ")` | CONFIRMADO |
| `escreval(...)` | `escreval("Muito prazer, ", nome, "!")` | CONFIRMADO |
| Lista de argumentos separada por `,` em `escreva/escreval` | `escreva("Olá, ", nome, ". Você tem ", idade, " anos.")` | CONFIRMADO |
| `se (cond) entao ... fimse` | `se (chovendo = verdadeiro) entao` | CONFIRMADO |
| `se ... entao ... senao ... fimse` | exemplo de idade | CONFIRMADO |
| `se` aninhado dentro de `senao` | exemplo de idade (`>= 18` / `>= 12`) | CONFIRMADO |
| `para v de a ate b faca ... fimpara` | `para i de 1 ate 5 faca` | CONFIRMADO |
| `para` com `passo` | `para i de 10 ate 0 passo -2 faca` (passo é opcional) | CONFIRMADO |
| `enquanto (cond) faca ... fimenquanto` | `enquanto (contador <= 5) faca` | CONFIRMADO |
| `retorne expr` | `retorne a + b`, `retorne verdadeiro` | CONFIRMADO |
| Chamada de procedimento sem parâmetros | `linha_decorativa` (sem parênteses) | CONFIRMADO |
| Chamada de procedimento com parâmetros | `mostrar_erro("...")` | CONFIRMADO |
| `repita ... ate`, `escolha ... caso` | — | SEM EVIDÊNCIA |

## 4. Sub-rotinas

| Elemento | Evidência | Nível |
|---|---|---|
| `funcao nome(p: tipo, q: tipo): tipoRetorno` ... `inicio` ... `fimfuncao` | `funcao somar(a: inteiro, b: inteiro): inteiro` | CONFIRMADO |
| Chamada de função dentro de expressão | `somar(10, 5)`, `eh_par(num)` | CONFIRMADO |
| `procedimento nome` (sem parâmetros, **sem** parênteses) ... `inicio` ... `fimprocedimento` | `linha_decorativa` | CONFIRMADO |
| `procedimento nome(p: tipo)` ... `fimprocedimento` | `mostrar_erro(mensagem: caractere)` | CONFIRMADO |
| Posição das sub-rotinas em relação a `var` e ao `inicio` principal | Exemplos inconsistentes | **AMBÍGUO** (AMB-05) |
| Variáveis locais em sub-rotinas | — | SEM EVIDÊNCIA |

## 5. Expressões

### Operadores

| Operador | Exemplo | Nível |
|---|---|---|
| `+` | `n1 + n2`, `contador + 1` | CONFIRMADO |
| `*` | `n1 * n2` | CONFIRMADO |
| `/` | `n1 / n2`, `soma / 4` | CONFIRMADO |
| `\` | `n1 \ n2` | CONFIRMADO |
| `MOD` | `v MOD 2 = 0` | CONFIRMADO |
| `-` como **sinal** | `passo -2` | CONFIRMADO |
| `-` **binário** | não há `n1 - n2` | **SEM EVIDÊNCIA** (AMB-06) |
| `=` | `senhaDigitada = 1234` | CONFIRMADO |
| `<>` | `nome <> "João"` | CONFIRMADO |
| `>=` | `idade >= 18` | CONFIRMADO |
| `<=` | `contador <= 5` | CONFIRMADO |
| `<` isolado | — | **SEM EVIDÊNCIA** (AMB-03) |
| `>` isolado | — | **SEM EVIDÊNCIA** (AMB-03) |
| `E` | `(idade >= 12) E (altura >= 1.50)` | CONFIRMADO |
| `OU` | Comentário do material: "O OU, basta um ser verdadeiro" | **MENCIONADO** (AMB-04) |
| `NAO` (negação) | — | SEM EVIDÊNCIA |
| `^` / potência | — | SEM EVIDÊNCIA |

### Operandos

| Elemento | Exemplo | Nível |
|---|---|---|
| Inteiro | `0`, `5`, `18`, `1234` | CONFIRMADO |
| Real | `1.60`, `1.50` | CONFIRMADO |
| String entre aspas duplas | `"Olá, mundo!"`, `"João"` | CONFIRMADO |
| `verdadeiro`, `falso` | `portaAberta <- verdadeiro`, `retorne falso` | CONFIRMADO |
| Variável | `nome`, `contador` | CONFIRMADO |
| Acesso a vetor | `nomes[1]`, `notas[i]` | CONFIRMADO |
| Chamada de função | `eh_par(num)` | CONFIRMADO |
| Parênteses | `(idade >= 12) E (altura >= 1.50)` | CONFIRMADO |

### Expressões-teste para o desenho da gramática

Toda gramática proposta na Fase C deve derivar cada linha abaixo
(ou justificar, via AMB, por que não deriva):

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
passo -2            (sinal negativo em contexto de `para`)
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
`OU` (**somente mencionado**, ver AMB-04).

> Observação: `de` e `ate` também casariam com a forma de identificador. Palavra
> reservada tem prioridade sobre ID (Contexto Mestre, §37). Se `leia/escreva/escreval`
> são reservadas ou nomes predefinidos é decisão da Fase B.

### Tokens de classe (ER candidatas, a validar na Fase B)

| Classe | ER candidata | Observação |
|---|---|---|
| ID | `[A-Za-z][A-Za-z0-9_]*` | Conferida contra: `nome`, `portaAberta`, `linha_decorativa`, `eh_par`, `n1`, `n2`, `v`, `i`. Não há exemplo com `_` inicial, nem com acento em identificador. |
| NUM_INT | `[0-9]+` | O sinal `-` é tratado à parte (AMB-06). |
| NUM_REAL | `[0-9]+\.[0-9]+` | Exige dígito após o ponto — necessário para não confundir `1..4` com `1.` seguido de `.4`. |
| STRING | `"` … `"` sem quebra de linha | Sem evidência de escapes nem multilinha. Aspas fazem parte do reconhecimento; não geram token próprio. |

### Símbolos

`(`  `)`  `[`  `]`  `:`  `,`  `..`  `<-`  `=`  `<>`  `>=`  `<=`  `+`  `*`  `/`  `\`  `-`

`//` inicia comentário e **não** é enviado ao parser. Aspas delimitam STRING e **não**
geram token separado.

## 7. Fora do escopo (explícito)

Análise semântica (tipos, escopos, declaração prévia), geração de código, otimização,
recuperação de erros, e qualquer construção do Visualg sem evidência no Anexo I.

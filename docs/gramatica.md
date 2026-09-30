# Gramática Livre de Contexto do MiniVisualg — Fase C

> **Status: GLC CONGELADA (Fase C).** Gramática formal em BNF, sem recursão à esquerda e fatorada, pronta
> para a verificação formal FIRST/FOLLOW/LL(1) da **Fase D**. Nenhum parser, lexer ou código C existe.
>
> **Fontes:** Anexo I do PDF oficial, requisitos reconciliados (`requisitos.md`), contrato léxico
> congelado (`especificacao-lexica.md`) e a metodologia de análise descendente das aulas.
> **Nada vem do Visualg completo** nem de gramática externa.
>
> **Origem.** Toda decisão desta fase é `GRUPO` (DEC-28 a DEC-46 em `decisoes.md`). Nenhuma é atribuída
> à professora.

## 1. Objetivo

Definir exatamente quais sequências de tokens formam um programa MiniVisualg sintaticamente válido, de
forma que:

- toda construção do Anexo I seja derivável;
- nada sem evidência seja derivável sem uma decisão registrada;
- a gramática seja adequada a um parser **descendente recursivo preditivo LL(1)** (DEC-03): sem recursão
  à esquerda, com prefixos comuns fatorados, uma futura função por não-terminal.

## 2. Fontes e limites

| Fonte | Papel |
|---|---|
| Anexo I (ENUNCIADO) | **única** fonte das construções da linguagem (REQ-04) |
| `especificacao-lexica.md` | **única** fonte dos terminais (§3 de lá; nada é inventado aqui) |
| Aulas (análise descendente, LL(1)) | forma da gramática: sem recursão à esquerda, fatorada |
| `decisoes.md` | decisões do grupo onde o Anexo I é lacunar |

Limites desta fase:

- **Não** há FIRST/FOLLOW nem tabela preditiva (Fase D). A §17 só **prepara** essa análise.
- **Não** há análise semântica (§16).
- Whitespace, quebras de linha e comentários **não** aparecem: já foram consumidos pelo léxico.
  **Quebra de linha não separa comandos**; comandos são reconhecidos só pela estrutura dos tokens.
- `TOKEN_EOF` **não** é terminal da GLC: não é lexema do programa. A gramática termina em
  `FIMALGORITMO`. Verificar que nada sobra depois (consumir `TOKEN_EOF`) é detalhe do parser (DEC-46).

## 3. Terminais

Os terminais são **exatamente** os 49 nomes impressos do contrato léxico (`especificacao-lexica.md` §3).
Todos são usados pela gramática.

| Grupo | Terminais |
|---|---|
| Classe | `ID` `NUM_INT` `NUM_REAL` `STRING` |
| Estrutura | `ALGORITMO` `VAR` `INICIO` `FIMALGORITMO` |
| Tipos | `INTEIRO` `REAL` `CARACTERE` `LOGICO` `VETOR` `DE` |
| Valores lógicos | `VERDADEIRO` `FALSO` |
| E/S | `LEIA` `ESCREVA` `ESCREVAL` |
| Condicional | `SE` `ENTAO` `SENAO` `FIMSE` |
| Repetição | `PARA` `ATE` `PASSO` `FACA` `FIMPARA` `ENQUANTO` `FIMENQUANTO` |
| Sub-rotinas | `PROCEDIMENTO` `FIMPROCEDIMENTO` `FUNCAO` `FIMFUNCAO` `RETORNE` |
| Operadores | `OP_REL` `OP_MULT` `MAIS` `MENOS` `ATRIBUICAO` `E` `OU` |
| Pontuação | `ABRE_PAR` `FECHA_PAR` `ABRE_COL` `FECHA_COL` `DOIS_PONTOS` `VIRGULA` `INTERVALO` |

Regras:

- **Atributos não são terminais.** `EQ` `NE` `LE` `GE` são atributos de `OP_REL`; `MUL` `DIV_REAL`
  `DIV_INT` `MOD` são atributos de `OP_MULT`. **Nenhuma produção depende do atributo**: o parser consome a
  classe.
- **Não existem** `<`, `>`, `LT`, `GT`, `NAO`, subtração binária ou `EOF` na gramática.
- `MENOS` aparece em **uma única** produção (P62, número do `passo`).

## 4. Não-terminais

50 não-terminais; símbolo inicial `<programa>`. Os nomes foram escolhidos para virar, na Fase G, uma
função do parser cada.

| Grupo | Não-terminais |
|---|---|
| Programa | `<programa>` `<corpo_programa>` `<secao_var>` `<secao_var_opcional>` `<bloco>` |
| Declarações | `<lista_declaracoes>` `<declaracao>` `<lista_ids>` `<lista_ids_cauda>` `<tipo_decl>` `<tipo_simples>` `<tipo_vetor>` |
| Sub-rotinas | `<lista_rotinas>` `<rotina>` `<procedimento>` `<parametros_procedimento>` `<funcao>` `<lista_parametros>` `<lista_parametros_cauda>` `<parametro>` |
| Comandos | `<lista_comandos>` `<lista_comandos_cauda>` `<comando>` `<cmd_id>` `<cauda_comando_id>` `<cmd_leia>` `<referencia>` `<indice_opcional>` `<cmd_escreva>` `<cmd_escreval>` `<cmd_se>` `<senao_opcional>` `<cmd_para>` `<passo_opcional>` `<numero_passo>` `<cmd_enquanto>` `<cmd_retorne>` |
| Argumentos | `<lista_argumentos>` `<lista_argumentos_cauda>` |
| Expressões | `<expressao>` `<expr_logica>` `<expr_logica_cauda>` `<expr_relacional>` `<expr_rel_cauda>` `<expr_aditiva>` `<expr_aditiva_cauda>` `<expr_mult>` `<expr_mult_cauda>` `<primario>` `<cauda_primario>` |

Convenção de nomes: `…_cauda` = resto de uma lista/expressão após eliminar recursão à esquerda;
`…_opcional` = parte que pode ser vazia; `cmd_…` = um comando.

## 5. Decisões sintáticas da Fase C

Resumo; o registro completo (problema, alternativas, justificativa, impacto) está em `decisoes.md`.

| DEC | Decisão | Resolve |
|---|---|---|
| DEC-28 | Sub-rotinas ficam depois de `ALGORITMO STRING` e antes de `VAR`/`INICIO` principal | AMB-05 |
| DEC-29 | `VAR` obrigatória quando não há sub-rotina; opcional quando há; lista de declarações pode ser vazia | AMB-05 |
| DEC-30 | Zero ou mais sub-rotinas, procedimentos e funções em qualquer ordem | — |
| DEC-31 | `OU` aceito sintaticamente | AMB-04 |
| DEC-32 | `E` e `OU` no mesmo nível de precedência | — |
| DEC-33 | Sem subtração binária; `MENOS` fora da gramática de expressões | AMB-06 |
| DEC-34 | `passo` só aceita `NUM_INT` ou `MENOS NUM_INT` | AMB-06 |
| DEC-35 | Hierarquia: lógico < relacional (não encadeável) < aditivo < multiplicativo < primário | — |
| DEC-36 | Comando iniciado por `ID` fatorado em `ID <cauda_comando_id>` | AMB-09 |
| DEC-37 | Primário iniciado por `ID` fatorado em `ID <cauda_primario>` | AMB-09 |
| DEC-38 | `RETORNE` aceito onde qualquer comando é aceito (limitação contextual assumida) | — |
| DEC-39 | Listas de comandos não vazias | — |
| DEC-40 | Função exige ≥ 1 parâmetro; tipo de retorno é `<tipo_simples>` | — |
| DEC-41 | Procedimento sem parâmetros não tem parênteses, nem na declaração nem na chamada | — |
| DEC-42 | Limites de vetor: só `NUM_INT` | — |
| DEC-43 | Limites de `para` (`de`/`ate`) são `<expressao>` (alternativa restrita documentada) | — |
| DEC-44 | `leia` com um único alvo; `escreva`/`escreval`/chamadas com ≥ 1 argumento | — |
| DEC-45 | Parâmetro = `ID DOIS_PONTOS <tipo_simples>` (sem vetor, sem referência) | — |
| DEC-46 | `TOKEN_EOF` fora da GLC; a gramática termina em `FIMALGORITMO` | — |

## 6. GLC formal

Notação BNF: `<nao_terminal>`, `TERMINAL`, `ε` para vazio, `|` para alternativa. Cada alternativa tem um
número `Pnn`, usado nas derivações (§14) e na Fase D. **Esta lista é a gramática oficial.**

```
-- Programa --------------------------------------------------------------------
P01  <programa>                -> ALGORITMO STRING <corpo_programa> FIMALGORITMO
P02  <corpo_programa>          -> <secao_var> <bloco>
P03                             | <rotina> <lista_rotinas> <secao_var_opcional> <bloco>
P04  <secao_var>               -> VAR <lista_declaracoes>
P05  <secao_var_opcional>      -> <secao_var>
P06                             | ε
P07  <bloco>                   -> INICIO <lista_comandos>

-- Declarações -----------------------------------------------------------------
P08  <lista_declaracoes>       -> <declaracao> <lista_declaracoes>
P09                             | ε
P10  <declaracao>              -> <lista_ids> DOIS_PONTOS <tipo_decl>
P11  <lista_ids>               -> ID <lista_ids_cauda>
P12  <lista_ids_cauda>         -> VIRGULA ID <lista_ids_cauda>
P13                             | ε
P14  <tipo_decl>               -> <tipo_simples>
P15                             | <tipo_vetor>
P16  <tipo_vetor>              -> VETOR ABRE_COL NUM_INT INTERVALO NUM_INT FECHA_COL DE <tipo_simples>
P17  <tipo_simples>            -> INTEIRO
P18                             | REAL
P19                             | CARACTERE
P20                             | LOGICO

-- Sub-rotinas -----------------------------------------------------------------
P21  <lista_rotinas>           -> <rotina> <lista_rotinas>
P22                             | ε
P23  <rotina>                  -> <procedimento>
P24                             | <funcao>
P25  <procedimento>            -> PROCEDIMENTO ID <parametros_procedimento> <bloco> FIMPROCEDIMENTO
P26  <parametros_procedimento> -> ABRE_PAR <lista_parametros> FECHA_PAR
P27                             | ε
P28  <funcao>                  -> FUNCAO ID ABRE_PAR <lista_parametros> FECHA_PAR DOIS_PONTOS <tipo_simples> <bloco> FIMFUNCAO
P29  <lista_parametros>        -> <parametro> <lista_parametros_cauda>
P30  <lista_parametros_cauda>  -> VIRGULA <parametro> <lista_parametros_cauda>
P31                             | ε
P32  <parametro>               -> ID DOIS_PONTOS <tipo_simples>

-- Comandos --------------------------------------------------------------------
P33  <lista_comandos>          -> <comando> <lista_comandos_cauda>
P34  <lista_comandos_cauda>    -> <comando> <lista_comandos_cauda>
P35                             | ε
P36  <comando>                 -> <cmd_id>
P37                             | <cmd_leia>
P38                             | <cmd_escreva>
P39                             | <cmd_escreval>
P40                             | <cmd_se>
P41                             | <cmd_para>
P42                             | <cmd_enquanto>
P43                             | <cmd_retorne>
P44  <cmd_id>                  -> ID <cauda_comando_id>
P45  <cauda_comando_id>        -> ATRIBUICAO <expressao>
P46                             | ABRE_COL <expressao> FECHA_COL ATRIBUICAO <expressao>
P47                             | ABRE_PAR <lista_argumentos> FECHA_PAR
P48                             | ε
P49  <cmd_leia>                -> LEIA ABRE_PAR <referencia> FECHA_PAR
P50  <referencia>              -> ID <indice_opcional>
P51  <indice_opcional>         -> ABRE_COL <expressao> FECHA_COL
P52                             | ε
P53  <cmd_escreva>             -> ESCREVA ABRE_PAR <lista_argumentos> FECHA_PAR
P54  <cmd_escreval>            -> ESCREVAL ABRE_PAR <lista_argumentos> FECHA_PAR
P55  <cmd_se>                  -> SE ABRE_PAR <expressao> FECHA_PAR ENTAO <lista_comandos> <senao_opcional> FIMSE
P56  <senao_opcional>          -> SENAO <lista_comandos>
P57                             | ε
P58  <cmd_para>                -> PARA ID DE <expressao> ATE <expressao> <passo_opcional> FACA <lista_comandos> FIMPARA
P59  <passo_opcional>          -> PASSO <numero_passo>
P60                             | ε
P61  <numero_passo>            -> NUM_INT
P62                             | MENOS NUM_INT
P63  <cmd_enquanto>            -> ENQUANTO ABRE_PAR <expressao> FECHA_PAR FACA <lista_comandos> FIMENQUANTO
P64  <cmd_retorne>             -> RETORNE <expressao>

-- Argumentos ------------------------------------------------------------------
P65  <lista_argumentos>        -> <expressao> <lista_argumentos_cauda>
P66  <lista_argumentos_cauda>  -> VIRGULA <expressao> <lista_argumentos_cauda>
P67                             | ε

-- Expressões ------------------------------------------------------------------
P68  <expressao>               -> <expr_logica>
P69  <expr_logica>             -> <expr_relacional> <expr_logica_cauda>
P70  <expr_logica_cauda>       -> E <expr_relacional> <expr_logica_cauda>
P71                             | OU <expr_relacional> <expr_logica_cauda>
P72                             | ε
P73  <expr_relacional>         -> <expr_aditiva> <expr_rel_cauda>
P74  <expr_rel_cauda>          -> OP_REL <expr_aditiva>
P75                             | ε
P76  <expr_aditiva>            -> <expr_mult> <expr_aditiva_cauda>
P77  <expr_aditiva_cauda>      -> MAIS <expr_mult> <expr_aditiva_cauda>
P78                             | ε
P79  <expr_mult>               -> <primario> <expr_mult_cauda>
P80  <expr_mult_cauda>         -> OP_MULT <primario> <expr_mult_cauda>
P81                             | ε
P82  <primario>                -> ID <cauda_primario>
P83                             | NUM_INT
P84                             | NUM_REAL
P85                             | STRING
P86                             | VERDADEIRO
P87                             | FALSO
P88                             | ABRE_PAR <expressao> FECHA_PAR
P89  <cauda_primario>          -> ABRE_COL <expressao> FECHA_COL
P90                             | ABRE_PAR <lista_argumentos> FECHA_PAR
P91                             | ε
```

**Totais:** 91 produções · 50 não-terminais · 49 terminais · 17 não-terminais anuláveis (§17).

### 6.1 Rastreabilidade: produção → evidência → origem → decisão

| Produções | Evidência (Anexo I) | Origem | Decisão |
|---|---|---|---|
| P01 | `algoritmo "PrimeiroPasso"` … `fimalgoritmo` | ANEXO I (todos os exemplos) | DEC-46 |
| P02, P04, P07 | `var` … `inicio` … nos programas sem sub-rotina | ANEXO I | DEC-29 |
| P03, P05, P06 | `funcao …` antes de `var`; comentário "Você os declara antes do início principal do programa"; exemplos de procedimento sem `var` aparente | ANEXO I — Funções / Procedimentos | DEC-28, DEC-29 |
| P08, P09 | `var` vazio (`PrimeiroPasso`); `var` com várias declarações | ANEXO I — Variáveis | DEC-29 |
| P10–P13 | `nome, sobrenome: caractere`; `idade: inteiro` | ANEXO I — Variáveis | — |
| P14, P17–P20 | `inteiro`, `real`, `caractere`, `logico` | ANEXO I — Variáveis | — |
| P15, P16 | `nomes: vetor[1..3] de caractere`; `notas: vetor[1..4] de real` | ANEXO I — Vetores | DEC-42 |
| P21–P24 | procedimentos e funções declarados antes do principal | ANEXO I — Procedimentos / Funções | DEC-30 |
| P25–P27 | `procedimento linha_decorativa`; `procedimento mostrar_erro(mensagem: caractere)` | ANEXO I — Procedimentos | DEC-41 |
| P28 | `funcao somar(a: inteiro, b: inteiro): inteiro`; `funcao eh_par(v: inteiro): logico` | ANEXO I — Funções | DEC-40 |
| P29–P32 | `a: inteiro, b: inteiro`; `mensagem: caractere`; `v: inteiro` | ANEXO I — Procedimentos / Funções | DEC-45 |
| P33–P35 | todo bloco do Anexo I tem ≥ 1 comando | ANEXO I | DEC-39 |
| P36–P43 | os oito tipos de comando dos exemplos | ANEXO I | — |
| P44–P48 | `nome <- "Florêncio"`; `nomes[1] <- "Ana"`; `mostrar_erro("…")`; `linha_decorativa` | ANEXO I — Variáveis / Vetores / Procedimentos | DEC-36, DEC-41 |
| P49–P52 | `leia(nome)`; `leia(notas[i])` | ANEXO I — Vetores | DEC-44 |
| P53, P54 | `escreva("Digite seu nome: ")`; `escreval("Muito prazer, ", nome, "!")` | ANEXO I | DEC-44 |
| P55–P57 | `se (chovendo = verdadeiro) entao`; `senao` com `se` aninhado | ANEXO I — Controle | AMB-10 |
| P58 | `para i de 1 ate 5 faca` | ANEXO I — Repetição | DEC-43 |
| P59–P62 | `passo -2`; `passo` ausente | ANEXO I — Repetição | DEC-34 |
| P63 | `enquanto (contador <= 5) faca` | ANEXO I — Repetição | AMB-10 |
| P64 | `retorne a + b`; `retorne verdadeiro`; `retorne falso` | ANEXO I — Funções | DEC-38 |
| P65–P67 | `somar(10, 5)`; `escreva("Olá, ", nome, ". Você tem ", idade, " anos.")` | ANEXO I | DEC-44 |
| P68–P72 | `(idade >= 12) E (altura >= 1.50)`; "O OU, basta um ser verdadeiro" | ANEXO I — Operadores | DEC-31, DEC-32 |
| P73–P75 | `idade >= 18`; `nome <> "João"`; `senhaDigitada = 1234`; `contador <= 5` | ANEXO I — Operadores / Controle | DEC-35 |
| P76–P78 | `n1 + n2`; `contador + 1` | ANEXO I — Operadores | DEC-33, DEC-35 |
| P79–P81 | `n1 * n2`; `n1 / n2`; `n1 \ n2`; `v MOD 2 = 0` | ANEXO I — Operadores / Funções | DEC-35 |
| P82, P89–P91 | `nome`; `notas[i]`; `nomes[2]`; `somar(10, 5)`; `eh_par(num)` | ANEXO I — Vetores / Funções | DEC-37 |
| P83–P87 | `18`; `1.50`; `"João"`; `verdadeiro`; `falso` | ANEXO I | — |
| P88 | `(idade >= 12)` | ANEXO I — Operadores | — |

## 7. Estrutura do programa (P01–P07)

O Anexo I mostra dois formatos seguros e uma página confusa:

| Formato | Evidência |
|---|---|
| `algoritmo "…"` `var` … `inicio` … `fimalgoritmo` | a maioria dos exemplos |
| `algoritmo "…"` `funcao …` `fimfuncao` `var` … `inicio` … `fimalgoritmo` | `RotinasComRetorno`, `eh_par` |
| procedimentos intercalados com dois exemplos sobrepostos | página de PROCEDIMENTOS (AMB-05) |

**Reconstrução (DEC-28, DEC-29):** a página de procedimentos não é reproduzida literalmente. Usamos as
duas evidências que se cruzam: o comentário "Você os declara antes do início principal do programa" e o
formato das funções (rotina antes de `var`). Resultado:

```
<corpo_programa> -> <secao_var> <bloco>                                        (sem sub-rotina: VAR obrigatória)
                  | <rotina> <lista_rotinas> <secao_var_opcional> <bloco>       (com ≥ 1 sub-rotina: VAR opcional)
```

O que isto aceita e rejeita:

| Forma | Aceita? | Por quê |
|---|---|---|
| `algoritmo "X"` `var` `inicio` … `fimalgoritmo` (var vazia) | sim | `PrimeiroPasso` (P02, P09) |
| `algoritmo "X"` `funcao …` `var` … `inicio` … | sim | Anexo I — Funções (P03, P05) |
| `algoritmo "X"` `procedimento …` `inicio` … (sem `var`) | sim | acomoda a página de procedimentos (P03, P06) |
| `algoritmo "X"` `inicio` … `fimalgoritmo` (sem rotina **e** sem `var`) | **não** | não aparece nos exemplos comuns; ampliaria a linguagem sem necessidade |
| sub-rotina **depois** de `var` | **não** | nenhum exemplo coerente mostra isso |

Isto **não** é afirmação sobre o Visualg completo; é a reconstrução mínima e coerente deste MiniVisualg.

## 8. Declarações (P08–P20)

- Depois de `VAR`, zero ou mais declarações (P08/P09). A lista acaba quando aparece `INICIO`: toda
  declaração começa com `ID`, então não é preciso separador de linha.
- Uma declaração é `<lista_ids> DOIS_PONTOS <tipo_decl>`, com um ou mais IDs separados por `VIRGULA`
  (`nome, sobrenome: caractere`).
- `<tipo_simples>` tem **exatamente** `INTEIRO` `REAL` `CARACTERE` `LOGICO`.
- Vetor (P16): `VETOR ABRE_COL NUM_INT INTERVALO NUM_INT FECHA_COL DE <tipo_simples>`. Os limites são
  **só `NUM_INT`** (DEC-42): não aceita `ID`, expressão, número negativo nem real. Vetor de vetor também
  não, porque o elemento é `<tipo_simples>`.
- Como quebra de linha não existe para a gramática, `nome: caractere idade: inteiro` numa linha só também
  é aceito. Isto decorre de REQ-03 (indentação e disposição não têm significado).

## 9. Procedimentos e funções (P21–P32)

- `<lista_rotinas>`: **zero ou mais** rotinas depois da primeira (P03 + P21/P22). Cada rotina é um
  procedimento ou uma função, em qualquer ordem (DEC-30). O Anexo I não mostra um programa misturando as
  duas formas; aceitar a mistura **não cria construção nova**, apenas repete as duas formas declarativas
  evidenciadas.
  - *Alternativa considerada:* permitir só um tipo de rotina por programa. Rejeitada: exigiria dois
    não-terminais de lista quase idênticos, sem base no material.
- **Procedimento (P25–P27):** `PROCEDIMENTO ID` seguido **ou** de `ABRE_PAR <lista_parametros> FECHA_PAR`
  **ou** de nada (ε), e depois `<bloco> FIMPROCEDIMENTO`. A fatoração é o `<parametros_procedimento>`.
  **Sem** parâmetros não há parênteses (DEC-41): `procedimento p()` é rejeitado.
- **Função (P28):** parênteses e lista de parâmetros **obrigatórios**, depois `DOIS_PONTOS <tipo_simples>`
  como tipo de retorno. **Não** há função sem parâmetros no Anexo I: `funcao f(): inteiro` é rejeitado
  (DEC-40). O retorno usa `<tipo_simples>` completo (o Anexo I só mostra `inteiro` e `logico`; restringir a
  esses dois criaria uma categoria de tipo artificial).
- **Parâmetro (P32):** `ID DOIS_PONTOS <tipo_simples>`. Sem parâmetro sem tipo, vetor como parâmetro,
  passagem por referência ou valor padrão (DEC-45).
- **Sem `var` local:** a rotina vai direto da assinatura para `<bloco>` (`INICIO …`).

## 10. Comandos (P33–P64)

- **Listas não vazias (DEC-39):** `<lista_comandos>` exige pelo menos um comando, no principal, em
  `se`, `senao`, `para`, `enquanto` e nas rotinas. Não existe comando vazio. A lista termina quando o
  próximo token é um fechamento: `FIMALGORITMO` `FIMSE` `SENAO` `FIMPARA` `FIMENQUANTO` `FIMPROCEDIMENTO`
  `FIMFUNCAO` (confirmação formal via FOLLOW na Fase D).
- **Oito comandos** (P36–P43): iniciado por `ID`, `leia`, `escreva`, `escreval`, `se`, `para`,
  `enquanto`, `retorne`. Nenhum outro.
- **Atribuição:** `ID ATRIBUICAO <expressao>` ou `ID ABRE_COL <expressao> FECHA_COL ATRIBUICAO <expressao>`
  (via fatoração, §12). Nenhum outro alvo: `f(1) <- 2` é rejeitado.
- **`leia` (P49–P52, DEC-44):** um único alvo, `ID` ou `ID [ expressão ]`. Sem lista de alvos.
- **`escreva`/`escreval` (P53, P54):** um ou mais argumentos. `escreval()` é rejeitado.
- **`se` (P55–P57):** `SE ABRE_PAR <expressao> FECHA_PAR ENTAO <lista_comandos> [SENAO <lista_comandos>]
  FIMSE`, com parênteses obrigatórios (AMB-10). O `se` aninhado sai naturalmente, porque um `<cmd_se>` é
  um `<comando>` dentro de `<lista_comandos>`. **Não** há "senão pendente" (dangling else): `FIMSE` fecha
  cada `se`.
- **`enquanto` (P63):** `ENQUANTO ABRE_PAR <expressao> FECHA_PAR FACA <lista_comandos> FIMENQUANTO`.
- **`para` (P58–P62):** `PARA ID DE <expressao> ATE <expressao> [PASSO <numero_passo>] FACA
  <lista_comandos> FIMPARA`.
  - **Limites como `<expressao>` (DEC-43).** O Anexo I só usa literais inteiros (`1`, `5`, `10`, `0`).
    *Alternativa comparada:* restringir a `NUM_INT` (ou `NUM_INT | ID`). Escolhemos `<expressao>` porque
    (a) `de … ate …` recebe **valores**, e o valor na linguagem já é a categoria `<expressao>`; (b) não cria
    token nem construção nova; (c) não afeta LL(1). Um limite negativo (`de -1`) continua **impossível**,
    porque `MENOS` não está em expressão. Revisável.
  - **`passo` restrito (DEC-34):** `<numero_passo> -> NUM_INT | MENOS NUM_INT`. Aceita `passo -2` e o
    positivo natural `passo 2`; rejeita `passo x`, `passo -x`, `passo 1.5` e qualquer subtração. A
    assimetria com os limites é intencional: o passo é o **único** lugar do Anexo I onde o sinal aparece.
- **`retorne` (P64, DEC-38):** `RETORNE <expressao>`, aceito **onde qualquer comando é aceito**.
  - O Anexo I só usa `retorne` dentro de função (inclusive dentro de `se` numa função).
  - Proibir `retorne` fora de função numa GLC exigiria duplicar toda a família de blocos e comandos
    (`<lista_comandos_funcao>`, `<cmd_se_funcao>`, …) só para isso.
  - Verificar se um `retorne` está no contexto certo é **restrição contextual/semântica**, não coberta
    por esta GLC. **Ampliação assumida e documentada**, não escondida.

## 11. Expressões e precedência (P65–P91)

Hierarquia (DEC-35), do nível **mais baixo** (liga por último) ao **mais alto**:

```
<expr_logica>      E, OU            (mesmo nível; zero ou mais)
   ↓
<expr_relacional>  OP_REL           (no máximo um; não encadeável)
   ↓
<expr_aditiva>     MAIS             (zero ou mais)
   ↓
<expr_mult>        OP_MULT          (* / \ MOD; zero ou mais)
   ↓
<primario>         ID, ID[...], ID(...), NUM_INT, NUM_REAL, STRING, VERDADEIRO, FALSO, ( expressão )
```

O que é **evidência** e o que é **decisão**:

- **Evidência direta:** `v MOD 2 = 0` só faz sentido com `MOD` ligando antes de `=`; a hierarquia
  deriva isso (§14-F). `(idade >= 12) E (altura >= 1.50)` mostra `E` combinando relações.
- **Decisão do grupo:** o resto da ordem (em particular `MAIS` abaixo de `OP_MULT` e o nível lógico abaixo do
  relacional). O Anexo I **não** prova a tabela inteira; ela é escolhida para dar uma GLC clara e
  preditiva, coerente com as categorias ensinadas.
- **`E` e `OU` no mesmo nível (DEC-32):** não há evidência de que um tenha precedência sobre o outro;
  **não** inventamos. Expressões mistas associam pela ordem de leitura (a cauda repete da esquerda para a
  direita). Para agrupar de outro jeito, usam-se parênteses (P88).
- **`OU` aceito (DEC-31):** é definido textualmente pelo próprio Anexo I e já é token (DEC-14). Excluí-lo
  tornaria impossível uma construção que o material ensina.
- **Relacional não encadeável:** P74 tem um único `OP_REL`; `a = b = c` não é derivável (sem evidência).
- **Sem `MENOS` (DEC-33):** nem binário (`a - b`) nem unário em expressão (`x <- -1`). O único `MENOS` da
  gramática está em P62.
- **Recursão à esquerda eliminada** pelo padrão `A -> B A_cauda`, `A_cauda -> op B A_cauda | ε`. Como
  não há análise semântica, a associatividade não altera nenhum resultado desta fase. Na implementação
  iterativa a leitura será da esquerda para a direita.
- **`<expressao> -> <expr_logica>` (P68)** é um apelido: dá um nome estável ("expressão") para ser
  referido por todos os comandos, independentemente de quantos níveis existam abaixo.

**Primário (P82–P91, DEC-37):** literais, `VERDADEIRO`/`FALSO`, expressão entre parênteses e o caso `ID`
fatorado (§12). Chamada de função em expressão exige ≥ 1 argumento: `f()` é rejeitado (DEC-44).

## 12. Fatoração dos comandos iniciados por ID (AMB-09)

Quatro formas de comando começam com `ID`:

```
nome <- "Ana"            ID ATRIBUICAO …
nomes[1] <- "Ana"        ID ABRE_COL … FECHA_COL ATRIBUICAO …
mostrar_erro("x")        ID ABRE_PAR … FECHA_PAR
linha_decorativa         ID
```

Escrever quatro alternativas `<comando> -> ID …` criaria um prefixo comum (`ID`), e com um único token de
lookahead o parser não saberia qual escolher. **Fatoração à esquerda (DEC-36):**

```
<cmd_id>           -> ID <cauda_comando_id>
<cauda_comando_id> -> ATRIBUICAO <expressao>                                  (A: atribuição simples)
                    | ABRE_COL <expressao> FECHA_COL ATRIBUICAO <expressao>   (B: atribuição indexada)
                    | ABRE_PAR <lista_argumentos> FECHA_PAR                   (C: chamada com argumentos)
                    | ε                                                       (D: chamada sem parâmetros)
```

Depois do `ID`, o **próximo** token decide: `ATRIBUICAO` → A; `ABRE_COL` → B; `ABRE_PAR` → C; qualquer
token que possa **seguir um comando** → D. A alternativa D (ε) existe porque `linha_decorativa` é uma
chamada completa.

**Limitação sintática da forma D (§16):** qualquer `ID` sozinho em posição de comando tem a mesma forma de
uma chamada sem parâmetros. A GLC não sabe se o `ID` é procedimento: verifica **forma**, não declaração.

O mesmo problema aparece em **expressões** e é resolvido do mesmo jeito (DEC-37):

```
<primario>       -> ID <cauda_primario> | …
<cauda_primario> -> ABRE_COL <expressao> FECHA_COL      (notas[i])
                  | ABRE_PAR <lista_argumentos> FECHA_PAR   (somar(10, 5))
                  | ε                                   (nome)
```

E, em `leia`, por `<referencia> -> ID <indice_opcional>`, sem a forma de chamada.

As três caudas são **separadas de propósito**: aceitam conjuntos diferentes (a de comando tem
`ATRIBUICAO`; a de primário não; a de `leia` não aceita chamada).

**Ponto para a Fase D:** a decisão "ε" das caudas depende de o próximo token **não** poder iniciar outra
alternativa. Isso é exatamente o que FIRST/FOLLOW verifica (§17).

## 13. Mapeamento Anexo I → produções (cobertura)

| Família do Anexo I | Exemplo representativo | Produções usadas | Deriva? | Decisão / ambiguidade |
|---|---|---|---|---|
| Esqueleto | `algoritmo "PrimeiroPasso"` `var` `inicio` `escreval("Olá, mundo!")` `fimalgoritmo` | P01 P02 P04 P09 P07 P33 P39 P54 P65 P67 P35 | **sim** (§14-A) | DEC-29 |
| Comentários | `// A área de variáveis está vazia` | nenhuma (consumido pelo léxico) | n/a | DEC-08, LEX-08 |
| Variáveis | `nome, sobrenome: caractere`; `idade: inteiro`; `nome <- "Florêncio"`; `portaAberta <- verdadeiro` | P08 P10–P14 P17–P20; P44 P45 P85/P86 | **sim** (§14-B) | — |
| Operadores aritméticos | `n1 + n2`, `n1 * n2`, `n1 / n2`, `n1 \ n2`, `contador + 1`, `soma / 4` | P76–P81 P82 P83 | **sim** | DEC-33, DEC-35 |
| Relacionais | `idade >= 18`, `contador <= 5`, `senhaDigitada = 1234`, `nome <> "João"` | P73 P74 | **sim** | DEC-35 |
| Lógicos | `podeBrincar <- (idade >= 12) E (altura >= 1.50)`; `OU` (definido textualmente) | P69 P70 P71 P88 | **sim** (§14-G) | DEC-31, DEC-32 |
| `se` | `se (chovendo = verdadeiro) entao` … `fimse` | P40 P55 P57 | **sim** | AMB-10 |
| `se`/`senao` aninhado | `se (idade >= 18)` … `senao` `se (idade >= 12)` … `fimse` `fimse` | P55 P56 P40 | **sim** (§14-D) | AMB-10 |
| `para` simples | `para i de 1 ate 5 faca` … `fimpara` | P41 P58 P60 | **sim** | DEC-43 |
| `para` com passo negativo | `para i de 10 ate 0 passo -2 faca` | P58 P59 P62 | **sim** (§14-E) | DEC-34 |
| `enquanto` | `enquanto (contador <= 5) faca` … `contador <- contador + 1` … `fimenquanto` | P42 P63 P45 P77 | **sim** | AMB-10 |
| Vetor | `nomes: vetor[1..3] de caractere`; `nomes[1] <- "Ana"`; `leia(notas[i])`; `nomes[2]` | P15 P16; P46; P49–P51; P89 | **sim** (§14-C) | DEC-42, DEC-44 |
| Procedimento sem parâmetro | `procedimento linha_decorativa` … `fimprocedimento`; chamada `linha_decorativa` | P03 P23 P25 P27; P44 P48 | **sim** (§14-J) | DEC-28, DEC-41 |
| Procedimento com parâmetro | `procedimento mostrar_erro(mensagem: caractere)`; chamada `mostrar_erro("…")` | P25 P26 P29 P31 P32; P44 P47 | **sim** | DEC-41, DEC-45 |
| Função `somar` | `funcao somar(a: inteiro, b: inteiro): inteiro` `inicio` `retorne a + b` `fimfuncao`; `resultado <- somar(10, 5)` | P03 P24 P28–P32 P64 P77; P45 P82 P90 | **sim** (§14-H, §14-I) | DEC-38, DEC-40 |
| Função `eh_par` | `funcao eh_par(v: inteiro): logico` … `se (v MOD 2 = 0) entao retorne verdadeiro senao retorne falso fimse` …; `escreval(…, eh_par(num))` | P28 P55 P56 P64 P80 P74 P86 P87; P66 P90 | **sim** (§14-F + estrutura de §14-H) | DEC-38 |

## 14. Derivações / validações manuais

Cada exemplo é primeiro convertido em tokens pelo contrato léxico e depois derivado a partir do
não-terminal indicado. As derivações são **mais à esquerda** (`⇒Pnn` = uma aplicação de Pnn); "…" repete
o prefixo/sufixo inalterado. Textos de string não mostrados no material aparecem como `"…"`.

**Lema P (cadeia de primário).** Para qualquer primário `p`, `<expressao> ⇒* p`:
`<expressao> ⇒P68 <expr_logica> ⇒P69 <expr_relacional> <expr_logica_cauda> ⇒P73 <expr_aditiva>
<expr_rel_cauda> <expr_logica_cauda> ⇒P76 <expr_mult> <expr_aditiva_cauda> … ⇒P79 <primario>
<expr_mult_cauda> …`, deriva-se `<primario> ⇒* p` e as quatro caudas vão a ε (P81, P78, P75, P72).
O mesmo vale com `<expr_aditiva> ⇒* p` (P76, P79, P81, P78).

**Lema R (relação simples).** `<expressao> ⇒* x OP_REL y` quando `x` e `y` são primários:
P68, P69, P73; `<expr_aditiva> ⇒* x`; P74; `<expr_aditiva> ⇒* y`; P72.

### A. Programa mínimo — ✔ derivável

```
algoritmo "PrimeiroPasso" var inicio escreval("Olá, mundo!") fimalgoritmo
ALGORITMO STRING VAR INICIO ESCREVAL ABRE_PAR STRING FECHA_PAR FIMALGORITMO
```

```
<programa>
⇒P01 ALGORITMO STRING <corpo_programa> FIMALGORITMO
⇒P02 ALGORITMO STRING <secao_var> <bloco> FIMALGORITMO
⇒P04 ALGORITMO STRING VAR <lista_declaracoes> <bloco> FIMALGORITMO
⇒P09 ALGORITMO STRING VAR <bloco> FIMALGORITMO
⇒P07 ALGORITMO STRING VAR INICIO <lista_comandos> FIMALGORITMO
⇒P33 … INICIO <comando> <lista_comandos_cauda> FIMALGORITMO
⇒P39 … INICIO <cmd_escreval> <lista_comandos_cauda> FIMALGORITMO
⇒P54 … INICIO ESCREVAL ABRE_PAR <lista_argumentos> FECHA_PAR <lista_comandos_cauda> FIMALGORITMO
⇒P65 … ESCREVAL ABRE_PAR <expressao> <lista_argumentos_cauda> FECHA_PAR <lista_comandos_cauda> …
⇒*   … ESCREVAL ABRE_PAR STRING <lista_argumentos_cauda> FECHA_PAR <lista_comandos_cauda> …   (Lema P, P85)
⇒P67 … ESCREVAL ABRE_PAR STRING FECHA_PAR <lista_comandos_cauda> FIMALGORITMO
⇒P35 ALGORITMO STRING VAR INICIO ESCREVAL ABRE_PAR STRING FECHA_PAR FIMALGORITMO   ✔
```

### B. Declaração múltipla — ✔ derivável

```
nome, sobrenome: caractere          →  ID VIRGULA ID DOIS_PONTOS CARACTERE
```

```
<declaracao>
⇒P10 <lista_ids> DOIS_PONTOS <tipo_decl>
⇒P11 ID <lista_ids_cauda> DOIS_PONTOS <tipo_decl>
⇒P12 ID VIRGULA ID <lista_ids_cauda> DOIS_PONTOS <tipo_decl>
⇒P13 ID VIRGULA ID DOIS_PONTOS <tipo_decl>
⇒P14 ID VIRGULA ID DOIS_PONTOS <tipo_simples>
⇒P19 ID VIRGULA ID DOIS_PONTOS CARACTERE   ✔
```

### C. Atribuição vetorial — ✔ derivável

```
nomes[1] <- "Ana"                   →  ID ABRE_COL NUM_INT FECHA_COL ATRIBUICAO STRING
```

```
<comando>
⇒P36 <cmd_id>
⇒P44 ID <cauda_comando_id>
⇒P46 ID ABRE_COL <expressao> FECHA_COL ATRIBUICAO <expressao>
⇒*   ID ABRE_COL NUM_INT FECHA_COL ATRIBUICAO <expressao>        (Lema P, P83)
⇒*   ID ABRE_COL NUM_INT FECHA_COL ATRIBUICAO STRING   ✔          (Lema P, P85)
```

A declaração `nomes: vetor[1..3] de caractere` (`ID DOIS_PONTOS VETOR ABRE_COL NUM_INT INTERVALO NUM_INT
FECHA_COL DE CARACTERE`) sai por P10, P11, P13, P15, P16, P19. ✔

### D. Condicional aninhado — ✔ derivável

```
se (idade >= 18) entao escreval("…") senao se (idade >= 12) entao escreval("…") senao escreval("…") fimse fimse
SE ABRE_PAR ID OP_REL NUM_INT FECHA_PAR ENTAO ESCREVAL ABRE_PAR STRING FECHA_PAR
SENAO SE ABRE_PAR ID OP_REL NUM_INT FECHA_PAR ENTAO ESCREVAL ABRE_PAR STRING FECHA_PAR
SENAO ESCREVAL ABRE_PAR STRING FECHA_PAR FIMSE FIMSE
```

```
<cmd_se>
⇒P55 SE ABRE_PAR <expressao> FECHA_PAR ENTAO <lista_comandos> <senao_opcional> FIMSE
⇒*   SE ABRE_PAR ID OP_REL NUM_INT FECHA_PAR ENTAO <lista_comandos> <senao_opcional> FIMSE     (Lema R)
⇒*   … ENTAO ESCREVAL ABRE_PAR STRING FECHA_PAR <senao_opcional> FIMSE     (P33 P39 P54 P65 Lema P P67 P35)
⇒P56 … FECHA_PAR SENAO <lista_comandos> FIMSE
⇒P33 … SENAO <comando> <lista_comandos_cauda> FIMSE
⇒P40 … SENAO <cmd_se> <lista_comandos_cauda> FIMSE
⇒*   … SENAO SE ABRE_PAR ID OP_REL NUM_INT FECHA_PAR ENTAO ESCREVAL ABRE_PAR STRING FECHA_PAR
         SENAO ESCREVAL ABRE_PAR STRING FECHA_PAR FIMSE <lista_comandos_cauda> FIMSE   (mesmos passos, P56)
⇒P35 … FIMSE FIMSE   ✔
```

O `se` interno é um `<comando>` dentro da `<lista_comandos>` do `senao`: aninhamento sem regra especial.

### E. `para` com passo negativo — ✔ derivável

```
para i de 10 ate 0 passo -2 faca escreval(i) fimpara
PARA ID DE NUM_INT ATE NUM_INT PASSO MENOS NUM_INT FACA ESCREVAL ABRE_PAR ID FECHA_PAR FIMPARA
```

```
<cmd_para>
⇒P58 PARA ID DE <expressao> ATE <expressao> <passo_opcional> FACA <lista_comandos> FIMPARA
⇒*   PARA ID DE NUM_INT ATE <expressao> <passo_opcional> FACA <lista_comandos> FIMPARA   (Lema P, P83)
⇒*   PARA ID DE NUM_INT ATE NUM_INT <passo_opcional> FACA <lista_comandos> FIMPARA       (Lema P, P83)
⇒P59 … ATE NUM_INT PASSO <numero_passo> FACA <lista_comandos> FIMPARA
⇒P62 … ATE NUM_INT PASSO MENOS NUM_INT FACA <lista_comandos> FIMPARA
⇒*   … FACA ESCREVAL ABRE_PAR ID FECHA_PAR FIMPARA   ✔       (P33 P39 P54 P65; Lema P com P82 P91; P67 P35)
```

### F. `v MOD 2 = 0` — ✔ derivável (e mostra a precedência)

```
ID OP_MULT NUM_INT OP_REL NUM_INT
```

```
<expressao>
⇒P68 <expr_logica>
⇒P69 <expr_relacional> <expr_logica_cauda>
⇒P73 <expr_aditiva> <expr_rel_cauda> <expr_logica_cauda>
⇒P76 <expr_mult> <expr_aditiva_cauda> <expr_rel_cauda> <expr_logica_cauda>
⇒P79 <primario> <expr_mult_cauda> <expr_aditiva_cauda> …
⇒P82 ID <cauda_primario> <expr_mult_cauda> …
⇒P91 ID <expr_mult_cauda> <expr_aditiva_cauda> <expr_rel_cauda> <expr_logica_cauda>
⇒P80 ID OP_MULT <primario> <expr_mult_cauda> …
⇒P83 ID OP_MULT NUM_INT <expr_mult_cauda> <expr_aditiva_cauda> <expr_rel_cauda> <expr_logica_cauda>
⇒P81 ID OP_MULT NUM_INT <expr_aditiva_cauda> <expr_rel_cauda> <expr_logica_cauda>
⇒P78 ID OP_MULT NUM_INT <expr_rel_cauda> <expr_logica_cauda>
⇒P74 ID OP_MULT NUM_INT OP_REL <expr_aditiva> <expr_logica_cauda>
⇒*   ID OP_MULT NUM_INT OP_REL NUM_INT <expr_logica_cauda>       (P76 P79 P83 P81 P78)
⇒P72 ID OP_MULT NUM_INT OP_REL NUM_INT   ✔
```

`v MOD 2` inteiro fica dentro da `<expr_aditiva>` à **esquerda** de `OP_REL`: `MOD` liga antes de `=`.

### G. `(idade >= 12) E (altura >= 1.50)` — ✔ derivável

```
ABRE_PAR ID OP_REL NUM_INT FECHA_PAR E ABRE_PAR ID OP_REL NUM_REAL FECHA_PAR
```

```
<expressao>
⇒P68 <expr_logica>
⇒P69 <expr_relacional> <expr_logica_cauda>
⇒*   <primario> <expr_logica_cauda>                       (P73 P76 P79; caudas P81 P78 P75 no fim)
⇒P88 ABRE_PAR <expressao> FECHA_PAR <expr_logica_cauda>
⇒*   ABRE_PAR ID OP_REL NUM_INT FECHA_PAR <expr_logica_cauda>                 (Lema R)
⇒P70 … FECHA_PAR E <expr_relacional> <expr_logica_cauda>
⇒*   … E ABRE_PAR ID OP_REL NUM_REAL FECHA_PAR <expr_logica_cauda>           (idem, P88, Lema R, P84)
⇒P72 ABRE_PAR ID OP_REL NUM_INT FECHA_PAR E ABRE_PAR ID OP_REL NUM_REAL FECHA_PAR   ✔
```

Trocando `E` por `OU`, a mesma derivação vale com P71 no lugar de P70. ✔

### H. Função `somar` — ✔ derivável

```
funcao somar(a: inteiro, b: inteiro): inteiro inicio retorne a + b fimfuncao
FUNCAO ID ABRE_PAR ID DOIS_PONTOS INTEIRO VIRGULA ID DOIS_PONTOS INTEIRO FECHA_PAR DOIS_PONTOS INTEIRO
INICIO RETORNE ID MAIS ID FIMFUNCAO
```

```
<rotina>
⇒P24 <funcao>
⇒P28 FUNCAO ID ABRE_PAR <lista_parametros> FECHA_PAR DOIS_PONTOS <tipo_simples> <bloco> FIMFUNCAO
⇒P29 … ABRE_PAR <parametro> <lista_parametros_cauda> FECHA_PAR …
⇒P32 … ABRE_PAR ID DOIS_PONTOS <tipo_simples> <lista_parametros_cauda> FECHA_PAR …
⇒P17 … ABRE_PAR ID DOIS_PONTOS INTEIRO <lista_parametros_cauda> FECHA_PAR …
⇒P30 … INTEIRO VIRGULA <parametro> <lista_parametros_cauda> FECHA_PAR …
⇒*   … INTEIRO VIRGULA ID DOIS_PONTOS INTEIRO <lista_parametros_cauda> FECHA_PAR …   (P32 P17)
⇒P31 … FECHA_PAR DOIS_PONTOS <tipo_simples> <bloco> FIMFUNCAO
⇒P17 … FECHA_PAR DOIS_PONTOS INTEIRO <bloco> FIMFUNCAO
⇒P07 … INTEIRO INICIO <lista_comandos> FIMFUNCAO
⇒P33 … INICIO <comando> <lista_comandos_cauda> FIMFUNCAO
⇒P43 … INICIO <cmd_retorne> <lista_comandos_cauda> FIMFUNCAO
⇒P64 … INICIO RETORNE <expressao> <lista_comandos_cauda> FIMFUNCAO
⇒*   … RETORNE <expr_aditiva> <expr_rel_cauda> <expr_logica_cauda> …   (P68 P69 P73)
⇒*   … RETORNE ID <expr_aditiva_cauda> …                                   (P76 P79 P82 P91 P81)
⇒P77 … RETORNE ID MAIS <expr_mult> <expr_aditiva_cauda> …
⇒*   … RETORNE ID MAIS ID <expr_aditiva_cauda> <expr_rel_cauda> <expr_logica_cauda> …   (P79 P82 P91 P81)
⇒*   … RETORNE ID MAIS ID <lista_comandos_cauda> FIMFUNCAO                (P78 P75 P72)
⇒P35 FUNCAO ID ABRE_PAR ID DOIS_PONTOS INTEIRO VIRGULA ID DOIS_PONTOS INTEIRO FECHA_PAR DOIS_PONTOS
     INTEIRO INICIO RETORNE ID MAIS ID FIMFUNCAO   ✔
```

`eh_par` segue a mesma estrutura, com `LOGICO` (P20) no retorno e um `<cmd_se>` no corpo cujos ramos são
`<cmd_retorne>` com `VERDADEIRO` (P86) e `FALSO` (P87); a condição é o exemplo F entre parênteses. ✔

### I. Chamada em expressão — ✔ derivável

```
resultado <- somar(10, 5)           →  ID ATRIBUICAO ID ABRE_PAR NUM_INT VIRGULA NUM_INT FECHA_PAR
```

```
<comando>
⇒P36 <cmd_id>
⇒P44 ID <cauda_comando_id>
⇒P45 ID ATRIBUICAO <expressao>
⇒*   ID ATRIBUICAO <primario> <expr_mult_cauda> <expr_aditiva_cauda> <expr_rel_cauda> <expr_logica_cauda>
⇒P82 ID ATRIBUICAO ID <cauda_primario> …
⇒P90 ID ATRIBUICAO ID ABRE_PAR <lista_argumentos> FECHA_PAR …
⇒P65 … ABRE_PAR <expressao> <lista_argumentos_cauda> FECHA_PAR …
⇒*   … ABRE_PAR NUM_INT <lista_argumentos_cauda> FECHA_PAR …                   (Lema P, P83)
⇒P66 … ABRE_PAR NUM_INT VIRGULA <expressao> <lista_argumentos_cauda> FECHA_PAR …
⇒*   … ABRE_PAR NUM_INT VIRGULA NUM_INT <lista_argumentos_cauda> FECHA_PAR …   (Lema P, P83)
⇒P67 … FECHA_PAR <expr_mult_cauda> <expr_aditiva_cauda> <expr_rel_cauda> <expr_logica_cauda>
⇒*   ID ATRIBUICAO ID ABRE_PAR NUM_INT VIRGULA NUM_INT FECHA_PAR   ✔       (P81 P78 P75 P72)
```

### J. Procedimento sem parâmetros (reconstrução documentada) — ✔ derivável

```
algoritmo "ProcedimentosSemParametros"
procedimento linha_decorativa
inicio
   escreval("--------------------------")
fimprocedimento
inicio
   linha_decorativa
fimalgoritmo

ALGORITMO STRING PROCEDIMENTO ID INICIO ESCREVAL ABRE_PAR STRING FECHA_PAR FIMPROCEDIMENTO
INICIO ID FIMALGORITMO
```

```
<programa>
⇒P01 ALGORITMO STRING <corpo_programa> FIMALGORITMO
⇒P03 ALGORITMO STRING <rotina> <lista_rotinas> <secao_var_opcional> <bloco> FIMALGORITMO
⇒P23 ALGORITMO STRING <procedimento> <lista_rotinas> <secao_var_opcional> <bloco> FIMALGORITMO
⇒P25 … PROCEDIMENTO ID <parametros_procedimento> <bloco> FIMPROCEDIMENTO <lista_rotinas> …
⇒P27 … PROCEDIMENTO ID <bloco> FIMPROCEDIMENTO <lista_rotinas> …
⇒*   … PROCEDIMENTO ID INICIO ESCREVAL ABRE_PAR STRING FECHA_PAR FIMPROCEDIMENTO <lista_rotinas> …
                                                                        (P07 P33 P39 P54 P65 Lema P P67 P35)
⇒P22 … FIMPROCEDIMENTO <secao_var_opcional> <bloco> FIMALGORITMO
⇒P06 … FIMPROCEDIMENTO <bloco> FIMALGORITMO
⇒P07 … FIMPROCEDIMENTO INICIO <lista_comandos> FIMALGORITMO
⇒P33 … INICIO <comando> <lista_comandos_cauda> FIMALGORITMO
⇒P36 … INICIO <cmd_id> <lista_comandos_cauda> FIMALGORITMO
⇒P44 … INICIO ID <cauda_comando_id> <lista_comandos_cauda> FIMALGORITMO
⇒P48 … INICIO ID <lista_comandos_cauda> FIMALGORITMO
⇒P35 ALGORITMO STRING PROCEDIMENTO ID INICIO ESCREVAL ABRE_PAR STRING FECHA_PAR FIMPROCEDIMENTO
     INICIO ID FIMALGORITMO   ✔
```

Com `var` entre `fimprocedimento` e `inicio`, a derivação troca P06 por P05, P04. ✔

## 15. Construções deliberadamente rejeitadas

Documental; nenhum teste foi executado. "Onde falha" indica o primeiro token que nenhuma produção aceita.

| Forma | Tokens (trecho) | Onde falha | Motivo |
|---|---|---|---|
| `x <- a - b` | `ID ATRIBUICAO ID MENOS ID` | em `MENOS` | subtração binária sem evidência; `MENOS` não está em expressão (DEC-33) |
| `x <- -1` | `ID ATRIBUICAO MENOS …` | em `MENOS` | sinal só existe em `passo` (DEC-34) |
| `se idade >= 18 entao` | `SE ID …` | em `ID` (esperado `ABRE_PAR`) | parênteses obrigatórios (AMB-10) |
| `enquanto contador <= 5 faca` | `ENQUANTO ID …` | em `ID` (esperado `ABRE_PAR`) | idem (AMB-10) |
| `escreval()` | `ESCREVAL ABRE_PAR FECHA_PAR` | em `FECHA_PAR` | sem argumento não há evidência (DEC-44) |
| `funcao f(): inteiro` | `FUNCAO ID ABRE_PAR FECHA_PAR …` | em `FECHA_PAR` (esperado `ID`) | função sem parâmetros sem evidência (DEC-40) |
| `procedimento p()` | `PROCEDIMENTO ID ABRE_PAR FECHA_PAR` | em `FECHA_PAR` (esperado `ID`) | sem parâmetros = sem parênteses (DEC-41) |
| `v: vetor[x..10] de inteiro` | `… VETOR ABRE_COL ID …` | em `ID` (esperado `NUM_INT`) | limites só `NUM_INT` (DEC-42) |
| `x < 10` | — | **erro léxico** em `<` | `<` isolado não é token (DEC-16); nem chega à sintaxe |
| `x > 10` | — | **erro léxico** em `>` | idem |
| `a = b = c` | `ID OP_REL ID OP_REL ID` | no 2º `OP_REL` | relacional não encadeável (DEC-35) |
| `passo x` / `passo -x` / `passo 1.5` | `PASSO ID` / `PASSO MENOS ID` / `PASSO NUM_REAL` | no token após `PASSO`/`MENOS` | passo restrito (DEC-34) |
| `algoritmo "X" inicio … fimalgoritmo` | `ALGORITMO STRING INICIO` | em `INICIO` | sem rotina, `VAR` é obrigatória (DEC-29) |
| rotina depois de `var` | `… VAR … PROCEDIMENTO` | em `PROCEDIMENTO` | rotinas só antes de `VAR` (DEC-28) |
| `var` dentro de rotina | `FUNCAO … INTEIRO VAR` | em `VAR` (esperado `INICIO`) | sem variável local (§9) |
| `leia(a, b)` | `LEIA ABRE_PAR ID VIRGULA` | em `VIRGULA` | um único alvo (DEC-44) |
| `somar()` em expressão | `ID ABRE_PAR FECHA_PAR` | em `FECHA_PAR` | chamada sem argumento sem evidência (DEC-44) |
| `f(1) <- 2` | `ID ABRE_PAR NUM_INT FECHA_PAR ATRIBUICAO` | em `ATRIBUICAO` | alvo de atribuição é só `ID` ou `ID[…]` (§10) |
| `v[1]` sozinho como comando | `ID ABRE_COL NUM_INT FECHA_COL` + fechamento | no token após `FECHA_COL` (esperado `ATRIBUICAO`) | acesso indexado não é comando |
| bloco vazio: `se (x) entao fimse` | `… ENTAO FIMSE` | em `FIMSE` | listas de comandos não vazias (DEC-39) |

## 16. Limitações sintáticas vs. semânticas

A GLC verifica a **forma** das cadeias de tokens. Ela **não** verifica:

- se um `ID` foi declarado;
- se um `ID` é variável, vetor, função ou procedimento;
- tipos dos operandos e compatibilidade de atribuição;
- se o índice de vetor é inteiro ou está nos limites;
- se a função retorna o tipo declarado, ou se retorna;
- se um procedimento é chamado como procedimento (`linha_decorativa`) e uma função como função;
- se `RETORNE` está dentro de uma função (DEC-38);
- quantidade e tipos de argumentos contra a assinatura;
- escopo.

Consequência: algumas cadeias **bem formadas** seriam **semanticamente** inválidas — por exemplo
`x` sozinho como comando quando `x` é uma variável, `retorne 1` no programa principal ou
`escreva("a" + verdadeiro)`. **Isso é o esperado** para um projeto que cobre análise léxica e sintática
(REQ-35); não é bug.

## 17. Preparação para FIRST/FOLLOW (Fase D)

**Não** é a análise formal; é a lista do que a Fase D deve verificar.

**Não-terminais anuláveis (têm alternativa ε), 17:**
`<secao_var_opcional>` `<lista_declaracoes>` `<lista_ids_cauda>` `<lista_rotinas>`
`<parametros_procedimento>` `<lista_parametros_cauda>` `<lista_comandos_cauda>` `<cauda_comando_id>`
`<indice_opcional>` `<senao_opcional>` `<passo_opcional>` `<lista_argumentos_cauda>`
`<expr_logica_cauda>` `<expr_rel_cauda>` `<expr_aditiva_cauda>` `<expr_mult_cauda>` `<cauda_primario>`

Nenhum outro não-terminal deriva ε, e `<programa>` sempre produz ao menos
`ALGORITMO STRING … FIMALGORITMO`.

**Auditoria de recursão à esquerda (feita nesta fase):**

- **Direta:** nenhuma produção tem a forma `<A> -> <A> …`.
- **Indireta:** o primeiro símbolo de cada produção é um terminal ou um não-terminal **mais "baixo"** na
  cadeia `<expressao> → <expr_logica> → <expr_relacional> → <expr_aditiva> → <expr_mult> → <primario>`,
  e `<primario>` começa sempre com terminal (inclusive P88, que começa com `ABRE_PAR`). Nos comandos,
  `<comando> → <cmd_…>` e todo `<cmd_…>` começa com terminal. Em estrutura/declarações, os primeiros
  símbolos levam a `VAR`, `PROCEDIMENTO`, `FUNCAO`, `ID`, `INICIO` ou tipos. Não há ciclo.
  As listas usam recursão **à direita** (`<A> -> x <A> | ε`).

**Pontos quentes para a Fase D (verificação qualitativa; nenhum conflito óbvio encontrado):**

| Ponto | O que precisa ser disjunto | Por que parece seguro |
|---|---|---|
| `<cauda_comando_id>` (P45–P48) | `{ATRIBUICAO, ABRE_COL, ABRE_PAR}` × o que pode seguir um comando | nenhum comando começa com `ATRIBUICAO`, `[` ou `(`; fechamentos também não |
| `<cauda_primario>` (P89–P91) | `{ABRE_COL, ABRE_PAR}` × o que pode seguir uma expressão | expressão é seguida por operador, `)`, `]`, `,`, `ATE`, `PASSO`, `FACA` ou início/fim de comando — nenhum é `[` ou `(` |
| `<lista_comandos_cauda>` (P34/P35) | início de comando × fechamentos | inícios: `ID LEIA ESCREVA ESCREVAL SE PARA ENQUANTO RETORNE`; fechamentos: `FIM…` e `SENAO` |
| `<expr_logica_cauda>`, `<expr_rel_cauda>`, `<expr_aditiva_cauda>`, `<expr_mult_cauda>` | operador do nível × o que segue o nível | cada operador (`E`/`OU`, `OP_REL`, `MAIS`, `OP_MULT`) só aparece no seu nível |
| `<corpo_programa>` (P02/P03) | `VAR` × `PROCEDIMENTO`/`FUNCAO` | inícios distintos |
| `<lista_rotinas>`, `<secao_var_opcional>` | `PROCEDIMENTO FUNCAO` / `VAR` × o que vem depois | seguem `VAR` ou `INICIO` |
| `<parametros_procedimento>` | `ABRE_PAR` × `INICIO` | disjuntos |
| `<lista_declaracoes>` | `ID` × `INICIO` | disjuntos |
| `<senao_opcional>` | `SENAO` × `FIMSE` | disjuntos |
| `<passo_opcional>` | `PASSO` × `FACA` | disjuntos |

**Ponto de atenção sem newline (§2):** como a quebra de linha não existe para o parser, um comando que
termina em expressão (`x <- a`) é seguido **diretamente** pelo primeiro token do próximo comando
(`b <- 1`: `… ID ID ATRIBUICAO …`). A Fase D deve confirmar que nenhum token que inicia comando
pertence ao FIRST das caudas de expressão. Pela análise qualitativa acima, não pertence.

## 18. Contrato congelado para a Fase D

1. A gramática oficial é a lista P01–P91 da §6. A Fase D calcula nullable, FIRST e FOLLOW **sobre ela**,
   sem reescrevê-la; se aparecer um conflito, a correção é uma **emenda registrada** (nova DEC), não uma
   troca silenciosa.
2. Terminais = os 49 do contrato léxico; `TOKEN_EOF` entra na Fase D **apenas** como marcador de fim
   (`$`) no FOLLOW de `<programa>`, por convenção do algoritmo — não como terminal da linguagem.
3. Os pontos da §17 devem receber verificação formal explícita, começando por `<cauda_comando_id>` e
   `<cauda_primario>`.
4. Nenhum atributo (`EQ`, `MOD`, …) participa da escolha de produção.
5. `MENOS` aparece só em P62; `OP_REL` só em P74; `OU` só em P71.
6. As decisões DEC-28 a DEC-46 são o comportamento esperado; a Fase E derivará delas os testes positivos
   e negativos (§13, §15).

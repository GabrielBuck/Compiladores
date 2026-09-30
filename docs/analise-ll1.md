# Análise LL(1) da GLC do MiniVisualg — Fase D

> **Resultado: a GLC P01–P91 é LL(1) para o vocabulário congelado.**
> Demonstração: nullable, FIRST e FOLLOW calculados por ponto fixo sobre as 91 produções publicadas em
> `gramatica.md` §6; SELECT de cada uma das 91 produções; **85 pares** de alternativas
> comparados em **24 não-terminais**, todas as interseções vazias; tabela preditiva com
> **281 células, nenhuma com duas produções**.
>
> **Nenhuma produção foi alterada.** P01–P91 são idênticas às do commit `a712477` (Fase C).
> Nada foi implementado: não há parser, lexer nem código C. Decisão registrada: DEC-47.

## 1. Objetivo e método

Provar formalmente, e não por inspeção, que um parser **descendente recursivo preditivo** com **um token
de lookahead** consegue escolher sempre, sem backtracking, a produção correta da GLC da Fase C.

Método:

1. **Nullable** por ponto fixo (§4).
2. **FIRST** por ponto fixo (§5).
3. **FOLLOW** por ponto fixo (§6).
4. **SELECT** de cada produção P01–P91 (§7).
5. **Condição LL(1):** para cada não-terminal com mais de uma produção, os SELECTs das alternativas são
   disjuntos dois a dois (§8).
6. **Tabela preditiva** `M[A, t]` a partir dos SELECTs, conferindo que nenhuma célula recebe duas
   produções (§10).
7. **Análises específicas** dos pontos críticos apontados pela Fase C (§9).

Os cálculos foram feitos à mão e conferidos por um verificador descartável, fora do repositório (§12).
As tabelas deste documento foram geradas a partir da gramática publicada, para evitar erro de
transcrição, e o resultado do verificador foi comparado conjunto a conjunto com o cálculo manual
(0 divergências).

## 2. Notação e o marcador `$`

- `ε` = cadeia vazia. `A ⇒* ε` = A deriva a cadeia vazia.
- Terminais = os 49 nomes impressos do contrato léxico (`especificacao-lexica.md` §3), sem atributos:
  `OP_REL` é um terminal; `EQ`, `GE` etc. **não** são (a escolha de produção nunca olha o atributo).
- Terminais são listados **na ordem do contrato léxico** (classe, reservadas, `E`, `OU`, operadores,
  pontuação), com `$` e `ε` no fim.
- **`$` é um marcador metalinguístico de fim da cadeia**, usado **só** na definição de FOLLOW:
  `$ ∈ FOLLOW(<programa>)`.
  - `$` **não** é terminal da GLC MiniVisualg e **não** é token produzido pelo scanner.
  - Na implementação (Fase G), o papel de `$` será desempenhado por `TOKEN_EOF`: depois de reconhecer
    `<programa>` (P01), o parser exige que o próximo token seja `TOKEN_EOF` (DEC-23, DEC-46).
  - Como `<programa>` não é anulável, `$` **não entra em nenhum SELECT** e, portanto, em nenhuma célula
    da tabela preditiva (verificado, §12).

## 3. Gramática analisada

A gramática é **exatamente** a lista P01–P91 de `gramatica.md` §6, sem transcrição manual. O verificador
lê o bloco da gramática diretamente do arquivo e confirma que ele é **byte a byte igual** ao do commit
`a712477` (Fase C). Totais: **91 produções · 50 não-terminais · 49 terminais**; todos os terminais
usados pertencem ao contrato léxico e todos os 49 são usados; `MENOS` aparece só em P62.

## 4. Nullable — LL1-01

**Definição.** `nullable(A)` é verdadeiro se, e somente se, `A ⇒* ε`.

**Algoritmo (ponto fixo).**

1. Marque `A` se existe `A -> ε`.
2. Marque `A` se existe `A -> X1 … Xn` em que **todos** os `Xi` são não-terminais já marcados
   (um terminal nunca é anulável).
3. Repita até uma rodada não marcar nada.

**Execução.** Rodada 1: a regra 1 marca 17 não-terminais (os que têm produção ε). Rodada 2: a regra 2
não marca nenhum outro, porque toda produção não vazia da gramática contém pelo menos um terminal ou um
não-terminal não anulável (a coluna "Motivo" abaixo mostra qual). Fim: **17 anuláveis**, que coincide com
o número declarado na Fase C, **obtido de forma independente**.

| # | Não-terminal | Nullable? | Motivo |
|---|---|---|---|
| 1 | `<programa>` | não | nenhuma alternativa anulável: P01 tem `ALGORITMO` (terminal) |
| 2 | `<corpo_programa>` | não | nenhuma alternativa anulável: P02 tem `<secao_var>` (não anulável); P03 tem `<rotina>` (não anulável) |
| 3 | `<secao_var>` | não | nenhuma alternativa anulável: P04 tem `VAR` (terminal) |
| 4 | `<secao_var_opcional>` | **sim** | regra 1: produção ε P06 |
| 5 | `<bloco>` | não | nenhuma alternativa anulável: P07 tem `INICIO` (terminal) |
| 6 | `<lista_declaracoes>` | **sim** | regra 1: produção ε P09 |
| 7 | `<declaracao>` | não | nenhuma alternativa anulável: P10 tem `<lista_ids>` (não anulável) |
| 8 | `<lista_ids>` | não | nenhuma alternativa anulável: P11 tem `ID` (terminal) |
| 9 | `<lista_ids_cauda>` | **sim** | regra 1: produção ε P13 |
| 10 | `<tipo_decl>` | não | nenhuma alternativa anulável: P14 tem `<tipo_simples>` (não anulável); P15 tem `<tipo_vetor>` (não anulável) |
| 11 | `<tipo_vetor>` | não | nenhuma alternativa anulável: P16 tem `VETOR` (terminal) |
| 12 | `<tipo_simples>` | não | nenhuma alternativa anulável: P17 tem `INTEIRO` (terminal); P18 tem `REAL` (terminal); P19 tem `CARACTERE` (terminal); P20 tem `LOGICO` (terminal) |
| 13 | `<lista_rotinas>` | **sim** | regra 1: produção ε P22 |
| 14 | `<rotina>` | não | nenhuma alternativa anulável: P23 tem `<procedimento>` (não anulável); P24 tem `<funcao>` (não anulável) |
| 15 | `<procedimento>` | não | nenhuma alternativa anulável: P25 tem `PROCEDIMENTO` (terminal) |
| 16 | `<parametros_procedimento>` | **sim** | regra 1: produção ε P27 |
| 17 | `<funcao>` | não | nenhuma alternativa anulável: P28 tem `FUNCAO` (terminal) |
| 18 | `<lista_parametros>` | não | nenhuma alternativa anulável: P29 tem `<parametro>` (não anulável) |
| 19 | `<lista_parametros_cauda>` | **sim** | regra 1: produção ε P31 |
| 20 | `<parametro>` | não | nenhuma alternativa anulável: P32 tem `ID` (terminal) |
| 21 | `<lista_comandos>` | não | nenhuma alternativa anulável: P33 tem `<comando>` (não anulável) |
| 22 | `<lista_comandos_cauda>` | **sim** | regra 1: produção ε P35 |
| 23 | `<comando>` | não | nenhuma alternativa anulável: P36 tem `<cmd_id>` (não anulável); P37 tem `<cmd_leia>` (não anulável); P38 tem `<cmd_escreva>` (não anulável); P39 tem `<cmd_escreval>` (não anulável); P40 tem `<cmd_se>` (não anulável); P41 tem `<cmd_para>` (não anulável); P42 tem `<cmd_enquanto>` (não anulável); P43 tem `<cmd_retorne>` (não anulável) |
| 24 | `<cmd_id>` | não | nenhuma alternativa anulável: P44 tem `ID` (terminal) |
| 25 | `<cauda_comando_id>` | **sim** | regra 1: produção ε P48 |
| 26 | `<cmd_leia>` | não | nenhuma alternativa anulável: P49 tem `LEIA` (terminal) |
| 27 | `<referencia>` | não | nenhuma alternativa anulável: P50 tem `ID` (terminal) |
| 28 | `<indice_opcional>` | **sim** | regra 1: produção ε P52 |
| 29 | `<cmd_escreva>` | não | nenhuma alternativa anulável: P53 tem `ESCREVA` (terminal) |
| 30 | `<cmd_escreval>` | não | nenhuma alternativa anulável: P54 tem `ESCREVAL` (terminal) |
| 31 | `<cmd_se>` | não | nenhuma alternativa anulável: P55 tem `SE` (terminal) |
| 32 | `<senao_opcional>` | **sim** | regra 1: produção ε P57 |
| 33 | `<cmd_para>` | não | nenhuma alternativa anulável: P58 tem `PARA` (terminal) |
| 34 | `<passo_opcional>` | **sim** | regra 1: produção ε P60 |
| 35 | `<numero_passo>` | não | nenhuma alternativa anulável: P61 tem `NUM_INT` (terminal); P62 tem `MENOS` (terminal) |
| 36 | `<cmd_enquanto>` | não | nenhuma alternativa anulável: P63 tem `ENQUANTO` (terminal) |
| 37 | `<cmd_retorne>` | não | nenhuma alternativa anulável: P64 tem `RETORNE` (terminal) |
| 38 | `<lista_argumentos>` | não | nenhuma alternativa anulável: P65 tem `<expressao>` (não anulável) |
| 39 | `<lista_argumentos_cauda>` | **sim** | regra 1: produção ε P67 |
| 40 | `<expressao>` | não | nenhuma alternativa anulável: P68 tem `<expr_logica>` (não anulável) |
| 41 | `<expr_logica>` | não | nenhuma alternativa anulável: P69 tem `<expr_relacional>` (não anulável) |
| 42 | `<expr_logica_cauda>` | **sim** | regra 1: produção ε P72 |
| 43 | `<expr_relacional>` | não | nenhuma alternativa anulável: P73 tem `<expr_aditiva>` (não anulável) |
| 44 | `<expr_rel_cauda>` | **sim** | regra 1: produção ε P75 |
| 45 | `<expr_aditiva>` | não | nenhuma alternativa anulável: P76 tem `<expr_mult>` (não anulável) |
| 46 | `<expr_aditiva_cauda>` | **sim** | regra 1: produção ε P78 |
| 47 | `<expr_mult>` | não | nenhuma alternativa anulável: P79 tem `<primario>` (não anulável) |
| 48 | `<expr_mult_cauda>` | **sim** | regra 1: produção ε P81 |
| 49 | `<primario>` | não | nenhuma alternativa anulável: P82 tem `ID` (terminal); P83 tem `NUM_INT` (terminal); P84 tem `NUM_REAL` (terminal); P85 tem `STRING` (terminal); P86 tem `VERDADEIRO` (terminal); P87 tem `FALSO` (terminal); P88 tem `ABRE_PAR` (terminal) |
| 50 | `<cauda_primario>` | **sim** | regra 1: produção ε P91 |

### 4.1 Auditoria das produções ε (as 17)

Comparadas com P01–P91, as produções ε são exatamente P06, P09, P13, P22, P27, P31, P35, P48, P52, P57,
P60, P67, P72, P75, P78, P81 e P91: nenhuma faltando, nenhuma a mais. Cada não-terminal anulável tem
**uma única** produção ε. O SELECT de uma produção ε é sempre o FOLLOW do seu lado esquerdo:

| Produção | Não-terminal | SELECT = FOLLOW(não-terminal) |
|---|---|---|
| P06 | `<secao_var_opcional>` | `INICIO` |
| P09 | `<lista_declaracoes>` | `INICIO` |
| P13 | `<lista_ids_cauda>` | `DOIS_PONTOS` |
| P22 | `<lista_rotinas>` | `VAR` `INICIO` |
| P27 | `<parametros_procedimento>` | `INICIO` |
| P31 | `<lista_parametros_cauda>` | `FECHA_PAR` |
| P35 | `<lista_comandos_cauda>` | `FIMALGORITMO` `SENAO` `FIMSE` `FIMPARA` `FIMENQUANTO` `FIMPROCEDIMENTO` `FIMFUNCAO` |
| P48 | `<cauda_comando_id>` | `ID` `FIMALGORITMO` `LEIA` `ESCREVA` `ESCREVAL` `SE` `SENAO` `FIMSE` `PARA` `FIMPARA` `ENQUANTO` `FIMENQUANTO` `FIMPROCEDIMENTO` `FIMFUNCAO` `RETORNE` |
| P52 | `<indice_opcional>` | `FECHA_PAR` |
| P57 | `<senao_opcional>` | `FIMSE` |
| P60 | `<passo_opcional>` | `FACA` |
| P67 | `<lista_argumentos_cauda>` | `FECHA_PAR` |
| P72 | `<expr_logica_cauda>` | `ID` `FIMALGORITMO` `LEIA` `ESCREVA` `ESCREVAL` `SE` `SENAO` `FIMSE` `PARA` `ATE` `PASSO` `FACA` `FIMPARA` `ENQUANTO` `FIMENQUANTO` `FIMPROCEDIMENTO` `FIMFUNCAO` `RETORNE` `FECHA_PAR` `FECHA_COL` `VIRGULA` |
| P75 | `<expr_rel_cauda>` | `ID` `FIMALGORITMO` `LEIA` `ESCREVA` `ESCREVAL` `SE` `SENAO` `FIMSE` `PARA` `ATE` `PASSO` `FACA` `FIMPARA` `ENQUANTO` `FIMENQUANTO` `FIMPROCEDIMENTO` `FIMFUNCAO` `RETORNE` `E` `OU` `FECHA_PAR` `FECHA_COL` `VIRGULA` |
| P78 | `<expr_aditiva_cauda>` | `ID` `FIMALGORITMO` `LEIA` `ESCREVA` `ESCREVAL` `SE` `SENAO` `FIMSE` `PARA` `ATE` `PASSO` `FACA` `FIMPARA` `ENQUANTO` `FIMENQUANTO` `FIMPROCEDIMENTO` `FIMFUNCAO` `RETORNE` `E` `OU` `OP_REL` `FECHA_PAR` `FECHA_COL` `VIRGULA` |
| P81 | `<expr_mult_cauda>` | `ID` `FIMALGORITMO` `LEIA` `ESCREVA` `ESCREVAL` `SE` `SENAO` `FIMSE` `PARA` `ATE` `PASSO` `FACA` `FIMPARA` `ENQUANTO` `FIMENQUANTO` `FIMPROCEDIMENTO` `FIMFUNCAO` `RETORNE` `E` `OU` `OP_REL` `MAIS` `FECHA_PAR` `FECHA_COL` `VIRGULA` |
| P91 | `<cauda_primario>` | `ID` `FIMALGORITMO` `LEIA` `ESCREVA` `ESCREVAL` `SE` `SENAO` `FIMSE` `PARA` `ATE` `PASSO` `FACA` `FIMPARA` `ENQUANTO` `FIMENQUANTO` `FIMPROCEDIMENTO` `FIMFUNCAO` `RETORNE` `E` `OU` `OP_REL` `OP_MULT` `MAIS` `FECHA_PAR` `FECHA_COL` `VIRGULA` |

## 5. FIRST — LL1-02

**Definição.**

- Terminal: `FIRST(t) = { t }`.
- Sequência `X1 X2 … Xn`: contém `FIRST(X1) − {ε}`; se `X1` é anulável, também `FIRST(X2) − {ε}`; e assim
  por diante; contém `ε` **somente** se todos os `Xi` são anuláveis (ou a sequência é vazia).
- Não-terminal: `FIRST(A)` = união dos FIRST dos lados direitos de todas as produções de `A`.

Calculado por ponto fixo sobre P01–P91.

| # | Não-terminal | FIRST |
|---|---|---|
| 1 | `<programa>` | `ALGORITMO` |
| 2 | `<corpo_programa>` | `VAR` `PROCEDIMENTO` `FUNCAO` |
| 3 | `<secao_var>` | `VAR` |
| 4 | `<secao_var_opcional>` | `VAR` `ε` |
| 5 | `<bloco>` | `INICIO` |
| 6 | `<lista_declaracoes>` | `ID` `ε` |
| 7 | `<declaracao>` | `ID` |
| 8 | `<lista_ids>` | `ID` |
| 9 | `<lista_ids_cauda>` | `VIRGULA` `ε` |
| 10 | `<tipo_decl>` | `INTEIRO` `REAL` `CARACTERE` `LOGICO` `VETOR` |
| 11 | `<tipo_vetor>` | `VETOR` |
| 12 | `<tipo_simples>` | `INTEIRO` `REAL` `CARACTERE` `LOGICO` |
| 13 | `<lista_rotinas>` | `PROCEDIMENTO` `FUNCAO` `ε` |
| 14 | `<rotina>` | `PROCEDIMENTO` `FUNCAO` |
| 15 | `<procedimento>` | `PROCEDIMENTO` |
| 16 | `<parametros_procedimento>` | `ABRE_PAR` `ε` |
| 17 | `<funcao>` | `FUNCAO` |
| 18 | `<lista_parametros>` | `ID` |
| 19 | `<lista_parametros_cauda>` | `VIRGULA` `ε` |
| 20 | `<parametro>` | `ID` |
| 21 | `<lista_comandos>` | `ID` `LEIA` `ESCREVA` `ESCREVAL` `SE` `PARA` `ENQUANTO` `RETORNE` |
| 22 | `<lista_comandos_cauda>` | `ID` `LEIA` `ESCREVA` `ESCREVAL` `SE` `PARA` `ENQUANTO` `RETORNE` `ε` |
| 23 | `<comando>` | `ID` `LEIA` `ESCREVA` `ESCREVAL` `SE` `PARA` `ENQUANTO` `RETORNE` |
| 24 | `<cmd_id>` | `ID` |
| 25 | `<cauda_comando_id>` | `ATRIBUICAO` `ABRE_PAR` `ABRE_COL` `ε` |
| 26 | `<cmd_leia>` | `LEIA` |
| 27 | `<referencia>` | `ID` |
| 28 | `<indice_opcional>` | `ABRE_COL` `ε` |
| 29 | `<cmd_escreva>` | `ESCREVA` |
| 30 | `<cmd_escreval>` | `ESCREVAL` |
| 31 | `<cmd_se>` | `SE` |
| 32 | `<senao_opcional>` | `SENAO` `ε` |
| 33 | `<cmd_para>` | `PARA` |
| 34 | `<passo_opcional>` | `PASSO` `ε` |
| 35 | `<numero_passo>` | `NUM_INT` `MENOS` |
| 36 | `<cmd_enquanto>` | `ENQUANTO` |
| 37 | `<cmd_retorne>` | `RETORNE` |
| 38 | `<lista_argumentos>` | `ID` `NUM_INT` `NUM_REAL` `STRING` `VERDADEIRO` `FALSO` `ABRE_PAR` |
| 39 | `<lista_argumentos_cauda>` | `VIRGULA` `ε` |
| 40 | `<expressao>` | `ID` `NUM_INT` `NUM_REAL` `STRING` `VERDADEIRO` `FALSO` `ABRE_PAR` |
| 41 | `<expr_logica>` | `ID` `NUM_INT` `NUM_REAL` `STRING` `VERDADEIRO` `FALSO` `ABRE_PAR` |
| 42 | `<expr_logica_cauda>` | `E` `OU` `ε` |
| 43 | `<expr_relacional>` | `ID` `NUM_INT` `NUM_REAL` `STRING` `VERDADEIRO` `FALSO` `ABRE_PAR` |
| 44 | `<expr_rel_cauda>` | `OP_REL` `ε` |
| 45 | `<expr_aditiva>` | `ID` `NUM_INT` `NUM_REAL` `STRING` `VERDADEIRO` `FALSO` `ABRE_PAR` |
| 46 | `<expr_aditiva_cauda>` | `MAIS` `ε` |
| 47 | `<expr_mult>` | `ID` `NUM_INT` `NUM_REAL` `STRING` `VERDADEIRO` `FALSO` `ABRE_PAR` |
| 48 | `<expr_mult_cauda>` | `OP_MULT` `ε` |
| 49 | `<primario>` | `ID` `NUM_INT` `NUM_REAL` `STRING` `VERDADEIRO` `FALSO` `ABRE_PAR` |
| 50 | `<cauda_primario>` | `ABRE_PAR` `ABRE_COL` `ε` |

### 5.1 Sanity checks (conferidos contra o cálculo, não usados no lugar dele)

| Verificação | Esperado | Obtido |
|---|---|---|
| `FIRST(<programa>)` | só `ALGORITMO` | `ALGORITMO` ✔ |
| `FIRST(<corpo_programa>)` | `VAR` `PROCEDIMENTO` `FUNCAO` | idem ✔ |
| `FIRST(<comando>)` | `ID LEIA ESCREVA ESCREVAL SE PARA ENQUANTO RETORNE` | idem ✔ |
| `FIRST(<expressao>)` = FIRST de um primário | `ID NUM_INT NUM_REAL STRING VERDADEIRO FALSO ABRE_PAR` | idem ✔ (vale também para `<expr_logica>`, `<expr_relacional>`, `<expr_aditiva>`, `<expr_mult>`, `<primario>`, `<lista_argumentos>`) |
| `FIRST(<cauda_comando_id>)` | `ATRIBUICAO ABRE_COL ABRE_PAR ε` | idem ✔ |
| `FIRST(<cauda_primario>)` | `ABRE_COL ABRE_PAR ε` | idem ✔ |
| `FIRST(<numero_passo>)` | `NUM_INT MENOS` | idem ✔ |

`MENOS` aparece em um único FIRST (`<numero_passo>`): confirma que não há sinal nem subtração em
expressão (DEC-33, DEC-34).

## 6. FOLLOW — LL1-03

**Regras.**

1. `$ ∈ FOLLOW(<programa>)`.
2. Para cada produção `A -> α B β`: acrescente `FIRST(β) − {ε}` a `FOLLOW(B)`.
3. Se `β ⇒* ε` (ou `B` está no fim da produção): acrescente `FOLLOW(A)` a `FOLLOW(B)`.

Repetido até nenhum conjunto mudar.

| # | Não-terminal | FOLLOW | Tam. |
|---|---|---|---|
| 1 | `<programa>` | `$` | 1 |
| 2 | `<corpo_programa>` | `FIMALGORITMO` | 1 |
| 3 | `<secao_var>` | `INICIO` | 1 |
| 4 | `<secao_var_opcional>` | `INICIO` | 1 |
| 5 | `<bloco>` | `FIMALGORITMO` `FIMPROCEDIMENTO` `FIMFUNCAO` | 3 |
| 6 | `<lista_declaracoes>` | `INICIO` | 1 |
| 7 | `<declaracao>` | `ID` `INICIO` | 2 |
| 8 | `<lista_ids>` | `DOIS_PONTOS` | 1 |
| 9 | `<lista_ids_cauda>` | `DOIS_PONTOS` | 1 |
| 10 | `<tipo_decl>` | `ID` `INICIO` | 2 |
| 11 | `<tipo_vetor>` | `ID` `INICIO` | 2 |
| 12 | `<tipo_simples>` | `ID` `INICIO` `FECHA_PAR` `VIRGULA` | 4 |
| 13 | `<lista_rotinas>` | `VAR` `INICIO` | 2 |
| 14 | `<rotina>` | `VAR` `INICIO` `PROCEDIMENTO` `FUNCAO` | 4 |
| 15 | `<procedimento>` | `VAR` `INICIO` `PROCEDIMENTO` `FUNCAO` | 4 |
| 16 | `<parametros_procedimento>` | `INICIO` | 1 |
| 17 | `<funcao>` | `VAR` `INICIO` `PROCEDIMENTO` `FUNCAO` | 4 |
| 18 | `<lista_parametros>` | `FECHA_PAR` | 1 |
| 19 | `<lista_parametros_cauda>` | `FECHA_PAR` | 1 |
| 20 | `<parametro>` | `FECHA_PAR` `VIRGULA` | 2 |
| 21 | `<lista_comandos>` | `FIMALGORITMO` `SENAO` `FIMSE` `FIMPARA` `FIMENQUANTO` `FIMPROCEDIMENTO` `FIMFUNCAO` | 7 |
| 22 | `<lista_comandos_cauda>` | `FIMALGORITMO` `SENAO` `FIMSE` `FIMPARA` `FIMENQUANTO` `FIMPROCEDIMENTO` `FIMFUNCAO` | 7 |
| 23 | `<comando>` | `ID` `FIMALGORITMO` `LEIA` `ESCREVA` `ESCREVAL` `SE` `SENAO` `FIMSE` `PARA` `FIMPARA` `ENQUANTO` `FIMENQUANTO` `FIMPROCEDIMENTO` `FIMFUNCAO` `RETORNE` | 15 |
| 24 | `<cmd_id>` | `ID` `FIMALGORITMO` `LEIA` `ESCREVA` `ESCREVAL` `SE` `SENAO` `FIMSE` `PARA` `FIMPARA` `ENQUANTO` `FIMENQUANTO` `FIMPROCEDIMENTO` `FIMFUNCAO` `RETORNE` | 15 |
| 25 | `<cauda_comando_id>` | `ID` `FIMALGORITMO` `LEIA` `ESCREVA` `ESCREVAL` `SE` `SENAO` `FIMSE` `PARA` `FIMPARA` `ENQUANTO` `FIMENQUANTO` `FIMPROCEDIMENTO` `FIMFUNCAO` `RETORNE` | 15 |
| 26 | `<cmd_leia>` | `ID` `FIMALGORITMO` `LEIA` `ESCREVA` `ESCREVAL` `SE` `SENAO` `FIMSE` `PARA` `FIMPARA` `ENQUANTO` `FIMENQUANTO` `FIMPROCEDIMENTO` `FIMFUNCAO` `RETORNE` | 15 |
| 27 | `<referencia>` | `FECHA_PAR` | 1 |
| 28 | `<indice_opcional>` | `FECHA_PAR` | 1 |
| 29 | `<cmd_escreva>` | `ID` `FIMALGORITMO` `LEIA` `ESCREVA` `ESCREVAL` `SE` `SENAO` `FIMSE` `PARA` `FIMPARA` `ENQUANTO` `FIMENQUANTO` `FIMPROCEDIMENTO` `FIMFUNCAO` `RETORNE` | 15 |
| 30 | `<cmd_escreval>` | `ID` `FIMALGORITMO` `LEIA` `ESCREVA` `ESCREVAL` `SE` `SENAO` `FIMSE` `PARA` `FIMPARA` `ENQUANTO` `FIMENQUANTO` `FIMPROCEDIMENTO` `FIMFUNCAO` `RETORNE` | 15 |
| 31 | `<cmd_se>` | `ID` `FIMALGORITMO` `LEIA` `ESCREVA` `ESCREVAL` `SE` `SENAO` `FIMSE` `PARA` `FIMPARA` `ENQUANTO` `FIMENQUANTO` `FIMPROCEDIMENTO` `FIMFUNCAO` `RETORNE` | 15 |
| 32 | `<senao_opcional>` | `FIMSE` | 1 |
| 33 | `<cmd_para>` | `ID` `FIMALGORITMO` `LEIA` `ESCREVA` `ESCREVAL` `SE` `SENAO` `FIMSE` `PARA` `FIMPARA` `ENQUANTO` `FIMENQUANTO` `FIMPROCEDIMENTO` `FIMFUNCAO` `RETORNE` | 15 |
| 34 | `<passo_opcional>` | `FACA` | 1 |
| 35 | `<numero_passo>` | `FACA` | 1 |
| 36 | `<cmd_enquanto>` | `ID` `FIMALGORITMO` `LEIA` `ESCREVA` `ESCREVAL` `SE` `SENAO` `FIMSE` `PARA` `FIMPARA` `ENQUANTO` `FIMENQUANTO` `FIMPROCEDIMENTO` `FIMFUNCAO` `RETORNE` | 15 |
| 37 | `<cmd_retorne>` | `ID` `FIMALGORITMO` `LEIA` `ESCREVA` `ESCREVAL` `SE` `SENAO` `FIMSE` `PARA` `FIMPARA` `ENQUANTO` `FIMENQUANTO` `FIMPROCEDIMENTO` `FIMFUNCAO` `RETORNE` | 15 |
| 38 | `<lista_argumentos>` | `FECHA_PAR` | 1 |
| 39 | `<lista_argumentos_cauda>` | `FECHA_PAR` | 1 |
| 40 | `<expressao>` | `ID` `FIMALGORITMO` `LEIA` `ESCREVA` `ESCREVAL` `SE` `SENAO` `FIMSE` `PARA` `ATE` `PASSO` `FACA` `FIMPARA` `ENQUANTO` `FIMENQUANTO` `FIMPROCEDIMENTO` `FIMFUNCAO` `RETORNE` `FECHA_PAR` `FECHA_COL` `VIRGULA` | 21 |
| 41 | `<expr_logica>` | `ID` `FIMALGORITMO` `LEIA` `ESCREVA` `ESCREVAL` `SE` `SENAO` `FIMSE` `PARA` `ATE` `PASSO` `FACA` `FIMPARA` `ENQUANTO` `FIMENQUANTO` `FIMPROCEDIMENTO` `FIMFUNCAO` `RETORNE` `FECHA_PAR` `FECHA_COL` `VIRGULA` | 21 |
| 42 | `<expr_logica_cauda>` | `ID` `FIMALGORITMO` `LEIA` `ESCREVA` `ESCREVAL` `SE` `SENAO` `FIMSE` `PARA` `ATE` `PASSO` `FACA` `FIMPARA` `ENQUANTO` `FIMENQUANTO` `FIMPROCEDIMENTO` `FIMFUNCAO` `RETORNE` `FECHA_PAR` `FECHA_COL` `VIRGULA` | 21 |
| 43 | `<expr_relacional>` | `ID` `FIMALGORITMO` `LEIA` `ESCREVA` `ESCREVAL` `SE` `SENAO` `FIMSE` `PARA` `ATE` `PASSO` `FACA` `FIMPARA` `ENQUANTO` `FIMENQUANTO` `FIMPROCEDIMENTO` `FIMFUNCAO` `RETORNE` `E` `OU` `FECHA_PAR` `FECHA_COL` `VIRGULA` | 23 |
| 44 | `<expr_rel_cauda>` | `ID` `FIMALGORITMO` `LEIA` `ESCREVA` `ESCREVAL` `SE` `SENAO` `FIMSE` `PARA` `ATE` `PASSO` `FACA` `FIMPARA` `ENQUANTO` `FIMENQUANTO` `FIMPROCEDIMENTO` `FIMFUNCAO` `RETORNE` `E` `OU` `FECHA_PAR` `FECHA_COL` `VIRGULA` | 23 |
| 45 | `<expr_aditiva>` | `ID` `FIMALGORITMO` `LEIA` `ESCREVA` `ESCREVAL` `SE` `SENAO` `FIMSE` `PARA` `ATE` `PASSO` `FACA` `FIMPARA` `ENQUANTO` `FIMENQUANTO` `FIMPROCEDIMENTO` `FIMFUNCAO` `RETORNE` `E` `OU` `OP_REL` `FECHA_PAR` `FECHA_COL` `VIRGULA` | 24 |
| 46 | `<expr_aditiva_cauda>` | `ID` `FIMALGORITMO` `LEIA` `ESCREVA` `ESCREVAL` `SE` `SENAO` `FIMSE` `PARA` `ATE` `PASSO` `FACA` `FIMPARA` `ENQUANTO` `FIMENQUANTO` `FIMPROCEDIMENTO` `FIMFUNCAO` `RETORNE` `E` `OU` `OP_REL` `FECHA_PAR` `FECHA_COL` `VIRGULA` | 24 |
| 47 | `<expr_mult>` | `ID` `FIMALGORITMO` `LEIA` `ESCREVA` `ESCREVAL` `SE` `SENAO` `FIMSE` `PARA` `ATE` `PASSO` `FACA` `FIMPARA` `ENQUANTO` `FIMENQUANTO` `FIMPROCEDIMENTO` `FIMFUNCAO` `RETORNE` `E` `OU` `OP_REL` `MAIS` `FECHA_PAR` `FECHA_COL` `VIRGULA` | 25 |
| 48 | `<expr_mult_cauda>` | `ID` `FIMALGORITMO` `LEIA` `ESCREVA` `ESCREVAL` `SE` `SENAO` `FIMSE` `PARA` `ATE` `PASSO` `FACA` `FIMPARA` `ENQUANTO` `FIMENQUANTO` `FIMPROCEDIMENTO` `FIMFUNCAO` `RETORNE` `E` `OU` `OP_REL` `MAIS` `FECHA_PAR` `FECHA_COL` `VIRGULA` | 25 |
| 49 | `<primario>` | `ID` `FIMALGORITMO` `LEIA` `ESCREVA` `ESCREVAL` `SE` `SENAO` `FIMSE` `PARA` `ATE` `PASSO` `FACA` `FIMPARA` `ENQUANTO` `FIMENQUANTO` `FIMPROCEDIMENTO` `FIMFUNCAO` `RETORNE` `E` `OU` `OP_REL` `OP_MULT` `MAIS` `FECHA_PAR` `FECHA_COL` `VIRGULA` | 26 |
| 50 | `<cauda_primario>` | `ID` `FIMALGORITMO` `LEIA` `ESCREVA` `ESCREVAL` `SE` `SENAO` `FIMSE` `PARA` `ATE` `PASSO` `FACA` `FIMPARA` `ENQUANTO` `FIMENQUANTO` `FIMPROCEDIMENTO` `FIMFUNCAO` `RETORNE` `E` `OU` `OP_REL` `OP_MULT` `MAIS` `FECHA_PAR` `FECHA_COL` `VIRGULA` | 26 |

### 6.1 Derivação dos FOLLOW críticos

Para facilitar a leitura, três conjuntos recorrentes recebem nome (as tabelas acima trazem os conjuntos
**completos**):

- **C** = FIRST(`<comando>`) = `ID LEIA ESCREVA ESCREVAL SE PARA ENQUANTO RETORNE` (8)
- **F** = FOLLOW(`<lista_comandos>`) = `FIMALGORITMO SENAO FIMSE FIMPARA FIMENQUANTO FIMPROCEDIMENTO FIMFUNCAO` (7)
- **X** = FOLLOW(`<expressao>`) = **C ∪ F** ∪ `FECHA_PAR FECHA_COL VIRGULA ATE PASSO FACA` (21)

**FOLLOW(`<lista_comandos>`) = F**, derivado produção a produção (e não por intuição):

| Ocorrência de `<lista_comandos>` | Regra | Contribui |
|---|---|---|
| P07 `<bloco> -> INICIO <lista_comandos>` (no fim) | 3 | FOLLOW(`<bloco>`) = `FIMALGORITMO` (de P02/P03 → FOLLOW(`<corpo_programa>`), que vem de P01) ∪ `FIMPROCEDIMENTO` (P25) ∪ `FIMFUNCAO` (P28) |
| P55 `… ENTAO <lista_comandos> <senao_opcional> FIMSE` | 2 | FIRST(`<senao_opcional> FIMSE`) − ε = `SENAO FIMSE` |
| P56 `<senao_opcional> -> SENAO <lista_comandos>` (no fim) | 3 | FOLLOW(`<senao_opcional>`) = `FIMSE` (de P55) |
| P58 `… FACA <lista_comandos> FIMPARA` | 2 | `FIMPARA` |
| P63 `… FACA <lista_comandos> FIMENQUANTO` | 2 | `FIMENQUANTO` |

Como `<lista_comandos_cauda>` está no fim de P33 e de P34, **FOLLOW(`<lista_comandos_cauda>`) = F**.
FOLLOW(`<comando>`) = FIRST(`<lista_comandos_cauda>`) − ε ∪ FOLLOW(`<lista_comandos_cauda>`) = **C ∪ F**
(P33/P34, regras 2 e 3), e o mesmo vale para os oito `<cmd_…>` e para `<cauda_comando_id>`.

**FOLLOW(`<expressao>`) = X**, de todas as ocorrências:

| Ocorrência | Contribui |
|---|---|
| P45 (fim), P46 (fim), P64 (fim) | FOLLOW(`<cauda_comando_id>`) ∪ FOLLOW(`<cmd_retorne>`) = C ∪ F |
| P46 (`… <expressao> FECHA_COL …`), P51, P89 | `FECHA_COL` |
| P55, P63, P88 | `FECHA_PAR` |
| P58 (`DE <expressao> ATE`) | `ATE` |
| P58 (`ATE <expressao> <passo_opcional> FACA`) | FIRST(`<passo_opcional> FACA`) − ε = `PASSO FACA` |
| P65, P66 (`<expressao> <lista_argumentos_cauda>`) | `VIRGULA` ∪ FOLLOW(`<lista_argumentos>`) = `VIRGULA FECHA_PAR` |

Os níveis de expressão acumulam para baixo: FOLLOW(`<expr_relacional>`) = X ∪ `E OU`;
FOLLOW(`<expr_aditiva>`) = X ∪ `E OU OP_REL`; FOLLOW(`<expr_mult>`) = X ∪ `E OU OP_REL MAIS`;
FOLLOW(`<primario>`) = X ∪ `E OU OP_REL MAIS OP_MULT`. Cada cauda tem o FOLLOW do seu nível.

## 7. SELECT de cada produção — LL1-04

**Definição** (também chamado PREDICT):

- se `ε ∉ FIRST(α)`: `SELECT(A -> α) = FIRST(α)`;
- se `ε ∈ FIRST(α)`: `SELECT(A -> α) = (FIRST(α) − {ε}) ∪ FOLLOW(A)`.

Na gramática, `ε ∈ FIRST(α)` acontece **apenas** nas 17 produções ε: nenhuma produção não vazia tem lado
direito anulável. Todas as 91 produções têm SELECT **não vazio**.

| Produção | Lado esquerdo | Lado direito | FIRST(lado direito) | Usa FOLLOW? | SELECT |
|---|---|---|---|---|---|
| P01 | `<programa>` | `ALGORITMO STRING <corpo_programa> FIMALGORITMO` | `ALGORITMO` | não | `ALGORITMO` |
| P02 | `<corpo_programa>` | `<secao_var> <bloco>` | `VAR` | não | `VAR` |
| P03 | `<corpo_programa>` | `<rotina> <lista_rotinas> <secao_var_opcional> <bloco>` | `PROCEDIMENTO` `FUNCAO` | não | `PROCEDIMENTO` `FUNCAO` |
| P04 | `<secao_var>` | `VAR <lista_declaracoes>` | `VAR` | não | `VAR` |
| P05 | `<secao_var_opcional>` | `<secao_var>` | `VAR` | não | `VAR` |
| P06 | `<secao_var_opcional>` | `ε` | `ε` | sim | `INICIO` |
| P07 | `<bloco>` | `INICIO <lista_comandos>` | `INICIO` | não | `INICIO` |
| P08 | `<lista_declaracoes>` | `<declaracao> <lista_declaracoes>` | `ID` | não | `ID` |
| P09 | `<lista_declaracoes>` | `ε` | `ε` | sim | `INICIO` |
| P10 | `<declaracao>` | `<lista_ids> DOIS_PONTOS <tipo_decl>` | `ID` | não | `ID` |
| P11 | `<lista_ids>` | `ID <lista_ids_cauda>` | `ID` | não | `ID` |
| P12 | `<lista_ids_cauda>` | `VIRGULA ID <lista_ids_cauda>` | `VIRGULA` | não | `VIRGULA` |
| P13 | `<lista_ids_cauda>` | `ε` | `ε` | sim | `DOIS_PONTOS` |
| P14 | `<tipo_decl>` | `<tipo_simples>` | `INTEIRO` `REAL` `CARACTERE` `LOGICO` | não | `INTEIRO` `REAL` `CARACTERE` `LOGICO` |
| P15 | `<tipo_decl>` | `<tipo_vetor>` | `VETOR` | não | `VETOR` |
| P16 | `<tipo_vetor>` | `VETOR ABRE_COL NUM_INT INTERVALO NUM_INT FECHA_COL DE <tipo_simples>` | `VETOR` | não | `VETOR` |
| P17 | `<tipo_simples>` | `INTEIRO` | `INTEIRO` | não | `INTEIRO` |
| P18 | `<tipo_simples>` | `REAL` | `REAL` | não | `REAL` |
| P19 | `<tipo_simples>` | `CARACTERE` | `CARACTERE` | não | `CARACTERE` |
| P20 | `<tipo_simples>` | `LOGICO` | `LOGICO` | não | `LOGICO` |
| P21 | `<lista_rotinas>` | `<rotina> <lista_rotinas>` | `PROCEDIMENTO` `FUNCAO` | não | `PROCEDIMENTO` `FUNCAO` |
| P22 | `<lista_rotinas>` | `ε` | `ε` | sim | `VAR` `INICIO` |
| P23 | `<rotina>` | `<procedimento>` | `PROCEDIMENTO` | não | `PROCEDIMENTO` |
| P24 | `<rotina>` | `<funcao>` | `FUNCAO` | não | `FUNCAO` |
| P25 | `<procedimento>` | `PROCEDIMENTO ID <parametros_procedimento> <bloco> FIMPROCEDIMENTO` | `PROCEDIMENTO` | não | `PROCEDIMENTO` |
| P26 | `<parametros_procedimento>` | `ABRE_PAR <lista_parametros> FECHA_PAR` | `ABRE_PAR` | não | `ABRE_PAR` |
| P27 | `<parametros_procedimento>` | `ε` | `ε` | sim | `INICIO` |
| P28 | `<funcao>` | `FUNCAO ID ABRE_PAR <lista_parametros> FECHA_PAR DOIS_PONTOS <tipo_simples> <bloco> FIMFUNCAO` | `FUNCAO` | não | `FUNCAO` |
| P29 | `<lista_parametros>` | `<parametro> <lista_parametros_cauda>` | `ID` | não | `ID` |
| P30 | `<lista_parametros_cauda>` | `VIRGULA <parametro> <lista_parametros_cauda>` | `VIRGULA` | não | `VIRGULA` |
| P31 | `<lista_parametros_cauda>` | `ε` | `ε` | sim | `FECHA_PAR` |
| P32 | `<parametro>` | `ID DOIS_PONTOS <tipo_simples>` | `ID` | não | `ID` |
| P33 | `<lista_comandos>` | `<comando> <lista_comandos_cauda>` | `ID` `LEIA` `ESCREVA` `ESCREVAL` `SE` `PARA` `ENQUANTO` `RETORNE` | não | `ID` `LEIA` `ESCREVA` `ESCREVAL` `SE` `PARA` `ENQUANTO` `RETORNE` |
| P34 | `<lista_comandos_cauda>` | `<comando> <lista_comandos_cauda>` | `ID` `LEIA` `ESCREVA` `ESCREVAL` `SE` `PARA` `ENQUANTO` `RETORNE` | não | `ID` `LEIA` `ESCREVA` `ESCREVAL` `SE` `PARA` `ENQUANTO` `RETORNE` |
| P35 | `<lista_comandos_cauda>` | `ε` | `ε` | sim | `FIMALGORITMO` `SENAO` `FIMSE` `FIMPARA` `FIMENQUANTO` `FIMPROCEDIMENTO` `FIMFUNCAO` |
| P36 | `<comando>` | `<cmd_id>` | `ID` | não | `ID` |
| P37 | `<comando>` | `<cmd_leia>` | `LEIA` | não | `LEIA` |
| P38 | `<comando>` | `<cmd_escreva>` | `ESCREVA` | não | `ESCREVA` |
| P39 | `<comando>` | `<cmd_escreval>` | `ESCREVAL` | não | `ESCREVAL` |
| P40 | `<comando>` | `<cmd_se>` | `SE` | não | `SE` |
| P41 | `<comando>` | `<cmd_para>` | `PARA` | não | `PARA` |
| P42 | `<comando>` | `<cmd_enquanto>` | `ENQUANTO` | não | `ENQUANTO` |
| P43 | `<comando>` | `<cmd_retorne>` | `RETORNE` | não | `RETORNE` |
| P44 | `<cmd_id>` | `ID <cauda_comando_id>` | `ID` | não | `ID` |
| P45 | `<cauda_comando_id>` | `ATRIBUICAO <expressao>` | `ATRIBUICAO` | não | `ATRIBUICAO` |
| P46 | `<cauda_comando_id>` | `ABRE_COL <expressao> FECHA_COL ATRIBUICAO <expressao>` | `ABRE_COL` | não | `ABRE_COL` |
| P47 | `<cauda_comando_id>` | `ABRE_PAR <lista_argumentos> FECHA_PAR` | `ABRE_PAR` | não | `ABRE_PAR` |
| P48 | `<cauda_comando_id>` | `ε` | `ε` | sim | `ID` `FIMALGORITMO` `LEIA` `ESCREVA` `ESCREVAL` `SE` `SENAO` `FIMSE` `PARA` `FIMPARA` `ENQUANTO` `FIMENQUANTO` `FIMPROCEDIMENTO` `FIMFUNCAO` `RETORNE` |
| P49 | `<cmd_leia>` | `LEIA ABRE_PAR <referencia> FECHA_PAR` | `LEIA` | não | `LEIA` |
| P50 | `<referencia>` | `ID <indice_opcional>` | `ID` | não | `ID` |
| P51 | `<indice_opcional>` | `ABRE_COL <expressao> FECHA_COL` | `ABRE_COL` | não | `ABRE_COL` |
| P52 | `<indice_opcional>` | `ε` | `ε` | sim | `FECHA_PAR` |
| P53 | `<cmd_escreva>` | `ESCREVA ABRE_PAR <lista_argumentos> FECHA_PAR` | `ESCREVA` | não | `ESCREVA` |
| P54 | `<cmd_escreval>` | `ESCREVAL ABRE_PAR <lista_argumentos> FECHA_PAR` | `ESCREVAL` | não | `ESCREVAL` |
| P55 | `<cmd_se>` | `SE ABRE_PAR <expressao> FECHA_PAR ENTAO <lista_comandos> <senao_opcional> FIMSE` | `SE` | não | `SE` |
| P56 | `<senao_opcional>` | `SENAO <lista_comandos>` | `SENAO` | não | `SENAO` |
| P57 | `<senao_opcional>` | `ε` | `ε` | sim | `FIMSE` |
| P58 | `<cmd_para>` | `PARA ID DE <expressao> ATE <expressao> <passo_opcional> FACA <lista_comandos> FIMPARA` | `PARA` | não | `PARA` |
| P59 | `<passo_opcional>` | `PASSO <numero_passo>` | `PASSO` | não | `PASSO` |
| P60 | `<passo_opcional>` | `ε` | `ε` | sim | `FACA` |
| P61 | `<numero_passo>` | `NUM_INT` | `NUM_INT` | não | `NUM_INT` |
| P62 | `<numero_passo>` | `MENOS NUM_INT` | `MENOS` | não | `MENOS` |
| P63 | `<cmd_enquanto>` | `ENQUANTO ABRE_PAR <expressao> FECHA_PAR FACA <lista_comandos> FIMENQUANTO` | `ENQUANTO` | não | `ENQUANTO` |
| P64 | `<cmd_retorne>` | `RETORNE <expressao>` | `RETORNE` | não | `RETORNE` |
| P65 | `<lista_argumentos>` | `<expressao> <lista_argumentos_cauda>` | `ID` `NUM_INT` `NUM_REAL` `STRING` `VERDADEIRO` `FALSO` `ABRE_PAR` | não | `ID` `NUM_INT` `NUM_REAL` `STRING` `VERDADEIRO` `FALSO` `ABRE_PAR` |
| P66 | `<lista_argumentos_cauda>` | `VIRGULA <expressao> <lista_argumentos_cauda>` | `VIRGULA` | não | `VIRGULA` |
| P67 | `<lista_argumentos_cauda>` | `ε` | `ε` | sim | `FECHA_PAR` |
| P68 | `<expressao>` | `<expr_logica>` | `ID` `NUM_INT` `NUM_REAL` `STRING` `VERDADEIRO` `FALSO` `ABRE_PAR` | não | `ID` `NUM_INT` `NUM_REAL` `STRING` `VERDADEIRO` `FALSO` `ABRE_PAR` |
| P69 | `<expr_logica>` | `<expr_relacional> <expr_logica_cauda>` | `ID` `NUM_INT` `NUM_REAL` `STRING` `VERDADEIRO` `FALSO` `ABRE_PAR` | não | `ID` `NUM_INT` `NUM_REAL` `STRING` `VERDADEIRO` `FALSO` `ABRE_PAR` |
| P70 | `<expr_logica_cauda>` | `E <expr_relacional> <expr_logica_cauda>` | `E` | não | `E` |
| P71 | `<expr_logica_cauda>` | `OU <expr_relacional> <expr_logica_cauda>` | `OU` | não | `OU` |
| P72 | `<expr_logica_cauda>` | `ε` | `ε` | sim | `ID` `FIMALGORITMO` `LEIA` `ESCREVA` `ESCREVAL` `SE` `SENAO` `FIMSE` `PARA` `ATE` `PASSO` `FACA` `FIMPARA` `ENQUANTO` `FIMENQUANTO` `FIMPROCEDIMENTO` `FIMFUNCAO` `RETORNE` `FECHA_PAR` `FECHA_COL` `VIRGULA` |
| P73 | `<expr_relacional>` | `<expr_aditiva> <expr_rel_cauda>` | `ID` `NUM_INT` `NUM_REAL` `STRING` `VERDADEIRO` `FALSO` `ABRE_PAR` | não | `ID` `NUM_INT` `NUM_REAL` `STRING` `VERDADEIRO` `FALSO` `ABRE_PAR` |
| P74 | `<expr_rel_cauda>` | `OP_REL <expr_aditiva>` | `OP_REL` | não | `OP_REL` |
| P75 | `<expr_rel_cauda>` | `ε` | `ε` | sim | `ID` `FIMALGORITMO` `LEIA` `ESCREVA` `ESCREVAL` `SE` `SENAO` `FIMSE` `PARA` `ATE` `PASSO` `FACA` `FIMPARA` `ENQUANTO` `FIMENQUANTO` `FIMPROCEDIMENTO` `FIMFUNCAO` `RETORNE` `E` `OU` `FECHA_PAR` `FECHA_COL` `VIRGULA` |
| P76 | `<expr_aditiva>` | `<expr_mult> <expr_aditiva_cauda>` | `ID` `NUM_INT` `NUM_REAL` `STRING` `VERDADEIRO` `FALSO` `ABRE_PAR` | não | `ID` `NUM_INT` `NUM_REAL` `STRING` `VERDADEIRO` `FALSO` `ABRE_PAR` |
| P77 | `<expr_aditiva_cauda>` | `MAIS <expr_mult> <expr_aditiva_cauda>` | `MAIS` | não | `MAIS` |
| P78 | `<expr_aditiva_cauda>` | `ε` | `ε` | sim | `ID` `FIMALGORITMO` `LEIA` `ESCREVA` `ESCREVAL` `SE` `SENAO` `FIMSE` `PARA` `ATE` `PASSO` `FACA` `FIMPARA` `ENQUANTO` `FIMENQUANTO` `FIMPROCEDIMENTO` `FIMFUNCAO` `RETORNE` `E` `OU` `OP_REL` `FECHA_PAR` `FECHA_COL` `VIRGULA` |
| P79 | `<expr_mult>` | `<primario> <expr_mult_cauda>` | `ID` `NUM_INT` `NUM_REAL` `STRING` `VERDADEIRO` `FALSO` `ABRE_PAR` | não | `ID` `NUM_INT` `NUM_REAL` `STRING` `VERDADEIRO` `FALSO` `ABRE_PAR` |
| P80 | `<expr_mult_cauda>` | `OP_MULT <primario> <expr_mult_cauda>` | `OP_MULT` | não | `OP_MULT` |
| P81 | `<expr_mult_cauda>` | `ε` | `ε` | sim | `ID` `FIMALGORITMO` `LEIA` `ESCREVA` `ESCREVAL` `SE` `SENAO` `FIMSE` `PARA` `ATE` `PASSO` `FACA` `FIMPARA` `ENQUANTO` `FIMENQUANTO` `FIMPROCEDIMENTO` `FIMFUNCAO` `RETORNE` `E` `OU` `OP_REL` `MAIS` `FECHA_PAR` `FECHA_COL` `VIRGULA` |
| P82 | `<primario>` | `ID <cauda_primario>` | `ID` | não | `ID` |
| P83 | `<primario>` | `NUM_INT` | `NUM_INT` | não | `NUM_INT` |
| P84 | `<primario>` | `NUM_REAL` | `NUM_REAL` | não | `NUM_REAL` |
| P85 | `<primario>` | `STRING` | `STRING` | não | `STRING` |
| P86 | `<primario>` | `VERDADEIRO` | `VERDADEIRO` | não | `VERDADEIRO` |
| P87 | `<primario>` | `FALSO` | `FALSO` | não | `FALSO` |
| P88 | `<primario>` | `ABRE_PAR <expressao> FECHA_PAR` | `ABRE_PAR` | não | `ABRE_PAR` |
| P89 | `<cauda_primario>` | `ABRE_COL <expressao> FECHA_COL` | `ABRE_COL` | não | `ABRE_COL` |
| P90 | `<cauda_primario>` | `ABRE_PAR <lista_argumentos> FECHA_PAR` | `ABRE_PAR` | não | `ABRE_PAR` |
| P91 | `<cauda_primario>` | `ε` | `ε` | sim | `ID` `FIMALGORITMO` `LEIA` `ESCREVA` `ESCREVAL` `SE` `SENAO` `FIMSE` `PARA` `ATE` `PASSO` `FACA` `FIMPARA` `ENQUANTO` `FIMENQUANTO` `FIMPROCEDIMENTO` `FIMFUNCAO` `RETORNE` `E` `OU` `OP_REL` `OP_MULT` `MAIS` `FECHA_PAR` `FECHA_COL` `VIRGULA` |

## 8. Verificação de conflitos LL(1) — LL1-05

**Condição.** A gramática é LL(1) se, e somente se, para todo não-terminal `A` com produções
`A -> α1 | … | αn`, vale `SELECT(αi) ∩ SELECT(αj) = ∅` para todo `i ≠ j`.

**Prova por contagem (usada na coluna Σ).** Um conjunto de SELECTs é disjunto dois a dois se, e somente
se, a soma dos tamanhos é igual ao tamanho da união. A coluna "Σ|SELECT| = |∪|" mostra os dois números
para cada não-terminal; a coluna "Interseção" mostra o resultado da comparação explícita de **todos os
pares**.

24 não-terminais têm alternativas (os outros 26 têm uma única produção e não exigem decisão);
foram comparados **85 pares**.

| Não-terminal | Produções comparadas | SELECT de cada alternativa | Pares | Σ\|SELECT\| = \|∪\|? | Interseção SELECT | Resultado |
|---|---|---|---|---|---|---|
| `<corpo_programa>` | P02, P03 | P02: `VAR`<br>P03: `PROCEDIMENTO` `FUNCAO` | 1 | 3 = 3 | ∅ (todos os pares) | **sem conflito** |
| `<secao_var_opcional>` | P05, P06 | P05: `VAR`<br>P06: `INICIO` | 1 | 2 = 2 | ∅ (todos os pares) | **sem conflito** |
| `<lista_declaracoes>` | P08, P09 | P08: `ID`<br>P09: `INICIO` | 1 | 2 = 2 | ∅ (todos os pares) | **sem conflito** |
| `<lista_ids_cauda>` | P12, P13 | P12: `VIRGULA`<br>P13: `DOIS_PONTOS` | 1 | 2 = 2 | ∅ (todos os pares) | **sem conflito** |
| `<tipo_decl>` | P14, P15 | P14: `INTEIRO` `REAL` `CARACTERE` `LOGICO`<br>P15: `VETOR` | 1 | 5 = 5 | ∅ (todos os pares) | **sem conflito** |
| `<tipo_simples>` | P17, P18, P19, P20 | P17: `INTEIRO`<br>P18: `REAL`<br>P19: `CARACTERE`<br>P20: `LOGICO` | 6 | 4 = 4 | ∅ (todos os pares) | **sem conflito** |
| `<lista_rotinas>` | P21, P22 | P21: `PROCEDIMENTO` `FUNCAO`<br>P22: `VAR` `INICIO` | 1 | 4 = 4 | ∅ (todos os pares) | **sem conflito** |
| `<rotina>` | P23, P24 | P23: `PROCEDIMENTO`<br>P24: `FUNCAO` | 1 | 2 = 2 | ∅ (todos os pares) | **sem conflito** |
| `<parametros_procedimento>` | P26, P27 | P26: `ABRE_PAR`<br>P27: `INICIO` | 1 | 2 = 2 | ∅ (todos os pares) | **sem conflito** |
| `<lista_parametros_cauda>` | P30, P31 | P30: `VIRGULA`<br>P31: `FECHA_PAR` | 1 | 2 = 2 | ∅ (todos os pares) | **sem conflito** |
| `<lista_comandos_cauda>` | P34, P35 | P34: `ID` `LEIA` `ESCREVA` `ESCREVAL` `SE` `PARA` `ENQUANTO` `RETORNE`<br>P35: `FIMALGORITMO` `SENAO` `FIMSE` `FIMPARA` `FIMENQUANTO` `FIMPROCEDIMENTO` `FIMFUNCAO` | 1 | 15 = 15 | ∅ (todos os pares) | **sem conflito** |
| `<comando>` | P36, P37, P38, P39, P40, P41, P42, P43 | P36: `ID`<br>P37: `LEIA`<br>P38: `ESCREVA`<br>P39: `ESCREVAL`<br>P40: `SE`<br>P41: `PARA`<br>P42: `ENQUANTO`<br>P43: `RETORNE` | 28 | 8 = 8 | ∅ (todos os pares) | **sem conflito** |
| `<cauda_comando_id>` | P45, P46, P47, P48 | P45: `ATRIBUICAO`<br>P46: `ABRE_COL`<br>P47: `ABRE_PAR`<br>P48: `ID` `FIMALGORITMO` `LEIA` `ESCREVA` `ESCREVAL` `SE` `SENAO` `FIMSE` `PARA` `FIMPARA` `ENQUANTO` `FIMENQUANTO` `FIMPROCEDIMENTO` `FIMFUNCAO` `RETORNE` | 6 | 18 = 18 | ∅ (todos os pares) | **sem conflito** |
| `<indice_opcional>` | P51, P52 | P51: `ABRE_COL`<br>P52: `FECHA_PAR` | 1 | 2 = 2 | ∅ (todos os pares) | **sem conflito** |
| `<senao_opcional>` | P56, P57 | P56: `SENAO`<br>P57: `FIMSE` | 1 | 2 = 2 | ∅ (todos os pares) | **sem conflito** |
| `<passo_opcional>` | P59, P60 | P59: `PASSO`<br>P60: `FACA` | 1 | 2 = 2 | ∅ (todos os pares) | **sem conflito** |
| `<numero_passo>` | P61, P62 | P61: `NUM_INT`<br>P62: `MENOS` | 1 | 2 = 2 | ∅ (todos os pares) | **sem conflito** |
| `<lista_argumentos_cauda>` | P66, P67 | P66: `VIRGULA`<br>P67: `FECHA_PAR` | 1 | 2 = 2 | ∅ (todos os pares) | **sem conflito** |
| `<expr_logica_cauda>` | P70, P71, P72 | P70: `E`<br>P71: `OU`<br>P72: `ID` `FIMALGORITMO` `LEIA` `ESCREVA` `ESCREVAL` `SE` `SENAO` `FIMSE` `PARA` `ATE` `PASSO` `FACA` `FIMPARA` `ENQUANTO` `FIMENQUANTO` `FIMPROCEDIMENTO` `FIMFUNCAO` `RETORNE` `FECHA_PAR` `FECHA_COL` `VIRGULA` | 3 | 23 = 23 | ∅ (todos os pares) | **sem conflito** |
| `<expr_rel_cauda>` | P74, P75 | P74: `OP_REL`<br>P75: `ID` `FIMALGORITMO` `LEIA` `ESCREVA` `ESCREVAL` `SE` `SENAO` `FIMSE` `PARA` `ATE` `PASSO` `FACA` `FIMPARA` `ENQUANTO` `FIMENQUANTO` `FIMPROCEDIMENTO` `FIMFUNCAO` `RETORNE` `E` `OU` `FECHA_PAR` `FECHA_COL` `VIRGULA` | 1 | 24 = 24 | ∅ (todos os pares) | **sem conflito** |
| `<expr_aditiva_cauda>` | P77, P78 | P77: `MAIS`<br>P78: `ID` `FIMALGORITMO` `LEIA` `ESCREVA` `ESCREVAL` `SE` `SENAO` `FIMSE` `PARA` `ATE` `PASSO` `FACA` `FIMPARA` `ENQUANTO` `FIMENQUANTO` `FIMPROCEDIMENTO` `FIMFUNCAO` `RETORNE` `E` `OU` `OP_REL` `FECHA_PAR` `FECHA_COL` `VIRGULA` | 1 | 25 = 25 | ∅ (todos os pares) | **sem conflito** |
| `<expr_mult_cauda>` | P80, P81 | P80: `OP_MULT`<br>P81: `ID` `FIMALGORITMO` `LEIA` `ESCREVA` `ESCREVAL` `SE` `SENAO` `FIMSE` `PARA` `ATE` `PASSO` `FACA` `FIMPARA` `ENQUANTO` `FIMENQUANTO` `FIMPROCEDIMENTO` `FIMFUNCAO` `RETORNE` `E` `OU` `OP_REL` `MAIS` `FECHA_PAR` `FECHA_COL` `VIRGULA` | 1 | 26 = 26 | ∅ (todos os pares) | **sem conflito** |
| `<primario>` | P82, P83, P84, P85, P86, P87, P88 | P82: `ID`<br>P83: `NUM_INT`<br>P84: `NUM_REAL`<br>P85: `STRING`<br>P86: `VERDADEIRO`<br>P87: `FALSO`<br>P88: `ABRE_PAR` | 21 | 7 = 7 | ∅ (todos os pares) | **sem conflito** |
| `<cauda_primario>` | P89, P90, P91 | P89: `ABRE_COL`<br>P90: `ABRE_PAR`<br>P91: `ID` `FIMALGORITMO` `LEIA` `ESCREVA` `ESCREVAL` `SE` `SENAO` `FIMSE` `PARA` `ATE` `PASSO` `FACA` `FIMPARA` `ENQUANTO` `FIMENQUANTO` `FIMPROCEDIMENTO` `FIMFUNCAO` `RETORNE` `E` `OU` `OP_REL` `OP_MULT` `MAIS` `FECHA_PAR` `FECHA_COL` `VIRGULA` | 3 | 28 = 28 | ∅ (todos os pares) | **sem conflito** |

**Relatório de conflitos: vazio.** Nenhum par de alternativas compartilha um token de seleção.

## 9. Análises específicas

### 9.1 `<cauda_comando_id>` — LL1-06

```
P44 <cmd_id>           -> ID <cauda_comando_id>
P45 <cauda_comando_id> -> ATRIBUICAO <expressao>
P46                     | ABRE_COL <expressao> FECHA_COL ATRIBUICAO <expressao>
P47                     | ABRE_PAR <lista_argumentos> FECHA_PAR
P48                     | ε
```

| Alternativa | FIRST do lado direito | SELECT |
|---|---|---|
| P45 | `ATRIBUICAO` | `ATRIBUICAO` |
| P46 | `ABRE_COL` | `ABRE_COL` |
| P47 | `ABRE_PAR` | `ABRE_PAR` |
| P48 | `ε` | FOLLOW(`<cauda_comando_id>`) = **C ∪ F** = `ID FIMALGORITMO LEIA ESCREVA ESCREVAL SE SENAO FIMSE PARA FIMPARA ENQUANTO FIMENQUANTO FIMPROCEDIMENTO FIMFUNCAO RETORNE` |

Interseções: `{ATRIBUICAO, ABRE_COL, ABRE_PAR} ∩ (C ∪ F) = ∅`, e os três primeiros são distintos entre si.

**Por que um token basta.** Depois de consumir o `ID`, o lookahead é:

| Entrada | Lookahead após o `ID` | Produção |
|---|---|---|
| `x <- …` | `ATRIBUICAO` | P45 |
| `x[…] <- …` | `ABRE_COL` | P46 |
| `p(…)` | `ABRE_PAR` | P47 |
| `linha_decorativa` seguido de outro comando ou de um fechamento | algum token de **C ∪ F** | P48 |

A razão estrutural: **nenhum comando começa** com `ATRIBUICAO`, `ABRE_COL` ou `ABRE_PAR`, e nenhum
fechamento de bloco é um desses tokens.

### 9.2 Efeito da ausência de quebra de linha — LL1-07

O léxico descarta quebras de linha; os comandos são separados só pela estrutura.

**`linha_decorativa` + `escreval("x")`** vira `ID ESCREVAL ABRE_PAR STRING FECHA_PAR …`:

| Passo | Não-terminal | Lookahead | Escolha | Justificativa |
|---|---|---|---|---|
| 1 | `<comando>` | `ID` | P36 | `ID ∈ SELECT(P36)` |
| 2 | `<cmd_id>` | `ID` | P44 (consome `ID`) | única produção |
| 3 | `<cauda_comando_id>` | `ESCREVAL` | **P48 (ε)** | `ESCREVAL ∈ C ⊂ SELECT(P48)`; `ESCREVAL ∉ SELECT(P45..P47)` |
| 4 | `<lista_comandos_cauda>` | `ESCREVAL` | P34 | `ESCREVAL ∈ SELECT(P34) = C` |
| 5 | `<comando>` | `ESCREVAL` | P39 | `ESCREVAL ∈ SELECT(P39)` |

**`linha_decorativa` + `outro_procedimento`** vira `ID ID …`: no passo 3 o lookahead é `ID`, e
`ID ∈ C ⊂ SELECT(P48)` → ε encerra o primeiro comando; no passo 4, `ID ∈ SELECT(P34)` → novo comando;
no passo 5, `ID ∈ SELECT(P36)`. O primeiro `ID` termina por ε e o segundo inicia o próximo comando.

**Comando que termina em expressão** (`x <- a` + `b <- 1`, tokens `ID ATRIBUICAO ID ID ATRIBUICAO NUM_INT`):
depois do primário `a`, o lookahead `ID` está em SELECT(P91) (FOLLOW(`<cauda_primario>`) ⊇ C); em seguida
`ID` seleciona as caudas ε P81, P78, P75, P72 (todas têm C no FOLLOW), a expressão termina, e `ID`
inicia o próximo comando por P34/P36.

*Consequência a registrar (não é conflito):* como `ID` sozinho é um comando completo (P48), uma cadeia como
`x <- a b` é lida **sintaticamente** como dois comandos (`x <- a` e a chamada `b`). Isso é consistente com a
limitação já documentada em `gramatica.md` §16: a sintaxe verifica forma, não se `b` é procedimento.

### 9.3 `<cauda_primario>` — LL1-08

```
P82 <primario>       -> ID <cauda_primario>
P89 <cauda_primario> -> ABRE_COL <expressao> FECHA_COL
P90                   | ABRE_PAR <lista_argumentos> FECHA_PAR
P91                   | ε
```

SELECT(P89) = `ABRE_COL`; SELECT(P90) = `ABRE_PAR`; SELECT(P91) = FOLLOW(`<cauda_primario>`) =
X ∪ `E OU OP_REL MAIS OP_MULT` (26 tokens, listados na tabela do §7).

**`ABRE_COL` e `ABRE_PAR` não pertencem a FOLLOW(`<cauda_primario>`)**: X só tem `FECHA_PAR` e
`FECHA_COL` como parênteses/colchetes, e os operadores acrescentados também não são aberturas. Logo
`SELECT(P91) ∩ {ABRE_COL, ABRE_PAR} = ∅`, e com um token após o `ID` o parser distingue:

| Entrada | Lookahead após `ID` | Produção |
|---|---|---|
| `nome` | qualquer token de SELECT(P91) (operador, `)`, `]`, `,`, `ate`, `passo`, `faca`, início/fim de comando) | P91 |
| `notas[i]` | `ABRE_COL` | P89 |
| `somar(10, 5)` | `ABRE_PAR` | P90 |

### 9.4 As quatro caudas de expressão — LL1-09

| Cauda | Alternativa explícita (SELECT) | Alternativa ε: SELECT = FOLLOW | Interseção |
|---|---|---|---|
| `<expr_logica_cauda>` | P70 `E`; P71 `OU` | P72: X (21 tokens) | `{E} ∩ {OU} = ∅`; `{E, OU} ∩ X = ∅` |
| `<expr_rel_cauda>` | P74 `OP_REL` | P75: X ∪ `E OU` (23) | `{OP_REL} ∩ (X ∪ {E, OU}) = ∅` |
| `<expr_aditiva_cauda>` | P77 `MAIS` | P78: X ∪ `E OU OP_REL` (24) | `{MAIS} ∩ … = ∅` |
| `<expr_mult_cauda>` | P80 `OP_MULT` | P81: X ∪ `E OU OP_REL MAIS` (25) | `{OP_MULT} ∩ … = ∅` |

Cada operador está **só** no SELECT explícito do seu nível e **no FOLLOW dos níveis mais altos**, nunca do
mesmo nível: `OP_MULT` só em P80; `MAIS` em P77 e nos FOLLOW de `<expr_mult_cauda>`/`<primario>`; `OP_REL`
em P74 e nos FOLLOW dos níveis aditivo e multiplicativo; `E`/`OU` em P70/P71 e nos FOLLOW dos três níveis
abaixo. É exatamente a hierarquia da DEC-35, confirmada pelos conjuntos.

**Relacional não encadeável — LL1-17.** P74 termina em `<expr_aditiva>`, e não em `<expr_rel_cauda>`: a
cauda relacional não recorre. Em `a = b = c` (`ID OP_REL ID OP_REL ID`): depois de `a = b`,
`<expr_aditiva_cauda>` vê `OP_REL` ∈ SELECT(P78) → ε; `<expr_rel_cauda>` já foi consumida por P74; em
`<expr_logica_cauda>`, `OP_REL` ∉ SELECT(P70) ∪ SELECT(P71) ∪ SELECT(P72) → **erro sintático determinístico**.
`a = b` é aceito. A propriedade **não cria conflito**: `OP_REL ∉ FOLLOW(<expr_rel_cauda>)`.

### 9.5 `<lista_comandos_cauda>` — LL1-10

SELECT(P34) = FIRST(`<comando>`) = **C**; SELECT(P35) = FOLLOW(`<lista_comandos_cauda>`) = **F**.
C ∩ F = ∅: nenhum token que inicia comando é fechamento de bloco. Como não há separador de linha,
**esta disjunção é o que permite saber onde a lista termina**. Cada contexto recebe o seu fechamento:
`FIMALGORITMO`/`FIMPROCEDIMENTO`/`FIMFUNCAO` (via `<bloco>`), `SENAO`/`FIMSE` (em `se`), `FIMPARA`,
`FIMENQUANTO`.

*Observação:* F é a **união** dos fechamentos de todos os contextos (o FOLLOW não distingue o contexto).
Por exemplo, dentro de um `para`, um `FIMSE` perdido também seleciona P35; o erro aparece logo em seguida,
quando P58 exige `FIMPARA`. Isso não é conflito: é o comportamento normal de LL(1) com FOLLOW global.

### 9.6 `<lista_declaracoes>` — LL1-11

SELECT(P08) = FIRST(`<declaracao>`) = `ID`; SELECT(P09) = FOLLOW(`<lista_declaracoes>`) = `INICIO`
(vem de P04 → FOLLOW(`<secao_var>`) = `INICIO`, por P02 e P05/P03). `{ID} ∩ {INICIO} = ∅`: a seção de
variáveis termina exatamente quando aparece `INICIO`, com ou sem declarações (`var` vazio do
`PrimeiroPasso`).

### 9.7 `<lista_rotinas>` e `<secao_var_opcional>` — LL1-12

SELECT(P21) = FIRST(`<rotina>`) = `PROCEDIMENTO FUNCAO`; SELECT(P22) = FOLLOW(`<lista_rotinas>`) =
FIRST(`<secao_var_opcional> <bloco>`) = `VAR INICIO` (P03, regra 2; `<secao_var_opcional>` é anulável, por
isso `INICIO` também entra). Interseção vazia: as rotinas param quando aparece `VAR` ou `INICIO`.
Em seguida, `<secao_var_opcional>`: SELECT(P05) = `VAR`; SELECT(P06) = `INICIO`. Disjuntos.
Em `<corpo_programa>`: SELECT(P02) = `VAR`; SELECT(P03) = `PROCEDIMENTO FUNCAO`. Disjuntos.

### 9.8 `<parametros_procedimento>` e procedimento com/sem parâmetros — LL1-13

SELECT(P26) = `ABRE_PAR`; SELECT(P27) = FOLLOW(`<parametros_procedimento>`) = FIRST(`<bloco>`) = `INICIO`.
A decisão acontece **depois** de P25 consumir `PROCEDIMENTO ID`, com um único token:
`procedimento mostrar_erro(…)` → `ABRE_PAR` → P26; `procedimento linha_decorativa` `inicio` → `INICIO` →
P27. `procedimento p()` segue P26 e falha em `FECHA_PAR`, que não está em FIRST(`<lista_parametros>`) = `ID`.

### 9.9 `<senao_opcional>` — LL1-14

SELECT(P56) = `SENAO`; SELECT(P57) = FOLLOW(`<senao_opcional>`) = `FIMSE`. Disjuntos. No `se` aninhado
dentro de `senao`, cada `se` termina com o seu `FIMSE`: não há "senão pendente" — o `SENAO` sempre
pertence ao `se` mais interno ainda aberto, porque a lista de comandos do `ENTAO` interno só termina (P35)
ao ver `SENAO` ou `FIMSE`.

### 9.10 `<passo_opcional>` e `<numero_passo>` — LL1-15

SELECT(P59) = `PASSO`; SELECT(P60) = FOLLOW(`<passo_opcional>`) = `FACA` (P58). Disjuntos.
`<numero_passo>`: SELECT(P61) = `NUM_INT`; SELECT(P62) = `MENOS`. Disjuntos. `MENOS` só é aceito aqui.

### 9.11 Listas com vírgula — LL1-16

| Cauda | SELECT explícito | SELECT de ε = FOLLOW | `VIRGULA` no FOLLOW? |
|---|---|---|---|
| `<lista_ids_cauda>` | P12: `VIRGULA` | P13: `DOIS_PONTOS` | não |
| `<lista_parametros_cauda>` | P30: `VIRGULA` | P31: `FECHA_PAR` | não |
| `<lista_argumentos_cauda>` | P66: `VIRGULA` | P67: `FECHA_PAR` | não |

Nas três, `VIRGULA` só seleciona "mais um elemento", e o fechamento (`:` ou `)`) seleciona o fim da lista.
Observação: `VIRGULA` **está** em FOLLOW(`<expressao>`) e em FOLLOW(`<parametro>`) — é justamente o que
permite que a expressão/parâmetro termine antes da vírgula —, mas não no FOLLOW das próprias caudas.

## 10. Tabela preditiva — LL1-18

Construção: para cada produção `P: A -> α` e cada terminal `t ∈ SELECT(P)`, `M[A, t] = P`. Células não
listadas são **erro sintático**; nenhuma foi preenchida artificialmente. Representação **esparsa**: uma
linha por não-terminal, com `lookahead→produção`.

Resultado: **281 células preenchidas**, usando 46 dos 49 terminais como coluna;
**nenhuma célula com mais de uma produção**. Os 3 terminais sem coluna — `ENTAO`, `DE` e `INTERVALO` —
só aparecem **no meio** de produções (P55, P58, P16), nunca no início de um lado direito nem em algum
FOLLOW: são apenas consumidos, nunca decidem uma escolha. A coluna `$` fica vazia (§2).

| Não-terminal | Entradas M[A, lookahead] | Células |
|---|---|---|
| `<programa>` | `ALGORITMO`→P01 | 1 |
| `<corpo_programa>` | `VAR`→P02 · `PROCEDIMENTO`→P03 · `FUNCAO`→P03 | 3 |
| `<secao_var>` | `VAR`→P04 | 1 |
| `<secao_var_opcional>` | `VAR`→P05 · `INICIO`→P06 | 2 |
| `<bloco>` | `INICIO`→P07 | 1 |
| `<lista_declaracoes>` | `ID`→P08 · `INICIO`→P09 | 2 |
| `<declaracao>` | `ID`→P10 | 1 |
| `<lista_ids>` | `ID`→P11 | 1 |
| `<lista_ids_cauda>` | `DOIS_PONTOS`→P13 · `VIRGULA`→P12 | 2 |
| `<tipo_decl>` | `INTEIRO`→P14 · `REAL`→P14 · `CARACTERE`→P14 · `LOGICO`→P14 · `VETOR`→P15 | 5 |
| `<tipo_vetor>` | `VETOR`→P16 | 1 |
| `<tipo_simples>` | `INTEIRO`→P17 · `REAL`→P18 · `CARACTERE`→P19 · `LOGICO`→P20 | 4 |
| `<lista_rotinas>` | `VAR`→P22 · `INICIO`→P22 · `PROCEDIMENTO`→P21 · `FUNCAO`→P21 | 4 |
| `<rotina>` | `PROCEDIMENTO`→P23 · `FUNCAO`→P24 | 2 |
| `<procedimento>` | `PROCEDIMENTO`→P25 | 1 |
| `<parametros_procedimento>` | `INICIO`→P27 · `ABRE_PAR`→P26 | 2 |
| `<funcao>` | `FUNCAO`→P28 | 1 |
| `<lista_parametros>` | `ID`→P29 | 1 |
| `<lista_parametros_cauda>` | `FECHA_PAR`→P31 · `VIRGULA`→P30 | 2 |
| `<parametro>` | `ID`→P32 | 1 |
| `<lista_comandos>` | `ID`→P33 · `LEIA`→P33 · `ESCREVA`→P33 · `ESCREVAL`→P33 · `SE`→P33 · `PARA`→P33 · `ENQUANTO`→P33 · `RETORNE`→P33 | 8 |
| `<lista_comandos_cauda>` | `ID`→P34 · `FIMALGORITMO`→P35 · `LEIA`→P34 · `ESCREVA`→P34 · `ESCREVAL`→P34 · `SE`→P34 · `SENAO`→P35 · `FIMSE`→P35 · `PARA`→P34 · `FIMPARA`→P35 · `ENQUANTO`→P34 · `FIMENQUANTO`→P35 · `FIMPROCEDIMENTO`→P35 · `FIMFUNCAO`→P35 · `RETORNE`→P34 | 15 |
| `<comando>` | `ID`→P36 · `LEIA`→P37 · `ESCREVA`→P38 · `ESCREVAL`→P39 · `SE`→P40 · `PARA`→P41 · `ENQUANTO`→P42 · `RETORNE`→P43 | 8 |
| `<cmd_id>` | `ID`→P44 | 1 |
| `<cauda_comando_id>` | `ID`→P48 · `FIMALGORITMO`→P48 · `LEIA`→P48 · `ESCREVA`→P48 · `ESCREVAL`→P48 · `SE`→P48 · `SENAO`→P48 · `FIMSE`→P48 · `PARA`→P48 · `FIMPARA`→P48 · `ENQUANTO`→P48 · `FIMENQUANTO`→P48 · `FIMPROCEDIMENTO`→P48 · `FIMFUNCAO`→P48 · `RETORNE`→P48 · `ATRIBUICAO`→P45 · `ABRE_PAR`→P47 · `ABRE_COL`→P46 | 18 |
| `<cmd_leia>` | `LEIA`→P49 | 1 |
| `<referencia>` | `ID`→P50 | 1 |
| `<indice_opcional>` | `FECHA_PAR`→P52 · `ABRE_COL`→P51 | 2 |
| `<cmd_escreva>` | `ESCREVA`→P53 | 1 |
| `<cmd_escreval>` | `ESCREVAL`→P54 | 1 |
| `<cmd_se>` | `SE`→P55 | 1 |
| `<senao_opcional>` | `SENAO`→P56 · `FIMSE`→P57 | 2 |
| `<cmd_para>` | `PARA`→P58 | 1 |
| `<passo_opcional>` | `PASSO`→P59 · `FACA`→P60 | 2 |
| `<numero_passo>` | `NUM_INT`→P61 · `MENOS`→P62 | 2 |
| `<cmd_enquanto>` | `ENQUANTO`→P63 | 1 |
| `<cmd_retorne>` | `RETORNE`→P64 | 1 |
| `<lista_argumentos>` | `ID`→P65 · `NUM_INT`→P65 · `NUM_REAL`→P65 · `STRING`→P65 · `VERDADEIRO`→P65 · `FALSO`→P65 · `ABRE_PAR`→P65 | 7 |
| `<lista_argumentos_cauda>` | `FECHA_PAR`→P67 · `VIRGULA`→P66 | 2 |
| `<expressao>` | `ID`→P68 · `NUM_INT`→P68 · `NUM_REAL`→P68 · `STRING`→P68 · `VERDADEIRO`→P68 · `FALSO`→P68 · `ABRE_PAR`→P68 | 7 |
| `<expr_logica>` | `ID`→P69 · `NUM_INT`→P69 · `NUM_REAL`→P69 · `STRING`→P69 · `VERDADEIRO`→P69 · `FALSO`→P69 · `ABRE_PAR`→P69 | 7 |
| `<expr_logica_cauda>` | `ID`→P72 · `FIMALGORITMO`→P72 · `LEIA`→P72 · `ESCREVA`→P72 · `ESCREVAL`→P72 · `SE`→P72 · `SENAO`→P72 · `FIMSE`→P72 · `PARA`→P72 · `ATE`→P72 · `PASSO`→P72 · `FACA`→P72 · `FIMPARA`→P72 · `ENQUANTO`→P72 · `FIMENQUANTO`→P72 · `FIMPROCEDIMENTO`→P72 · `FIMFUNCAO`→P72 · `RETORNE`→P72 · `E`→P70 · `OU`→P71 · `FECHA_PAR`→P72 · `FECHA_COL`→P72 · `VIRGULA`→P72 | 23 |
| `<expr_relacional>` | `ID`→P73 · `NUM_INT`→P73 · `NUM_REAL`→P73 · `STRING`→P73 · `VERDADEIRO`→P73 · `FALSO`→P73 · `ABRE_PAR`→P73 | 7 |
| `<expr_rel_cauda>` | `ID`→P75 · `FIMALGORITMO`→P75 · `LEIA`→P75 · `ESCREVA`→P75 · `ESCREVAL`→P75 · `SE`→P75 · `SENAO`→P75 · `FIMSE`→P75 · `PARA`→P75 · `ATE`→P75 · `PASSO`→P75 · `FACA`→P75 · `FIMPARA`→P75 · `ENQUANTO`→P75 · `FIMENQUANTO`→P75 · `FIMPROCEDIMENTO`→P75 · `FIMFUNCAO`→P75 · `RETORNE`→P75 · `E`→P75 · `OU`→P75 · `OP_REL`→P74 · `FECHA_PAR`→P75 · `FECHA_COL`→P75 · `VIRGULA`→P75 | 24 |
| `<expr_aditiva>` | `ID`→P76 · `NUM_INT`→P76 · `NUM_REAL`→P76 · `STRING`→P76 · `VERDADEIRO`→P76 · `FALSO`→P76 · `ABRE_PAR`→P76 | 7 |
| `<expr_aditiva_cauda>` | `ID`→P78 · `FIMALGORITMO`→P78 · `LEIA`→P78 · `ESCREVA`→P78 · `ESCREVAL`→P78 · `SE`→P78 · `SENAO`→P78 · `FIMSE`→P78 · `PARA`→P78 · `ATE`→P78 · `PASSO`→P78 · `FACA`→P78 · `FIMPARA`→P78 · `ENQUANTO`→P78 · `FIMENQUANTO`→P78 · `FIMPROCEDIMENTO`→P78 · `FIMFUNCAO`→P78 · `RETORNE`→P78 · `E`→P78 · `OU`→P78 · `OP_REL`→P78 · `MAIS`→P77 · `FECHA_PAR`→P78 · `FECHA_COL`→P78 · `VIRGULA`→P78 | 25 |
| `<expr_mult>` | `ID`→P79 · `NUM_INT`→P79 · `NUM_REAL`→P79 · `STRING`→P79 · `VERDADEIRO`→P79 · `FALSO`→P79 · `ABRE_PAR`→P79 | 7 |
| `<expr_mult_cauda>` | `ID`→P81 · `FIMALGORITMO`→P81 · `LEIA`→P81 · `ESCREVA`→P81 · `ESCREVAL`→P81 · `SE`→P81 · `SENAO`→P81 · `FIMSE`→P81 · `PARA`→P81 · `ATE`→P81 · `PASSO`→P81 · `FACA`→P81 · `FIMPARA`→P81 · `ENQUANTO`→P81 · `FIMENQUANTO`→P81 · `FIMPROCEDIMENTO`→P81 · `FIMFUNCAO`→P81 · `RETORNE`→P81 · `E`→P81 · `OU`→P81 · `OP_REL`→P81 · `OP_MULT`→P80 · `MAIS`→P81 · `FECHA_PAR`→P81 · `FECHA_COL`→P81 · `VIRGULA`→P81 | 26 |
| `<primario>` | `ID`→P82 · `NUM_INT`→P83 · `NUM_REAL`→P84 · `STRING`→P85 · `VERDADEIRO`→P86 · `FALSO`→P87 · `ABRE_PAR`→P88 | 7 |
| `<cauda_primario>` | `ID`→P91 · `FIMALGORITMO`→P91 · `LEIA`→P91 · `ESCREVA`→P91 · `ESCREVAL`→P91 · `SE`→P91 · `SENAO`→P91 · `FIMSE`→P91 · `PARA`→P91 · `ATE`→P91 · `PASSO`→P91 · `FACA`→P91 · `FIMPARA`→P91 · `ENQUANTO`→P91 · `FIMENQUANTO`→P91 · `FIMPROCEDIMENTO`→P91 · `FIMFUNCAO`→P91 · `RETORNE`→P91 · `E`→P91 · `OU`→P91 · `OP_REL`→P91 · `OP_MULT`→P91 · `MAIS`→P91 · `ABRE_PAR`→P90 · `FECHA_PAR`→P91 · `ABRE_COL`→P89 · `FECHA_COL`→P91 · `VIRGULA`→P91 | 28 |

## 11. Resultado

**A GLC P01–P91 é LL(1) para o vocabulário congelado da Fase B.** A afirmação se apoia em:

1. SELECT das 91 produções (§7), todos não vazios;
2. comparação de **todos os 85 pares** de alternativas dos 24 não-terminais com escolha, todas as
   interseções vazias (§8);
3. tabela preditiva com 281 células, **nenhuma duplicada** (§10);
4. conferência independente (§12).

Consequências para a implementação (Fase G):

- **uma função por não-terminal**, com decisão por **um único token de lookahead**;
- **sem backtracking** e sem lookahead 2;
- a GLC **não** precisou de nenhuma emenda: P01–P91 permanecem as da Fase C.

## 12. Verificação independente (script descartável)

Script em Python no diretório temporário da sessão, **fora do repositório** e **não versionado**. Ele
implementa os algoritmos dos §4–§8 de forma explícita (laços de ponto fixo) e confere:

| Invariante | Resultado |
|---|---|
| P01–P91 idênticas às do commit `a712477` | ✔ |
| 91 produções numeradas P01…P91 sem lacuna | ✔ |
| 50 não-terminais, todos definidos | ✔ |
| 49 terminais, todos do contrato léxico, todos usados | ✔ |
| 17 anuláveis (rodada 1: 17; rodada 2: 0) | ✔ |
| as 17 produções ε são P06 P09 P13 P22 P27 P31 P35 P48 P52 P57 P60 P67 P72 P75 P78 P81 P91 | ✔ |
| FIRST e FOLLOW dos 50 não-terminais = cálculo manual feito antes do script | ✔ (0 divergências) |
| SELECT não vazio para as 91 produções | ✔ |
| toda produção ε usa FOLLOW, e SELECT(ε) = FOLLOW(lado esquerdo) | ✔ |
| `$` só em FOLLOW(`<programa>`) e em nenhum SELECT | ✔ |
| `MENOS` só em P62 | ✔ |
| 85 pares comparados, 0 interseções não vazias | ✔ |
| tabela preditiva: 281 células, 0 duplicadas | ✔ |

## 13. Mapa de decisão do parser

Para a Fase G. **Não é código**: é a regra de escolha que cada função de não-terminal seguirá.
"Qualquer outro" = erro sintático (o token encontrado é reportado com sua linha, REQ-31).

### 13.1 Não-terminais com alternativas (24)

| Não-terminal | Lookahead → produção |
|---|---|
| `<corpo_programa>` | `VAR` → P02 · `PROCEDIMENTO`, `FUNCAO` → P03 |
| `<secao_var_opcional>` | `VAR` → P05 · `INICIO` → P06 (ε) |
| `<lista_declaracoes>` | `ID` → P08 · `INICIO` → P09 (ε) |
| `<lista_ids_cauda>` | `VIRGULA` → P12 · `DOIS_PONTOS` → P13 (ε) |
| `<tipo_decl>` | `INTEIRO`, `REAL`, `CARACTERE`, `LOGICO` → P14 · `VETOR` → P15 |
| `<tipo_simples>` | `INTEIRO` → P17 · `REAL` → P18 · `CARACTERE` → P19 · `LOGICO` → P20 |
| `<lista_rotinas>` | `PROCEDIMENTO`, `FUNCAO` → P21 · `VAR`, `INICIO` → P22 (ε) |
| `<rotina>` | `PROCEDIMENTO` → P23 · `FUNCAO` → P24 |
| `<parametros_procedimento>` | `ABRE_PAR` → P26 · `INICIO` → P27 (ε) |
| `<lista_parametros_cauda>` | `VIRGULA` → P30 · `FECHA_PAR` → P31 (ε) |
| `<lista_comandos_cauda>` | C (`ID LEIA ESCREVA ESCREVAL SE PARA ENQUANTO RETORNE`) → P34 · F (`FIMALGORITMO SENAO FIMSE FIMPARA FIMENQUANTO FIMPROCEDIMENTO FIMFUNCAO`) → P35 (ε) |
| `<comando>` | `ID` → P36 · `LEIA` → P37 · `ESCREVA` → P38 · `ESCREVAL` → P39 · `SE` → P40 · `PARA` → P41 · `ENQUANTO` → P42 · `RETORNE` → P43 |
| `<cauda_comando_id>` | `ATRIBUICAO` → P45 · `ABRE_COL` → P46 · `ABRE_PAR` → P47 · C ∪ F → P48 (ε) |
| `<indice_opcional>` | `ABRE_COL` → P51 · `FECHA_PAR` → P52 (ε) |
| `<senao_opcional>` | `SENAO` → P56 · `FIMSE` → P57 (ε) |
| `<passo_opcional>` | `PASSO` → P59 · `FACA` → P60 (ε) |
| `<numero_passo>` | `NUM_INT` → P61 · `MENOS` → P62 |
| `<lista_argumentos_cauda>` | `VIRGULA` → P66 · `FECHA_PAR` → P67 (ε) |
| `<expr_logica_cauda>` | `E` → P70 · `OU` → P71 · X → P72 (ε) |
| `<expr_rel_cauda>` | `OP_REL` → P74 · X ∪ `E OU` → P75 (ε) |
| `<expr_aditiva_cauda>` | `MAIS` → P77 · X ∪ `E OU OP_REL` → P78 (ε) |
| `<expr_mult_cauda>` | `OP_MULT` → P80 · X ∪ `E OU OP_REL MAIS` → P81 (ε) |
| `<primario>` | `ID` → P82 · `NUM_INT` → P83 · `NUM_REAL` → P84 · `STRING` → P85 · `VERDADEIRO` → P86 · `FALSO` → P87 · `ABRE_PAR` → P88 |
| `<cauda_primario>` | `ABRE_COL` → P89 · `ABRE_PAR` → P90 · X ∪ `E OU OP_REL MAIS OP_MULT` → P91 (ε) |

Os conjuntos C, F e X estão definidos no §6.1; as listas completas de cada ε estão no §7.

**Nota de implementação (a decidir na Fase G, não aqui):** formalmente, uma alternativa ε é escolhida
**só** para tokens do seu SELECT. Uma implementação pode testar esse conjunto explicitamente (detecta o erro
mais cedo) ou tratar ε como "caso contrário" e deixar o erro aparecer no próximo `consome()` (mais simples).
As duas reconhecem a mesma linguagem; a diferença é só **onde** o erro sintático é reportado.

### 13.2 Não-terminais de produção única (26)

Não há escolha: a função consome a sequência da produção. O primeiro token esperado é o SELECT (§7):
`<programa>` (`ALGORITMO`), `<secao_var>` (`VAR`), `<bloco>` (`INICIO`), `<declaracao>` e `<lista_ids>` (`ID`),
`<tipo_vetor>` (`VETOR`), `<procedimento>` (`PROCEDIMENTO`), `<funcao>` (`FUNCAO`), `<lista_parametros>` e
`<parametro>` (`ID`), `<lista_comandos>` (C), `<cmd_id>` (`ID`), `<cmd_leia>` (`LEIA`), `<referencia>` (`ID`),
`<cmd_escreva>` (`ESCREVA`), `<cmd_escreval>` (`ESCREVAL`), `<cmd_se>` (`SE`), `<cmd_para>` (`PARA`),
`<cmd_enquanto>` (`ENQUANTO`), `<cmd_retorne>` (`RETORNE`), e `<lista_argumentos>`, `<expressao>`,
`<expr_logica>`, `<expr_relacional>`, `<expr_aditiva>`, `<expr_mult>` (FIRST de primário:
`ID NUM_INT NUM_REAL STRING VERDADEIRO FALSO ABRE_PAR`).

## 14. Rastreabilidade — IDs `LL1-nn`

Cadeia: **produção → FIRST/FOLLOW/SELECT → decisão do parser → teste (Fase E)**.

| ID | O que fica provado | Onde | Produções |
|---|---|---|---|
| LL1-01 | 17 anuláveis, calculados por ponto fixo | §4 | P06 … P91 (ε) |
| LL1-02 | FIRST dos 50 não-terminais | §5 | todas |
| LL1-03 | FOLLOW dos 50 não-terminais | §6 | todas |
| LL1-04 | SELECT das 91 produções | §7 | P01–P91 |
| LL1-05 | todas as alternativas disjuntas (85 pares) | §8 | 24 não-terminais |
| LL1-06 | `<cauda_comando_id>` decide com 1 token | §9.1 | P44–P48 |
| LL1-07 | comandos separados sem quebra de linha | §9.2 | P34, P36, P48, P72–P91 |
| LL1-08 | `<cauda_primario>` decide com 1 token | §9.3 | P82, P89–P91 |
| LL1-09 | caudas de expressão sem conflito; hierarquia confirmada | §9.4 | P70–P81 |
| LL1-10 | fim da lista de comandos pelo fechamento | §9.5 | P33–P35 |
| LL1-11 | fim da seção de variáveis em `INICIO` | §9.6 | P08, P09 |
| LL1-12 | fim das rotinas em `VAR`/`INICIO`; `var` opcional | §9.7 | P02, P03, P05, P06, P21, P22 |
| LL1-13 | procedimento com/sem parâmetros decide após `PROCEDIMENTO ID` | §9.8 | P25–P27 |
| LL1-14 | `senao` opcional; aninhamento sem ambiguidade | §9.9 | P55–P57 |
| LL1-15 | `passo` opcional; `MENOS` só no passo | §9.10 | P59–P62 |
| LL1-16 | listas com vírgula terminam no fechamento | §9.11 | P12, P13, P30, P31, P66, P67 |
| LL1-17 | relacional não encadeável sem conflito | §9.4 | P73–P75 |
| LL1-18 | tabela preditiva sem célula duplicada | §10 | todas |
| LL1-19 | `$` (metalinguístico) ↔ `TOKEN_EOF` (implementação) | §2 | P01 |

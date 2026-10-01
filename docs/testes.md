# Testes do MiniVisualg — Fase E

> **Status: suíte pronta, compilador inexistente.** 62 testes (9 LX-V, 9 LX-E, 19 SY-V, 20 SY-E, 5 SY-L)
> escritos **antes** da implementação. Cobertura: P01–P91 (todas), os 49 terminais, `LEX-01`…`LEX-18` e as
> decisões críticas. Nada foi executado em compilador nenhum: ele ainda não existe.
>
> Arquivos em [`../testes/`](../testes/); índice em [`../testes/manifest.tsv`](../testes/manifest.tsv).

## 1. Objetivo

Fixar, antes do código, o comportamento observável que as Fases F (léxico), G (sintático) e H (integração)
devem produzir, com cada expectativa rastreada até o material, uma regra `LEX-nn`, uma produção `Pnn`, uma
validação `LL1-nn` ou uma decisão `DEC-nn`.

Cada teste responde: **o que** se testa; **qual regra** sustenta; **qual autoridade** (CORE, DECISION ou
LIMITATION); **qual resultado**; e, se for erro, **qual tipo, linha e sequência/token**.

## 2. Estratégia test-first

1. Programas de teste escritos à mão, um foco por arquivo, só com construções do Anexo I ou das DEC.
2. **Expectativas escritas antes de qualquer verificação:** os 9 `.erro.txt`, os goldens de LX-V01 e LX-V07 e
   a linha de cada `SY-E` no manifest foram escritos à mão a partir da especificação.
3. Conferência com um **verificador descartável** (fora do repositório, §13): um tokenizador de referência
   escrito só a partir do contrato léxico e um analisador LL(1) estrito montado a partir de P01–P91.
   **Todas as expectativas manuais coincidiram** com a referência.
4. Os outros sete goldens léxicos (LX-V02 a LX-V06, LX-V08 e LX-V09) foram gerados pelo tokenizador de referência e
   **revisados manualmente**, token a token, contra o contrato.
5. Daqui em diante, os testes são o contrato: o código se adapta a eles, e não o contrário.

## 3. Organização

```
testes/
├── README.md
├── manifest.tsv
├── lexico/{validos, erros, esperados}
└── sintatico/{validos, erros, limites}
```

`manifest.tsv` (separado por TAB): `id camada classe arquivo esperado linha detalhe rastreabilidade`.
`esperado` ∈ {`TOKENS`, `ACEITA`, `ERRO_LEXICO`, `ERRO_SINTATICO`}; `linha` é a linha do primeiro erro ou `-`.
**Não há coluna de exit status** (AMB-13).

## 4. Legenda de autoridade

| Classe | Significado |
|---|---|
| **CORE** | sustentado diretamente pelo Anexo I ou pelo enunciado |
| **DECISION** | sustentado por decisão do grupo (DEC); **não** é requisito da professora |
| **LIMITATION** | aceito sintaticamente, mas semanticamente questionável; mostra o limite da análise |

Quando um arquivo mistura evidência e decisão (ex.: `E` confirmado e `OU` por DEC-31), a classe é a mais
fraca envolvida (DECISION).

## 5. Matriz léxica válida (LX-V)

Cada LX-V tem golden `testes/lexico/esperados/<arquivo>.tokens.txt`.

| ID | Classe | Foco | Tokens | Regras |
|---|---|---|---|---|
| LX-V01 `minimo` | CORE | programa mínimo; reservadas; `STRING` com acento; `(`/`)` encostados | 9 | LEX-03 LEX-07 LEX-17 DEC-20 DEC-22 DEC-23 |
| LX-V02 `espacado` | DECISION | mesmo programa com espaços extras e TAB; **golden idêntico ao de LX-V01** | 9 | REQ-14 AMB-01 DEC-05 LEX-09 |
| LX-V03 `tabela-simbolos` | CORE | `nome`=1, `sobrenome`=2; reocorrências reutilizam o índice; reservadas fora da TS | 26 | REQ-26 LEX-15 DEC-07 |
| LX-V04 `numeros-intervalo` | CORE | `0`, `1234`, `1.50`, `1.60` como escritos; `1..4` = `NUM_INT INTERVALO NUM_INT` | 37 | LEX-04 LEX-05 LEX-06 DEC-24 |
| LX-V05 `operadores` | DECISION | `<- + * / \ = <> <= >= MOD E OU`; `-` só em `passo -2` | 84 | LEX-10 LEX-14 DEC-14 DEC-15 DEC-18 DEC-19 |
| LX-V06 `strings-comentarios` | CORE | `"Olá, João!"`, `"http://exemplo"`, `""`, `"a // b"`; comentário de linha inteira (com `@` e aspas) e após código | 23 | LEX-07 LEX-08 LEX-12 DEC-21 |
| LX-V07 `caixa-identificadores` | DECISION | `ALGORITMO`, `mod`, `e`, `ou`, `Nome`, `nome`, `NAO` viram `ID`; `portaAberta`, `linha_decorativa`, `n1` | 14 | LEX-01 LEX-02 LEX-14 DEC-13 DEC-27 |
| LX-V08 `comentario-eof` | CORE | comentário termina no EOF; **arquivo sem `\n` final** | 9 | LEX-08 DEC-23 |
| LX-V09 `reservadas` | CORE | as 13 reservadas que não aparecem nos outros goldens (`se entao senao fimse enquanto fimenquanto procedimento fimprocedimento funcao fimfuncao retorne verdadeiro falso`) | 59 | LEX-03 LEX-17 DEC-20 |

Pontos verificados literalmente nos goldens:

- `OP_REL` com os quatro atributos `EQ NE LE GE` e `OP_MULT` com `MUL DIV_REAL DIV_INT MOD` (LX-V05). Não
  existe `OP_LT` nem `OP_GT`.
- `-2` é `MENOS` + `NUM_INT | 2` (LX-V05).
- `NUM_REAL | 1.60`, e não `1.6` (LX-V04).
- Índices de TS determinísticos: ordem da primeira ocorrência no arquivo, começando em 1. Em LX-V07:
  `ALGORITMO`=1, `mod`=2, `e`=3, `ou`=4, `Nome`=5, `nome`=6, `NAO`=7, `portaAberta`=8,
  `linha_decorativa`=9, `n1`=10.
- Nenhuma linha `EOF` e nenhum `| NULL`.

## 6. Matriz de erros léxicos (LX-E)

O conteúdo esperado está em `testes/lexico/esperados/<arquivo>.erro.txt`
(`TIPO=ERRO LÉXICO`, `LINHA=`, `SEQUENCIA=`).

| ID | Classe | Linha | Sequência | Situação | Regras |
|---|---|---|---|---|---|
| LX-E01 | CORE | 5 | `@` | caractere fora do alfabeto em `usuario@dominio` | REQ-24 LEX-13 |
| LX-E02 | DECISION | 3 | `_` | identificador iniciando por `_` | LEX-02 DEC-27 |
| LX-E03 | DECISION | 3 | `ç` | `preço`: `pre` é `ID`, `ç` é inválido fora de string | LEX-02 DEC-27 AMB-08 |
| LX-E04 | DECISION | 6 | `<` | `<` isolado em `idade < 18` | LEX-11 DEC-16 |
| LX-E05 | DECISION | 6 | `>` | `>` isolado em `idade > 18` | LEX-11 DEC-16 |
| LX-E06 | DECISION | 5 | `.` | `1.` = `NUM_INT` seguido de `.` isolado | LEX-06 DEC-24 |
| LX-E07 | CORE | 4 | `"Olá, mundo)` | `STRING` aberta até a quebra de linha | LEX-07 DEC-09 DEC-26 |
| LX-E08 | CORE | 4 | `"sem fechamento` | `STRING` aberta até o EOF (arquivo sem `\n` final) | LEX-07 DEC-09 DEC-26 |
| LX-E09 | DECISION | 5 | `^` | `^` não pertence ao vocabulário | LEX-13 REQ-04 |

Sobre LX-E03: a sequência esperada é o **caractere** `ç`; como ele é lido em bytes (UTF-8 vs. Windows-1252)
fica para a Fase F (AMB-08). Os arquivos de teste estão em UTF-8.

### 6.1 Fronteira: o que **não** é erro léxico

Estas formas **não** viram erro léxico automaticamente, porque cada pedaço é um lexema válido. Se forem
rejeitadas, é pela **sintaxe**, e dependendo do contexto:

| Forma | Tokenização pelo contrato | Onde está coberto |
|---|---|---|
| `ALGORITMO`, `Algoritmo` | `ID` (case-sensitive) | LX-V07 |
| `mod`, `e`, `ou` | `ID` (só `MOD`, `E`, `OU` são operadores) | LX-V07 |
| `NAO` | `ID` (não é reservada) | LX-V07 |
| `/* comentario */` | `OP_MULT\|DIV_REAL` `OP_MULT\|MUL` `ID` `OP_MULT\|MUL` `OP_MULT\|DIV_REAL` | SY-E20 (rejeitado pela GLC) |
| `1,5` | `NUM_INT\|1` `VIRGULA` `NUM_INT\|5` | nota de fronteira; em `escreval(1,5)` seria **aceito** como dois argumentos |
| `a - b` | `ID` `MENOS` `ID` | SY-E14 (rejeitado pela GLC) |

## 7. Matriz sintática válida (SY-V) — esperado: ACEITA

| ID | Classe | Construção coberta | Produções principais |
|---|---|---|---|
| SY-V01 `minimo` | CORE | `var` vazio, comentário, `escreval` | P01 P02 P04 P09 P07 P54 |
| SY-V02 `declaracoes` | CORE | `nome, sobrenome: caractere`; `inteiro` `real` `caractere` `logico`; `verdadeiro` | P10–P14 P17–P20 P86 |
| SY-V03 `entrada-saida` | CORE | `leia`, `escreva`, `escreval` com 5 argumentos | P49 P50 P52 P53 P54 P65–P67 |
| SY-V04 `aritmetica` | CORE | `+ * / \ MOD`; `n1 + n2 * 2` | P76–P81 |
| SY-V05 `relacionais` | CORE | `>= <= = <>` | P73–P75 |
| SY-V06 `logicos` | DECISION | `E` (Anexo I) e `OU` (DEC-31); `falso`; `1.50`; parênteses | P69–P72 P84 P87 P88 |
| SY-V07 `se` | CORE | `se ( … ) entao … fimse` | P40 P55 P57 |
| SY-V08 `se-aninhado` | CORE | `se`/`senao` com `se` aninhado no `senao` | P55 P56 P57 |
| SY-V09 `para` | CORE | `para … de … ate … faca` sem passo | P41 P58 P60 |
| SY-V10 `para-passo` | CORE | `passo -2` | P59 P62 |
| SY-V11 `enquanto` | CORE | `enquanto ( … ) faca`; `contador <- contador + 1` | P42 P63 P77 |
| SY-V12 `vetores` | CORE | `vetor[1..3] de caractere`; `nomes[1] <- …`; `leia(notas[i])`; `notas[i]` em expressão | P15 P16 P46 P51 P89 |
| SY-V13 `procedimento-sem-parametros` | DECISION | procedimento sem parênteses; `var` depois da rotina; chamada `linha_decorativa` | P03 P05 P23 P25 P27 P48 |
| SY-V14 `procedimento-com-parametro` | DECISION | procedimento com parâmetro; **sem** `var`; chamada `mostrar_erro("…")` | P03 P06 P26 P29 P31 P32 P47 |
| SY-V15 `funcao-somar` | CORE | `funcao somar(a: inteiro, b: inteiro): inteiro`; `retorne a + b`; `somar(10, 5)` | P24 P28 P30 P43 P64 P90 |
| SY-V16 `funcao-eh-par` | CORE | `retorne` dentro de `se`; `v MOD 2 = 0`; `eh_par(num)` como argumento | P28 P55 P56 P64 P74 P80 |
| SY-V17 `rotinas-mistas` | DECISION | procedimento, função e procedimento no mesmo programa | P21 P23 P24 |
| SY-V18 `para-limites-expressao` | DECISION | `de ini ate n * 2` e `passo 2` (não mostrados no Anexo I) | P58 P61 |
| SY-V19 `comandos-id-consecutivos` | DECISION | `linha_a` `linha_b` seguidos: `ID ID` sem newline como token | P34 P36 P48 |

SY-V13/SY-V14/SY-V17/SY-V19 seguem a **reconstrução do grupo** para a estrutura com rotinas (DEC-28/DEC-29;
AMB-05), não a diagramação da página de procedimentos do PDF. SY-V15/SY-V16 são CORE porque o Anexo I
mostra a função antes de `var` de forma clara.

## 8. Matriz de erros sintáticos (SY-E) — esperado: ERRO_SINTATICO

Todos os SY-E **tokenizam sem erro léxico** (verificado, §13). O ponto de falha foi obtido **formalmente** da
tabela preditiva de `analise-ll1.md`: é o primeiro token para o qual a função em curso não tem entrada
`M[A, t]`, ou o terminal esperado não coincide.

| ID | Classe | Linha | Token incorreto | Ponto da gramática | Motivo |
|---|---|---|---|---|---|
| SY-E01 | DECISION | 2 | `INICIO` | `M[<corpo_programa>, INICIO]` vazio (só `VAR`→P02, `PROCEDIMENTO`/`FUNCAO`→P03) | sem `var` e sem rotina (DEC-29) |
| SY-E02 | DECISION | 4 | `FIMALGORITMO` | `M[<lista_comandos>, FIMALGORITMO]` vazio (P33 exige início de comando) | bloco vazio (DEC-39) |
| SY-E03 | CORE | 3 | `INTEIRO` | `M[<lista_ids_cauda>, INTEIRO]` vazio (só `VIRGULA`/`DOIS_PONTOS`) | declaração sem `:` |
| SY-E04 | DECISION | 4 | `ID` | P16 espera `NUM_INT` depois de `ABRE_COL` | limite de vetor `i` (DEC-42) |
| SY-E05 | DECISION | 2 | `FECHA_PAR` | `M[<lista_parametros>, FECHA_PAR]` vazio (P29 exige `ID`) | `procedimento p()` (DEC-41) |
| SY-E06 | DECISION | 2 | `FECHA_PAR` | `M[<lista_parametros>, FECHA_PAR]` vazio | `funcao f(): inteiro` (DEC-40) |
| SY-E07 | DECISION | 4 | `FECHA_PAR` | `M[<lista_argumentos>, FECHA_PAR]` vazio | `escreval()` (DEC-44) |
| SY-E08 | DECISION | 5 | `VIRGULA` | `M[<indice_opcional>, VIRGULA]` vazio (só `ABRE_COL`/`FECHA_PAR`) | `leia(a, b)` (DEC-44) |
| SY-E09 | DECISION | 5 | `ID` | P55 espera `ABRE_PAR` depois de `SE` | `se idade = 18 entao` (AMB-10) |
| SY-E10 | DECISION | 6 | `ID` | P63 espera `ABRE_PAR` depois de `ENQUANTO` | `enquanto contador <= 5 faca` (AMB-10) |
| SY-E11 | CORE | 6 | `ESCREVAL` | P55 espera `ENTAO` depois de `FECHA_PAR` | `se (…)` sem `entao`: o erro aparece na **linha seguinte**, no token que está no lugar de `entao` |
| SY-E12 | CORE | 8 | `FIMALGORITMO` | `M[<senao_opcional>, FIMALGORITMO]` vazio (só `SENAO`/`FIMSE`) | `se` sem `fimse` |
| SY-E13 | DECISION | 6 | `OP_REL` (2º) | `M[<expr_logica_cauda>, OP_REL]` vazio | `a = b = c` (DEC-35, LL1-17) |
| SY-E14 | DECISION | 5 | `MENOS` | `M[<cauda_primario>, MENOS]` vazio | `b - c`: o `-` é léxico válido, a rejeição é **sintática** (DEC-33) |
| SY-E15 | DECISION | 5 | `ID` | `M[<numero_passo>, ID]` vazio (só `NUM_INT`/`MENOS`) | `passo x` (DEC-34) |
| SY-E16 | DECISION | 5 | `ID` | P62 espera `NUM_INT` depois de `MENOS` | `passo -x` (DEC-34) |
| SY-E17 | DECISION | 9 | `FECHA_PAR` | `M[<lista_argumentos>, FECHA_PAR]` vazio (via P90) | `dobro()` em expressão (DEC-44) |
| SY-E18 | DECISION | 7 | `FECHA_PAR` | `M[<lista_argumentos>, FECHA_PAR]` vazio (via P47) | `linha_decorativa()` (DEC-41) |
| SY-E19 | CORE | 4 | fim de arquivo | `M[<lista_comandos_cauda>, $]` vazio (`$` ↔ `TOKEN_EOF`) | falta `fimalgoritmo` (arquivo sem `\n` final) |
| SY-E20 | DECISION | 5 | `OP_MULT` (`/`) | `M[<lista_comandos_cauda>, OP_MULT]` vazio | `/* comentario */` não é comentário (DEC-08) |

**O que a Fase G deverá reportar** (REQ-31): `ERRO SINTÁTICO`, o **token incorreto** e a **linha** das colunas
acima. Informar "esperado/encontrado" é opcional.

**Token e linha não dependem da estratégia de implementação.** A tabela usa a forma estrita (ε escolhida só
para tokens do SELECT). Se a Fase G tratar ε como "caso contrário" (`analise-ll1.md` §13), o erro pode ser
detectado em **outra função**, mas no **mesmo token e na mesma linha**, porque nenhum token é consumido entre
as duas detecções (propriedade do prefixo válido do LL(1)). Só a lista de "esperados" pode mudar.

## 9. Limitações sintáticas (SY-L) — esperado: ACEITA_SINTATICAMENTE

No manifest, o esperado é `ACEITA`; a classe `LIMITATION` indica que o programa **não** é exemplo oficial de
MiniVisualg válido.

| ID | O que é aceito | Por que a sintaxe aceita | Seria verificado por |
|---|---|---|---|
| SY-L01 `retorne-fora-funcao` | `retorne 1` no programa principal | `RETORNE` é comando geral (DEC-38) | análise contextual/semântica |
| SY-L02 `id-isolado` | `x` (variável) sozinho como comando | `ID` + ε = forma de chamada sem parâmetros (P48, DEC-36) | tabela de símbolos semântica |
| SY-L03 `chamada-nao-declarada` | `mostrar_algo("x")` sem declaração | P47 verifica forma, não declaração | semântica |
| SY-L04 `aridade-incompativel` | `mostrar("a", "b")` com 1 parâmetro declarado | a GLC não compara assinatura | semântica |
| SY-L05 `atribuicao-seguida-id` | `x <- a` e, na linha seguinte, `b` | sem delimitador de linha, `ID ATRIBUICAO ID ID` = dois comandos (LL1-07) | — (é a própria linguagem baseada em tokens) |

**Não** há testes que esperam erro para variável não declarada, tipos, escopo, índice real, assinatura ou
`retorne` fora de função: isso está fora do Projeto 1 atual (REQ-35).

## 10. Goldens

- **Léxicos válidos:** `.tokens.txt`, comparação **literal** linha a linha (linha, nome, atributo). Fim de
  linha: o repositório guarda LF e o Git no Windows (`core.autocrlf=true`) entrega CRLF no checkout; a
  comparação da Fase F deve ignorar só essa diferença.
- **Erros léxicos:** `.erro.txt` define o **conteúdo** (tipo, linha, sequência), não o texto exato da mensagem
  nem o destino. O formato proposto da mensagem é o de DEC-26.
- **Sintáticos:** aceitação/rejeição; para SY-E, a linha do manifest e o token da §8.
- Os goldens são **referências internas de teste**. Eles **não** definem o nome do arquivo de tokens que o
  compilador gerará em produção (AMB-14).

## 11. Cobertura de requisitos, regras e decisões

### 11.1 Regras léxicas `LEX-01`…`LEX-18`

| Regra | Positivo | Negativo |
|---|---|---|
| LEX-01 case-sensitive | LX-V07 | — (variações viram `ID`, não erro) |
| LEX-02 ER de ID | LX-V07 (`portaAberta`, `linha_decorativa`, `n1`), LX-V03 | LX-E02 (`_x`), LX-E03 (acento) |
| LEX-03 reservada > ID | LX-V01, LX-V07, LX-V09 (as 31 reservadas cobertas pelos goldens) | — |
| LEX-04 `NUM_INT` sem sinal | LX-V04, LX-V05 (`-2`) | — |
| LEX-05 `NUM_REAL` | LX-V04 (`1.50`, `1.60`) | LX-E06 (`1.`) |
| LEX-06 regra do ponto | LX-V04 (`1..4`) | LX-E06 |
| LEX-07 `STRING` | LX-V01, LX-V06 (acento, `//`, vazia) | LX-E07 (EOL), LX-E08 (EOF) |
| LEX-08 comentário `//` | LX-V06, LX-V08 (EOF) | SY-E20 (`/* */` não é comentário) |
| LEX-09 whitespace e linhas | LX-V02 (espaços/TAB), todos os goldens (linhas), LX-V06 (linha só de comentário) | — |
| LEX-10 maximal munch | LX-V05 (`<- <> <= >=`), LX-V04 (`..`) | LX-E04, LX-E05 |
| LEX-11 `<`/`>` isolados | — | LX-E04, LX-E05 |
| LEX-12 `/` vs `//` | LX-V05 (`/`), LX-V06 (`//`) | SY-E20 |
| LEX-13 caractere inválido | — | LX-E01, LX-E02, LX-E03, LX-E09 |
| LEX-14 `MOD`/`E`/`OU` exatos | LX-V05, LX-V07 | — |
| LEX-15 TS só de IDs | LX-V03, LX-V07 | — |
| LEX-16 atributos | LX-V03, LX-V04, LX-V05, LX-V06 | — |
| LEX-17 formato de saída | todos os `.tokens.txt` | — |
| LEX-18 mensagem de erro | todos os `.erro.txt` | — |

### 11.2 Decisões críticas

| Decisão | Testes |
|---|---|
| DEC-05 scanner sem depender de espaço | LX-V01 × LX-V02 (goldens idênticos) |
| DEC-13 case-sensitive | LX-V07 |
| DEC-14 `OU` reservado | LX-V05, LX-V07 |
| DEC-15 `MENOS` separado | LX-V05, SY-V10 |
| DEC-16 `<`/`>` inválidos | LX-E04, LX-E05 |
| DEC-17 nome `ID` | todos os goldens |
| DEC-18 `OP_REL` + atributo | LX-V05, SY-V05 |
| DEC-19 `OP_MULT` + atributo | LX-V05, SY-V04 |
| DEC-20 uma reservada = um token | LX-V01, LX-V07, LX-V09 |
| DEC-21 `STRING` preserva valor | LX-V06 |
| DEC-22/23 sem `\|` vazio; EOF não impresso | todos os goldens |
| DEC-24 regra do ponto | LX-V04, LX-E06 |
| DEC-28/29 rotinas antes do principal; `var` conservadora | SY-V13 (com `var`), SY-V14 (sem `var`), SY-V15, SY-E01 |
| DEC-30 rotinas mistas | SY-V17 |
| DEC-31/32 `OU` na gramática, mesmo nível de `E` | SY-V06 |
| DEC-33/34 sem subtração; passo restrito | SY-V10, SY-V18, SY-E14, SY-E15, SY-E16 |
| DEC-35 hierarquia; relacional não encadeável | SY-V04, SY-V16, SY-E13 |
| DEC-36/37 fatoração por `ID` | SY-V12, SY-V13, SY-V14, SY-V15, SY-V19, SY-L02 |
| DEC-38 `RETORNE` contextual | SY-L01 |
| DEC-39 listas não vazias | SY-E02 |
| DEC-40/41 sem `()` vazio em rotinas | SY-E05, SY-E06, SY-E18 |
| DEC-42 limites de vetor | SY-E04 |
| DEC-43 limites de `para` | SY-V18 |
| DEC-44 aridade de `leia`/`escreva`/chamadas | SY-E07, SY-E08, SY-E17 |
| DEC-47 LL(1) / mapa de decisão | SY-V19, SY-L05 e a §8 inteira (pontos de falha tirados da tabela) |
| AMB-10 parênteses em `se`/`enquanto` | SY-V07, SY-V11, SY-E09, SY-E10 |

### 11.3 Requisitos de erro do enunciado

| Requisito | Verificado depois por |
|---|---|
| REQ-22 tokens no arquivo **e** na tela | goldens LX-V (Fases F/H comparam os dois destinos) |
| REQ-23 formato `linha# Nome \| Atributo` | goldens LX-V |
| REQ-24 `ERRO LÉXICO` + linha + sequência; encerrar | `.erro.txt` de LX-E01…LX-E09 |
| REQ-31 `ERRO SINTÁTICO` + token + linha; encerrar | §8 (SY-E01…SY-E20) |
| REQ-11 retorno 0 em entrada válida | todos os LX-V, SY-V, SY-L (Fase H) |

## 12. Cobertura das produções e dos terminais

### 12.1 P01–P91

Cada produção é percorrida por pelo menos um **SY-V** aceito (as SY-L não contam). Lista gerada a partir
das derivações reais do analisador LL(1) de referência.

| Produção | Testes SY-V que a percorrem |
|---|---|
| P01 `<programa> -> ALGORITMO STRING <corpo_programa> FIMALGORITMO` | SY-V01 SY-V02 SY-V03 SY-V04 SY-V05 SY-V06 SY-V07 SY-V08 SY-V09 SY-V10 SY-V11 SY-V12 SY-V13 SY-V14 SY-V15 SY-V16 SY-V17 SY-V18 SY-V19 |
| P02 `<corpo_programa> -> <secao_var> <bloco>` | SY-V01 SY-V02 SY-V03 SY-V04 SY-V05 SY-V06 SY-V07 SY-V08 SY-V09 SY-V10 SY-V11 SY-V12 SY-V18 |
| P03 `<corpo_programa> -> <rotina> <lista_rotinas> <secao_var_opcional> <bloco>` | SY-V13 SY-V14 SY-V15 SY-V16 SY-V17 SY-V19 |
| P04 `<secao_var> -> VAR <lista_declaracoes>` | SY-V01 SY-V02 SY-V03 SY-V04 SY-V05 SY-V06 SY-V07 SY-V08 SY-V09 SY-V10 SY-V11 SY-V12 SY-V13 SY-V15 SY-V16 SY-V17 SY-V18 |
| P05 `<secao_var_opcional> -> <secao_var>` | SY-V13 SY-V15 SY-V16 SY-V17 |
| P06 `<secao_var_opcional> -> ε` | SY-V14 SY-V19 |
| P07 `<bloco> -> INICIO <lista_comandos>` | SY-V01 SY-V02 SY-V03 SY-V04 SY-V05 SY-V06 SY-V07 SY-V08 SY-V09 SY-V10 SY-V11 SY-V12 SY-V13 SY-V14 SY-V15 SY-V16 SY-V17 SY-V18 SY-V19 |
| P08 `<lista_declaracoes> -> <declaracao> <lista_declaracoes>` | SY-V02 SY-V03 SY-V04 SY-V05 SY-V06 SY-V07 SY-V08 SY-V09 SY-V10 SY-V11 SY-V12 SY-V15 SY-V16 SY-V17 SY-V18 |
| P09 `<lista_declaracoes> -> ε` | SY-V01 SY-V02 SY-V03 SY-V04 SY-V05 SY-V06 SY-V07 SY-V08 SY-V09 SY-V10 SY-V11 SY-V12 SY-V13 SY-V15 SY-V16 SY-V17 SY-V18 |
| P10 `<declaracao> -> <lista_ids> DOIS_PONTOS <tipo_decl>` | SY-V02 SY-V03 SY-V04 SY-V05 SY-V06 SY-V07 SY-V08 SY-V09 SY-V10 SY-V11 SY-V12 SY-V15 SY-V16 SY-V17 SY-V18 |
| P11 `<lista_ids> -> ID <lista_ids_cauda>` | SY-V02 SY-V03 SY-V04 SY-V05 SY-V06 SY-V07 SY-V08 SY-V09 SY-V10 SY-V11 SY-V12 SY-V15 SY-V16 SY-V17 SY-V18 |
| P12 `<lista_ids_cauda> -> VIRGULA ID <lista_ids_cauda>` | SY-V02 SY-V04 SY-V05 SY-V06 SY-V18 |
| P13 `<lista_ids_cauda> -> ε` | SY-V02 SY-V03 SY-V04 SY-V05 SY-V06 SY-V07 SY-V08 SY-V09 SY-V10 SY-V11 SY-V12 SY-V15 SY-V16 SY-V17 SY-V18 |
| P14 `<tipo_decl> -> <tipo_simples>` | SY-V02 SY-V03 SY-V04 SY-V05 SY-V06 SY-V07 SY-V08 SY-V09 SY-V10 SY-V11 SY-V12 SY-V15 SY-V16 SY-V17 SY-V18 |
| P15 `<tipo_decl> -> <tipo_vetor>` | SY-V12 |
| P16 `<tipo_vetor> -> VETOR ABRE_COL NUM_INT INTERVALO NUM_INT FECHA_COL DE <tipo_simples>` | SY-V12 |
| P17 `<tipo_simples> -> INTEIRO` | SY-V02 SY-V03 SY-V04 SY-V05 SY-V06 SY-V08 SY-V09 SY-V10 SY-V11 SY-V12 SY-V15 SY-V16 SY-V17 SY-V18 |
| P18 `<tipo_simples> -> REAL` | SY-V02 SY-V04 SY-V06 SY-V12 |
| P19 `<tipo_simples> -> CARACTERE` | SY-V02 SY-V03 SY-V05 SY-V12 SY-V14 |
| P20 `<tipo_simples> -> LOGICO` | SY-V02 SY-V05 SY-V06 SY-V07 SY-V16 |
| P21 `<lista_rotinas> -> <rotina> <lista_rotinas>` | SY-V17 SY-V19 |
| P22 `<lista_rotinas> -> ε` | SY-V13 SY-V14 SY-V15 SY-V16 SY-V17 SY-V19 |
| P23 `<rotina> -> <procedimento>` | SY-V13 SY-V14 SY-V17 SY-V19 |
| P24 `<rotina> -> <funcao>` | SY-V15 SY-V16 SY-V17 |
| P25 `<procedimento> -> PROCEDIMENTO ID <parametros_procedimento> <bloco> FIMPROCEDIMENTO` | SY-V13 SY-V14 SY-V17 SY-V19 |
| P26 `<parametros_procedimento> -> ABRE_PAR <lista_parametros> FECHA_PAR` | SY-V14 SY-V17 |
| P27 `<parametros_procedimento> -> ε` | SY-V13 SY-V17 SY-V19 |
| P28 `<funcao> -> FUNCAO ID ABRE_PAR <lista_parametros> FECHA_PAR DOIS_PONTOS <tipo_simples> <bloco> FIMFUNCAO` | SY-V15 SY-V16 SY-V17 |
| P29 `<lista_parametros> -> <parametro> <lista_parametros_cauda>` | SY-V14 SY-V15 SY-V16 SY-V17 |
| P30 `<lista_parametros_cauda> -> VIRGULA <parametro> <lista_parametros_cauda>` | SY-V15 |
| P31 `<lista_parametros_cauda> -> ε` | SY-V14 SY-V15 SY-V16 SY-V17 |
| P32 `<parametro> -> ID DOIS_PONTOS <tipo_simples>` | SY-V14 SY-V15 SY-V16 SY-V17 |
| P33 `<lista_comandos> -> <comando> <lista_comandos_cauda>` | SY-V01 SY-V02 SY-V03 SY-V04 SY-V05 SY-V06 SY-V07 SY-V08 SY-V09 SY-V10 SY-V11 SY-V12 SY-V13 SY-V14 SY-V15 SY-V16 SY-V17 SY-V18 SY-V19 |
| P34 `<lista_comandos_cauda> -> <comando> <lista_comandos_cauda>` | SY-V02 SY-V03 SY-V04 SY-V05 SY-V06 SY-V07 SY-V08 SY-V11 SY-V12 SY-V13 SY-V15 SY-V16 SY-V17 SY-V18 SY-V19 |
| P35 `<lista_comandos_cauda> -> ε` | SY-V01 SY-V02 SY-V03 SY-V04 SY-V05 SY-V06 SY-V07 SY-V08 SY-V09 SY-V10 SY-V11 SY-V12 SY-V13 SY-V14 SY-V15 SY-V16 SY-V17 SY-V18 SY-V19 |
| P36 `<comando> -> <cmd_id>` | SY-V02 SY-V04 SY-V05 SY-V06 SY-V07 SY-V11 SY-V12 SY-V13 SY-V14 SY-V15 SY-V16 SY-V17 SY-V18 SY-V19 |
| P37 `<comando> -> <cmd_leia>` | SY-V03 SY-V08 SY-V12 |
| P38 `<comando> -> <cmd_escreva>` | SY-V03 |
| P39 `<comando> -> <cmd_escreval>` | SY-V01 SY-V02 SY-V03 SY-V07 SY-V08 SY-V09 SY-V10 SY-V11 SY-V12 SY-V13 SY-V14 SY-V15 SY-V16 SY-V17 SY-V18 SY-V19 |
| P40 `<comando> -> <cmd_se>` | SY-V07 SY-V08 SY-V16 |
| P41 `<comando> -> <cmd_para>` | SY-V09 SY-V10 SY-V12 SY-V18 |
| P42 `<comando> -> <cmd_enquanto>` | SY-V11 |
| P43 `<comando> -> <cmd_retorne>` | SY-V15 SY-V16 SY-V17 |
| P44 `<cmd_id> -> ID <cauda_comando_id>` | SY-V02 SY-V04 SY-V05 SY-V06 SY-V07 SY-V11 SY-V12 SY-V13 SY-V14 SY-V15 SY-V16 SY-V17 SY-V18 SY-V19 |
| P45 `<cauda_comando_id> -> ATRIBUICAO <expressao>` | SY-V02 SY-V04 SY-V05 SY-V06 SY-V07 SY-V11 SY-V12 SY-V15 SY-V16 SY-V17 SY-V18 |
| P46 `<cauda_comando_id> -> ABRE_COL <expressao> FECHA_COL ATRIBUICAO <expressao>` | SY-V12 |
| P47 `<cauda_comando_id> -> ABRE_PAR <lista_argumentos> FECHA_PAR` | SY-V14 SY-V17 |
| P48 `<cauda_comando_id> -> ε` | SY-V13 SY-V17 SY-V19 |
| P49 `<cmd_leia> -> LEIA ABRE_PAR <referencia> FECHA_PAR` | SY-V03 SY-V08 SY-V12 |
| P50 `<referencia> -> ID <indice_opcional>` | SY-V03 SY-V08 SY-V12 |
| P51 `<indice_opcional> -> ABRE_COL <expressao> FECHA_COL` | SY-V12 |
| P52 `<indice_opcional> -> ε` | SY-V03 SY-V08 |
| P53 `<cmd_escreva> -> ESCREVA ABRE_PAR <lista_argumentos> FECHA_PAR` | SY-V03 |
| P54 `<cmd_escreval> -> ESCREVAL ABRE_PAR <lista_argumentos> FECHA_PAR` | SY-V01 SY-V02 SY-V03 SY-V07 SY-V08 SY-V09 SY-V10 SY-V11 SY-V12 SY-V13 SY-V14 SY-V15 SY-V16 SY-V17 SY-V18 SY-V19 |
| P55 `<cmd_se> -> SE ABRE_PAR <expressao> FECHA_PAR ENTAO <lista_comandos> <senao_opcional> FIMSE` | SY-V07 SY-V08 SY-V16 |
| P56 `<senao_opcional> -> SENAO <lista_comandos>` | SY-V08 SY-V16 |
| P57 `<senao_opcional> -> ε` | SY-V07 |
| P58 `<cmd_para> -> PARA ID DE <expressao> ATE <expressao> <passo_opcional> FACA <lista_comandos> FIMPARA` | SY-V09 SY-V10 SY-V12 SY-V18 |
| P59 `<passo_opcional> -> PASSO <numero_passo>` | SY-V10 SY-V18 |
| P60 `<passo_opcional> -> ε` | SY-V09 SY-V12 |
| P61 `<numero_passo> -> NUM_INT` | SY-V18 |
| P62 `<numero_passo> -> MENOS NUM_INT` | SY-V10 |
| P63 `<cmd_enquanto> -> ENQUANTO ABRE_PAR <expressao> FECHA_PAR FACA <lista_comandos> FIMENQUANTO` | SY-V11 |
| P64 `<cmd_retorne> -> RETORNE <expressao>` | SY-V15 SY-V16 SY-V17 |
| P65 `<lista_argumentos> -> <expressao> <lista_argumentos_cauda>` | SY-V01 SY-V02 SY-V03 SY-V07 SY-V08 SY-V09 SY-V10 SY-V11 SY-V12 SY-V13 SY-V14 SY-V15 SY-V16 SY-V17 SY-V18 SY-V19 |
| P66 `<lista_argumentos_cauda> -> VIRGULA <expressao> <lista_argumentos_cauda>` | SY-V03 SY-V14 SY-V15 SY-V16 SY-V17 |
| P67 `<lista_argumentos_cauda> -> ε` | SY-V01 SY-V02 SY-V03 SY-V07 SY-V08 SY-V09 SY-V10 SY-V11 SY-V12 SY-V13 SY-V14 SY-V15 SY-V16 SY-V17 SY-V18 SY-V19 |
| P68 `<expressao> -> <expr_logica>` | SY-V01 SY-V02 SY-V03 SY-V04 SY-V05 SY-V06 SY-V07 SY-V08 SY-V09 SY-V10 SY-V11 SY-V12 SY-V13 SY-V14 SY-V15 SY-V16 SY-V17 SY-V18 SY-V19 |
| P69 `<expr_logica> -> <expr_relacional> <expr_logica_cauda>` | SY-V01 SY-V02 SY-V03 SY-V04 SY-V05 SY-V06 SY-V07 SY-V08 SY-V09 SY-V10 SY-V11 SY-V12 SY-V13 SY-V14 SY-V15 SY-V16 SY-V17 SY-V18 SY-V19 |
| P70 `<expr_logica_cauda> -> E <expr_relacional> <expr_logica_cauda>` | SY-V06 |
| P71 `<expr_logica_cauda> -> OU <expr_relacional> <expr_logica_cauda>` | SY-V06 |
| P72 `<expr_logica_cauda> -> ε` | SY-V01 SY-V02 SY-V03 SY-V04 SY-V05 SY-V06 SY-V07 SY-V08 SY-V09 SY-V10 SY-V11 SY-V12 SY-V13 SY-V14 SY-V15 SY-V16 SY-V17 SY-V18 SY-V19 |
| P73 `<expr_relacional> -> <expr_aditiva> <expr_rel_cauda>` | SY-V01 SY-V02 SY-V03 SY-V04 SY-V05 SY-V06 SY-V07 SY-V08 SY-V09 SY-V10 SY-V11 SY-V12 SY-V13 SY-V14 SY-V15 SY-V16 SY-V17 SY-V18 SY-V19 |
| P74 `<expr_rel_cauda> -> OP_REL <expr_aditiva>` | SY-V05 SY-V06 SY-V07 SY-V08 SY-V11 SY-V16 |
| P75 `<expr_rel_cauda> -> ε` | SY-V01 SY-V02 SY-V03 SY-V04 SY-V06 SY-V07 SY-V08 SY-V09 SY-V10 SY-V11 SY-V12 SY-V13 SY-V14 SY-V15 SY-V16 SY-V17 SY-V18 SY-V19 |
| P76 `<expr_aditiva> -> <expr_mult> <expr_aditiva_cauda>` | SY-V01 SY-V02 SY-V03 SY-V04 SY-V05 SY-V06 SY-V07 SY-V08 SY-V09 SY-V10 SY-V11 SY-V12 SY-V13 SY-V14 SY-V15 SY-V16 SY-V17 SY-V18 SY-V19 |
| P77 `<expr_aditiva_cauda> -> MAIS <expr_mult> <expr_aditiva_cauda>` | SY-V04 SY-V11 SY-V12 SY-V15 |
| P78 `<expr_aditiva_cauda> -> ε` | SY-V01 SY-V02 SY-V03 SY-V04 SY-V05 SY-V06 SY-V07 SY-V08 SY-V09 SY-V10 SY-V11 SY-V12 SY-V13 SY-V14 SY-V15 SY-V16 SY-V17 SY-V18 SY-V19 |
| P79 `<expr_mult> -> <primario> <expr_mult_cauda>` | SY-V01 SY-V02 SY-V03 SY-V04 SY-V05 SY-V06 SY-V07 SY-V08 SY-V09 SY-V10 SY-V11 SY-V12 SY-V13 SY-V14 SY-V15 SY-V16 SY-V17 SY-V18 SY-V19 |
| P80 `<expr_mult_cauda> -> OP_MULT <primario> <expr_mult_cauda>` | SY-V04 SY-V12 SY-V16 SY-V17 SY-V18 |
| P81 `<expr_mult_cauda> -> ε` | SY-V01 SY-V02 SY-V03 SY-V04 SY-V05 SY-V06 SY-V07 SY-V08 SY-V09 SY-V10 SY-V11 SY-V12 SY-V13 SY-V14 SY-V15 SY-V16 SY-V17 SY-V18 SY-V19 |
| P82 `<primario> -> ID <cauda_primario>` | SY-V02 SY-V03 SY-V04 SY-V05 SY-V06 SY-V07 SY-V08 SY-V09 SY-V10 SY-V11 SY-V12 SY-V14 SY-V15 SY-V16 SY-V17 SY-V18 |
| P83 `<primario> -> NUM_INT` | SY-V04 SY-V05 SY-V06 SY-V08 SY-V09 SY-V10 SY-V11 SY-V12 SY-V15 SY-V16 SY-V17 SY-V18 |
| P84 `<primario> -> NUM_REAL` | SY-V06 |
| P85 `<primario> -> STRING` | SY-V01 SY-V02 SY-V03 SY-V05 SY-V07 SY-V08 SY-V12 SY-V13 SY-V14 SY-V15 SY-V16 SY-V17 SY-V19 |
| P86 `<primario> -> VERDADEIRO` | SY-V02 SY-V07 SY-V16 |
| P87 `<primario> -> FALSO` | SY-V06 SY-V16 |
| P88 `<primario> -> ABRE_PAR <expressao> FECHA_PAR` | SY-V06 |
| P89 `<cauda_primario> -> ABRE_COL <expressao> FECHA_COL` | SY-V12 |
| P90 `<cauda_primario> -> ABRE_PAR <lista_argumentos> FECHA_PAR` | SY-V15 SY-V16 SY-V17 |
| P91 `<cauda_primario> -> ε` | SY-V02 SY-V03 SY-V04 SY-V05 SY-V06 SY-V07 SY-V08 SY-V09 SY-V10 SY-V11 SY-V12 SY-V14 SY-V15 SY-V16 SY-V17 SY-V18 |

### 12.2 Os 49 terminais

Todos os 49 aparecem em pelo menos um golden léxico **e** em pelo menos um programa SY-V aceito. Com isso, o
catálogo inteiro do léxico (inclusive as 31 reservadas) é verificado diretamente pelos goldens na Fase F.

| Terminal | Goldens léxicos (LX-V) | Programas aceitos (SY-V) |
|---|---|---|
| `ID` | LX-V03 LX-V04 LX-V05 LX-V06 LX-V07 LX-V09 | SY-V02 SY-V03 SY-V04 SY-V05 SY-V06 SY-V07 … |
| `NUM_INT` | LX-V04 LX-V05 LX-V09 | SY-V04 SY-V05 SY-V06 SY-V08 SY-V09 SY-V10 … |
| `NUM_REAL` | LX-V04 | SY-V06 |
| `STRING` | LX-V01 LX-V02 LX-V03 LX-V04 LX-V05 LX-V06 LX-V08 LX-V09 | SY-V01 SY-V02 SY-V03 SY-V04 SY-V05 SY-V06 … |
| `ALGORITMO` | LX-V01 LX-V02 LX-V03 LX-V04 LX-V05 LX-V06 LX-V07 LX-V08 LX-V09 | SY-V01 SY-V02 SY-V03 SY-V04 SY-V05 SY-V06 … |
| `VAR` | LX-V01 LX-V02 LX-V03 LX-V04 LX-V05 LX-V06 LX-V08 LX-V09 | SY-V01 SY-V02 SY-V03 SY-V04 SY-V05 SY-V06 … |
| `INICIO` | LX-V01 LX-V02 LX-V03 LX-V04 LX-V05 LX-V06 LX-V08 LX-V09 | SY-V01 SY-V02 SY-V03 SY-V04 SY-V05 SY-V06 … |
| `FIMALGORITMO` | LX-V01 LX-V02 LX-V03 LX-V04 LX-V05 LX-V06 LX-V08 LX-V09 | SY-V01 SY-V02 SY-V03 SY-V04 SY-V05 SY-V06 … |
| `INTEIRO` | LX-V04 LX-V05 LX-V09 | SY-V02 SY-V03 SY-V04 SY-V05 SY-V06 SY-V08 … |
| `REAL` | LX-V04 LX-V05 | SY-V02 SY-V04 SY-V06 SY-V12 |
| `CARACTERE` | LX-V03 LX-V06 | SY-V02 SY-V03 SY-V05 SY-V12 SY-V14 |
| `LOGICO` | LX-V05 LX-V09 | SY-V02 SY-V05 SY-V06 SY-V07 SY-V16 |
| `VERDADEIRO` | LX-V09 | SY-V02 SY-V07 SY-V16 |
| `FALSO` | LX-V09 | SY-V06 SY-V16 |
| `LEIA` | LX-V03 | SY-V03 SY-V08 SY-V12 |
| `ESCREVA` | LX-V06 | SY-V03 |
| `ESCREVAL` | LX-V01 LX-V02 LX-V03 LX-V04 LX-V05 LX-V06 LX-V08 LX-V09 | SY-V01 SY-V02 SY-V03 SY-V07 SY-V08 SY-V09 … |
| `SE` | LX-V09 | SY-V07 SY-V08 SY-V16 |
| `ENTAO` | LX-V09 | SY-V07 SY-V08 SY-V16 |
| `SENAO` | LX-V09 | SY-V08 SY-V16 |
| `FIMSE` | LX-V09 | SY-V07 SY-V08 SY-V16 |
| `PARA` | LX-V05 | SY-V09 SY-V10 SY-V12 SY-V18 |
| `DE` | LX-V04 LX-V05 | SY-V09 SY-V10 SY-V12 SY-V18 |
| `ATE` | LX-V05 | SY-V09 SY-V10 SY-V12 SY-V18 |
| `PASSO` | LX-V05 | SY-V10 SY-V18 |
| `FACA` | LX-V05 LX-V09 | SY-V09 SY-V10 SY-V11 SY-V12 SY-V18 |
| `FIMPARA` | LX-V05 | SY-V09 SY-V10 SY-V12 SY-V18 |
| `ENQUANTO` | LX-V09 | SY-V11 |
| `FIMENQUANTO` | LX-V09 | SY-V11 |
| `VETOR` | LX-V04 | SY-V12 |
| `PROCEDIMENTO` | LX-V09 | SY-V13 SY-V14 SY-V17 SY-V19 |
| `FIMPROCEDIMENTO` | LX-V09 | SY-V13 SY-V14 SY-V17 SY-V19 |
| `FUNCAO` | LX-V09 | SY-V15 SY-V16 SY-V17 |
| `FIMFUNCAO` | LX-V09 | SY-V15 SY-V16 SY-V17 |
| `RETORNE` | LX-V09 | SY-V15 SY-V16 SY-V17 |
| `E` | LX-V05 LX-V07 | SY-V06 |
| `OU` | LX-V05 LX-V07 | SY-V06 |
| `OP_REL` | LX-V05 LX-V09 | SY-V05 SY-V06 SY-V07 SY-V08 SY-V11 SY-V16 |
| `OP_MULT` | LX-V05 LX-V07 LX-V09 | SY-V04 SY-V12 SY-V16 SY-V17 SY-V18 |
| `MAIS` | LX-V05 LX-V09 | SY-V04 SY-V11 SY-V12 SY-V15 |
| `MENOS` | LX-V05 | SY-V10 |
| `ATRIBUICAO` | LX-V04 LX-V05 LX-V06 LX-V09 | SY-V02 SY-V04 SY-V05 SY-V06 SY-V07 SY-V11 … |
| `ABRE_PAR` | LX-V01 LX-V02 LX-V03 LX-V04 LX-V05 LX-V06 LX-V08 LX-V09 | SY-V01 SY-V02 SY-V03 SY-V06 SY-V07 SY-V08 … |
| `FECHA_PAR` | LX-V01 LX-V02 LX-V03 LX-V04 LX-V05 LX-V06 LX-V08 LX-V09 | SY-V01 SY-V02 SY-V03 SY-V06 SY-V07 SY-V08 … |
| `ABRE_COL` | LX-V04 | SY-V12 |
| `FECHA_COL` | LX-V04 | SY-V12 |
| `DOIS_PONTOS` | LX-V03 LX-V04 LX-V05 LX-V06 LX-V09 | SY-V02 SY-V03 SY-V04 SY-V05 SY-V06 SY-V07 … |
| `VIRGULA` | LX-V03 LX-V05 | SY-V02 SY-V03 SY-V04 SY-V05 SY-V06 SY-V14 … |
| `INTERVALO` | LX-V04 | SY-V12 |

## 13. Validação automática descartável

Script Python no diretório temporário da sessão, **fora do repositório e não versionado**:

- **tokenizador de referência**, escrito só a partir de `especificacao-lexica.md`, com catálogos, regra do
  ponto, `STRING`, comentário, maximal munch e erros;
- **analisador LL(1) estrito**, com a tabela calculada de P01–P91 lidas de `gramatica.md`.

Resultado da última execução: **TUDO OK**.

| Verificação | Resultado |
|---|---|
| manifest: 62 linhas de teste, 8 colunas, IDs únicos, todo arquivo existe, todo `.alg` do disco está no manifest | ✔ |
| arquivos UTF-8, sem CR no disco; só LX-V08, LX-E08 e SY-E19 sem `\n` final | ✔ |
| 9 LX-V tokenizam até o fim; os 9 goldens = saída de referência (LX-V01 e LX-V07 escritos à mão antes) | ✔ |
| LX-V01 e LX-V02 têm goldens idênticos | ✔ |
| 9 LX-E: tipo, linha e sequência = `.erro.txt` (escritos à mão antes) = linha do manifest | ✔ |
| 19 SY-V e 5 SY-L: sem erro léxico, aceitos | ✔ |
| 20 SY-E: sem erro léxico, rejeitados, na linha prevista à mão no manifest | ✔ |
| P01–P91 percorridas por SY-V; 49 terminais cobertos | ✔ |
| nenhum `.c`, `.h`, `.py`, `.exe`, `.sh` no repositório | ✔ |

**Correção feita durante a fase:** todas as expectativas manuais coincidiram com a referência na primeira
execução, mas a tabela de cobertura mostrou que 13 palavras reservadas (`SE`, `ENTAO`, `SENAO`, `FIMSE`,
`ENQUANTO`, `FIMENQUANTO`, `PROCEDIMENTO`, `FIMPROCEDIMENTO`, `FUNCAO`, `FIMFUNCAO`, `RETORNE`, `VERDADEIRO`,
`FALSO`) só apareciam em testes sintáticos, sem golden léxico. Isso deixaria parte do catálogo do léxico sem
verificação direta na Fase F; por isso foi acrescentado o **LX-V09**. Nenhum teste existente foi alterado.

## 14. Pontos deliberadamente não testados ainda

| Ponto | Por quê | Quando |
|---|---|---|
| exit status em erro | AMB-13 aberta | Fases H/I |
| destino da saída (stdout/stderr) e nome do arquivo de tokens | AMB-14 | Fase F |
| arquivo de entrada com **CRLF** | o Git normaliza para LF no repositório; o teste será gerado na hora da execução (DEC-25) | Fases F/I |
| arquivo em Windows-1252 | AMB-08 (política de bytes) | Fase F |
| estouro de `NUM_INT`, lexemas muito longos | AMB-14 (limites) | Fases F/I |
| **linha do `TOKEN_EOF`** quando o arquivo termina com `\n` | o contrato não fixa se é a última linha com conteúdo ou a seguinte; SY-E19 evita a dúvida terminando sem `\n` (registrado em AMB-14) | Fase F/G |
| texto exato da mensagem de erro sintático e "esperado" | formato a definir na Fase G (só token e linha são exigidos) | Fase G |
| intercalação da listagem de tokens com o parser | DEC-04 (decisão da Fase H) | Fase H |
| árvore de derivação | AMB-07 | Fase G |
| arquivo vazio, arquivo inexistente, sem argumento | integração (`argc/argv`) | Fases H/I |

## 15. Critério de saída da Fase E

| Critério | Situação |
|---|---|
| matriz pronta (manifest + este documento) | ✔ |
| arquivos `.alg` prontos (62) | ✔ |
| goldens léxicos (9) e `.erro.txt` (9) prontos | ✔ |
| cobertura P01–P91 completa | ✔ |
| cobertura dos 49 terminais e de `LEX-01`…`LEX-18` | ✔ |
| validação descartável passou | ✔ |
| nenhuma alteração em P01–P91 nem no contrato léxico | ✔ |
| nenhum código | ✔ |

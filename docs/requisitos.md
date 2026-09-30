# Requisitos do Projeto 1 — Fase 1 (Análise Léxica e Sintática)

Disciplina: Compiladores — Profa. Daniela Cunha
Grupo: Gabriel Nottoli Buck, Julia Andrade, Joao vitor rocha miranda
Linguagem analisada: **MiniVisualg** (subconjunto simplificado do Visualg)
Linguagem de implementação: **C**

> **Fonte e estado de conferência.** Este documento nasceu na Fase A a partir do *Contexto Mestre*
> (resumo do enunciado, das aulas e do Anexo I). Na **Fase A.1** parte dele foi reconciliada com o
> PDF oficial *"PROJETO 1 – Fase 1 – Análise Léxica e Análise Sintática"*, conferido externamente
> pelo grupo. Itens marcados **✔PDF** foram confirmados contra o PDF nessa reconciliação; itens sem a
> marca vêm do Contexto Mestre e continuam **a conferir** antes da entrega.

## Legenda de origem

A origem é sempre o mais específica possível: *o que* sustenta o item e *onde*.

| Etiqueta | Significado |
|---|---|
| `ENUNCIADO — Objetivo` / `Etapa 1` / `Etapa 2` / `Etapa 3` / `Critério de Avaliação` / `Figura 2` | Exigência ou referência do PDF do Projeto 1, na seção indicada. |
| `ANEXO I — Variáveis` / `Operadores` / `Controle` / `Repetição` / `Vetores` / `Procedimentos` / `Funções` | Evidência de linguagem extraída dos exemplos do Anexo I, na seção indicada. |
| `AULA — <tema>` | Conteúdo/orientação das aulas; esperado, mas não necessariamente formal no enunciado. |
| `GRUPO` | Decisão do grupo (registrada em `decisoes.md`). Nunca atribuída à professora. |
| `AMBIGUIDADE` | Material contraditório ou lacunar; registrada em `decisoes.md` (Parte 2). |
| `✔PDF` | Item conferido contra o PDF original na Fase A.1. |

Só se cita a seção do PDF quando ela foi confirmada. Onde a seção exata ainda não foi anotada, a origem fica apenas `ENUNCIADO` (sem chute).

## Etapas e pesos

| Etapa | Peso | Conteúdo | Origem |
|---|---|---|---|
| 1 | 20% | Expressões regulares + Gramática Livre de Contexto | ENUNCIADO — Etapa 1 |
| 2 | 40% | Analisador léxico | ENUNCIADO — Etapa 2 |
| 3 | 40% | Analisador sintático | ENUNCIADO — Etapa 3 |

**Não entregar uma etapa zera o projeto** (ENUNCIADO). As três etapas são obrigatórias.

## Requisitos

### Geral / entrega

| ID | Requisito | Origem |
|---|---|---|
| REQ-01 | Implementar análise léxica e análise sintática do MiniVisualg. | ENUNCIADO |
| REQ-02 | Implementação em linguagem C. | ENUNCIADO |
| REQ-03 | O programa MiniVisualg analisado está em um único arquivo-fonte; sem pacotes; indentação sem significado. | ENUNCIADO |
| REQ-04 | As definições e estruturas dos exemplos do Anexo I são as **únicas** suportadas e devem ser seguidas rigorosamente. | ENUNCIADO |
| REQ-05 | Os nomes dos três integrantes aparecem em comentário no topo do arquivo-fonte. | ENUNCIADO |
| REQ-06 | Entrega = código + documentação, ou código muito bem comentado. | ENUNCIADO |
| REQ-07 | Existe um `readme.txt` com: até que parte o trabalho foi concluído; como executar; bugs/erros conhecidos; opcionalmente decisões de design. | ENUNCIADO |
| REQ-08 | Entrega conservadora do grupo: `compilador.c`, `documentacao.pdf`, `readme.txt`. | GRUPO |
| REQ-09 | A nota final de **cada etapa** depende da **entrega da documentação** e da **apresentação**. Consequência prática: toda decisão precisa ser defensável oralmente. | ENUNCIADO — Critério de Avaliação ✔PDF |
| REQ-14 | "Considerar que todos os lexemas no código fonte estão separados por um espaço." Esta regra **contradiz** exemplos do próprio Anexo I (ver AMB-01). | ENUNCIADO ✔PDF (contradição: AMBIGUIDADE AMB-01) |

### Compilação e execução

| ID | Requisito | Origem |
|---|---|---|
| REQ-10 | Comando de referência: `gcc -Wall -Wno-unused-result -g -Og compilador.c -o compilador` (MinGW + VS Code). | ENUNCIADO |
| REQ-11 | Não compila ou não executa → nota 0. Warnings geram penalidade. Retorno final adequado. | ENUNCIADO |
| REQ-12 | Meta operacional: 0 erros, 0 warnings. Nenhuma afirmação de "compila"/"passa" sem evidência de terminal. | GRUPO |
| REQ-13 | Nome do arquivo MiniVisualg vem da linha de comando (`argc/argv`), sem caminho fixo; falha de abertura é tratada. | ENUNCIADO (nome por linha de comando) + GRUPO (detalhes) |

### Análise léxica

| ID | Requisito | Origem |
|---|---|---|
| REQ-20 | "Cada token deve ser armazenado em um registro (struct) com um conjunto de campos de tipos diferentes para armazenar cada uma das informações referente ao token." | ENUNCIADO — Etapa 2 ✔PDF |
| REQ-21 | O PDF traz, na **Figura 2**, uma estrutura *ilustrativa* de token com: tipo, linha e `union` de atributos. É **referência de formato**, não vocabulário obrigatório (ver AMB-11). | ENUNCIADO — Figura 2 ✔PDF (referência ilustrativa) |
| REQ-22 | O analisador léxico deve **produzir um arquivo** com os tokens **e** apresentar **na tela** o mesmo resultado. Imprimir só na tela não basta. | ENUNCIADO ✔PDF |
| REQ-23 | Formato de saída: `Número da Linha do Átomo# NomeToken \| Atributo`. O próprio enunciado exemplifica com `11# IDENTIFICADOR \| 1` **e** `11# ID \| 1` (ver AMB-12). Nem todo token precisa de atributo significativo ("se necessário"). | ENUNCIADO ✔PDF (formato); GRUPO (quando há atributo) |
| REQ-24 | Erro léxico: exibir `ERRO LÉXICO`, a linha e a sequência incorreta; **finalizar todo o processamento**. Sem recuperação sofisticada. | ENUNCIADO |
| REQ-25 | Ignorar espaço em branco e comentários; controlar número de linha. | AULA — implementação do léxico |
| REQ-26 | Tabela de símbolos desde a análise léxica. Escopo do grupo: só identificadores; 1ª ocorrência cria índice, demais reutilizam. | AULA — tabela de símbolos (TS); GRUPO (escopo) |
| REQ-27 | Não basta "comparar com uma lista" para tokens de classe (ID, inteiro, real, string). | AULA — implementação do léxico |
| REQ-28 | **Utilizar os nomes de módulos sugeridos no Projeto 1.** Conhecidos com certeza: `nextToken()` (sintático) e `obterToken()` (léxico). Nenhum outro nome é obrigatório além dos que o PDF explicitar; não inventar. | ENUNCIADO — Etapa 2 e Etapa 3 ✔PDF |
| REQ-29 | **Decisão do grupo:** não copiar a struct da Figura 2 cegamente; adaptá-la aos tokens reais do MiniVisualg (ver DEC-06). Distinta de REQ-21, que é o que o enunciado oferece. | GRUPO |

### Análise sintática

| ID | Requisito | Origem |
|---|---|---|
| REQ-30 | Análise sintática **descendente**. | ENUNCIADO |
| REQ-31 | Erro sintático: exibir `ERRO SINTÁTICO`, o token incorreto e a linha; pode informar esperado/encontrado; **finalizar o processo**. | ENUNCIADO |
| REQ-32 | "O analisador léxico deve atender as demandas do analisador sintático. A interação […] se dá por meio da função `nextToken()` (analisador sintático) que realizará chamadas à função `obterToken()` (analisador léxico)." | **ENUNCIADO — Objetivo ✔PDF**. Reforçado por AULA — implementação do sintático. |
| REQ-33 | Parser **descendente recursivo preditivo LL(1)**, uma função por não-terminal, `lookahead` + `consome()`. | GRUPO (baseado fortemente nas aulas; **não** é exigência literal do enunciado) |
| REQ-34 | Registrar/gerar a derivação (árvore de derivação) sem montar uma AST complexa. | AULA — implementação do sintático (expectativa; ver AMB-07) |
| REQ-35 | Sem análise semântica e sem geração de código nesta fase. | GRUPO |

### Etapa 1 — documentos formais

| ID | Requisito | Origem |
|---|---|---|
| REQ-40 | Apresentar as expressões regulares da linguagem. | ENUNCIADO |
| REQ-41 | Apresentar a GLC correspondente. | ENUNCIADO |
| REQ-42 | Gramática sem recursão à esquerda e fatorada; FIRST/FOLLOW/nullable calculados; verificação LL(1). | AULA — análise descendente; GRUPO (verificação explícita) |

## Rastreabilidade: requisito → especificação léxica (Fase B)

Somente referências. **As origens acima não foram alteradas**; as decisões da Fase B continuam `GRUPO`.

| Requisito | Onde a Fase B o trata (`especificacao-lexica.md`) |
|---|---|
| REQ-14 (lexemas separados por espaço) | §2 princípio 5, §10 — scanner não depende de espaços (LEX-09) |
| REQ-20 (struct de token) | §14 — campos conceituais `type`, `line`, `lexeme`, atributo |
| REQ-21 (Figura 2, ilustrativa) | §14 — como a estrutura deriva da Figura 2 |
| REQ-22 (arquivo **e** tela) | §16 — mesma listagem nos dois destinos (LEX-17) |
| REQ-23 (formato `linha# Nome \| Atributo`) | §15, §16 — atributos por token; omissão de `\| atributo` quando não há (LEX-16, LEX-17) |
| REQ-24 (erro léxico) | §17 — situações, sequência reportada, formato da mensagem (LEX-13, LEX-18); exit status em AMB-13 |
| REQ-25 (ignorar espaço/comentários; contar linhas) | §9, §10 (LEX-08, LEX-09) |
| REQ-26 (tabela de símbolos) | §13 — só IDs, índice estável (LEX-15) |
| REQ-27 (não basta lista) | §5, §11 — catálogos + reconhecedores de classe (LEX-02..07) |
| REQ-28 (nomes de módulos) | §14 — `obterToken()` é a interface do léxico; nenhum outro nome fixado |
| REQ-29 (não copiar a Figura 2 cegamente) | §14 — adaptação ao MiniVisualg |
| REQ-32 (`nextToken()` → `obterToken()`) | §3.A, §16 — `TOKEN_EOF` como fronteira da interface (DEC-23) |
| REQ-40 (expressões regulares) | §4 — ERs de ID, NUM_INT, NUM_REAL, STRING; §9 — comentário |

## Rastreabilidade: requisito → gramática (Fase C)

Somente referências; origens inalteradas. As decisões da Fase C são `GRUPO` (DEC-28 a DEC-46).

| Requisito | Onde a Fase C o trata (`gramatica.md`) |
|---|---|
| REQ-03 (indentação sem significado) | §2, §8 — quebra de linha não separa comandos |
| REQ-04 (só as estruturas do Anexo I) | §6.1 (produção → evidência), §13 (cobertura), §15 (rejeitadas) |
| REQ-30 (análise descendente) | §1, §17 — sem recursão à esquerda, fatorada |
| REQ-33 (preditivo LL(1), decisão do grupo) | §12 (fatoração), §17 (pontos para a Fase D) |
| REQ-35 (sem semântica) | §16 — limitações sintáticas vs. semânticas |
| REQ-41 (GLC) | §6 — P01–P91 |
| REQ-42 (sem recursão à esquerda, fatorada; FIRST/FOLLOW) | §17 (auditoria feita); FIRST/FOLLOW na Fase D |

## Restrições de processo do grupo (todas as sessões)

Origem: `GRUPO`.

1. Nunca implementar requisito inexistente silenciosamente; lacuna → registrar ambiguidade.
2. Nunca usar Visualg externo como fonte de verdade.
3. Nunca afirmar que compila/passa sem executar.
4. Nunca alterar `main` diretamente; nunca fazer merge sem autorização; nunca `push --force`.
5. A branch `dev` não é fonte de verdade e não deve ser copiada.
6. Comentários explicam decisões, não narram linha a linha; código defensável oralmente em ~30 s por decisão (motivo reforçado por REQ-09).
7. Rastreabilidade: **material → regra → código → teste**.

## Prioridades (ordem)

1. Aderência ao enunciado → 2. aderência à abordagem da professora → 3. correção →
4. clareza → 5. capacidade de defender oralmente → 6. originalidade técnica onde houver liberdade.

Se uma decisão criativa prejudicar a aderência, é descartada.

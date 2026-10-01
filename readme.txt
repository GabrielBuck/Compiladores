PROJETO 1 - FASE 1 - ANALISE LEXICA E ANALISE SINTATICA (MiniVisualg)
Disciplina: Compiladores - Profa. Daniela Cunha

Integrantes:
  Gabriel Nottoli Buck
  Julia Andrade
  Joao vitor rocha miranda

------------------------------------------------------------
1. O QUE FOI CONCLUIDO
------------------------------------------------------------
[ATUALIZAR A CADA FASE]

Situacao atual: FASES A, A.1, B, C, D, E, F e G concluidas.

  Etapa 1 - ERs, GLC, FIRST/FOLLOW, LL(1) ...... CONCLUIDA
  Etapa 2 - Analisador lexico .................. IMPLEMENTADO E TESTADO
            (compilador.c; suite lexica: 9/9 validos, 9/9 erros;
             44/44 arquivos sintaticos sem erro lexico)
  Etapa 3 - Analisador sintatico ............... IMPLEMENTADO E TESTADO
            (19/19 validos aceitos; 20/20 erros no primeiro token
             e linha esperados; 5/5 limitacoes aceitas)

O programa executa as analises lexica e sintatica integradas. O parser
pede cada token ao lexico sob demanda, mostra os tokens na tela, grava a
listagem em tokens.txt e escreve a arvore de derivacao em arvore.txt.

------------------------------------------------------------
2. COMO COMPILAR E EXECUTAR
------------------------------------------------------------
Compilar (MinGW):
  gcc -Wall -Wno-unused-result -g -Og compilador.c -o compilador
(0 erros e 0 warnings com o gcc 15.2.0 do MSYS2.)

Executar (o nome do arquivo MiniVisualg e passado na linha de comando):
  compilador.exe programa.alg

Arquivos gerados:
  tokens.txt  (no diretorio atual; sobrescrito a cada execucao;
               contem exatamente o que e mostrado na tela)
  arvore.txt  (arvore de derivacao textual em pre-ordem; sobrescrita;
               em erro sintatico fica parcial e marca <ERRO SINTATICO>)

Formato de cada linha:
  <linha># <NOME>                 ex.: 1# ALGORITMO
  <linha># <NOME> | <atributo>    ex.: 4# ID | 1   /   5# OP_REL | GE

Erro lexico (mostrado na tela e gravado em tokens.txt):
  ERRO LÉXICO - linha <n> - sequência: <sequencia>
O processamento termina no primeiro erro.

Erro sintatico (mostrado somente na tela):
  ERRO SINTÁTICO - linha <n> - token: <NOME> - esperado: <...>
O processamento termina no primeiro erro; tokens.txt continua contendo
somente a saida lexica.

Problemas de uso (sem argumento, argumento extra, arquivo inexistente)
sao mostrados em stderr e o programa termina com status diferente de 0.

------------------------------------------------------------
3. BUGS E ERROS CONHECIDOS
------------------------------------------------------------
- Codigo de retorno em erro lexico ou sintatico: PROVISORIAMENTE 0 (o criterio de
  avaliacao penaliza retorno diferente de 0). O erro sintatico usa a mesma
  constante. Valor a confirmar com a professora; a politica esta
  centralizada em uma constante no codigo.
- Numeros fora da faixa de long long (inteiros) ou de double (reais)
  sao reportados como "ERRO INTERNO" com status diferente de 0: e uma
  limitacao de representacao do C, nao uma regra do MiniVisualg.
- Acentos: o programa le BYTES. Acentos dentro de strings e comentarios
  sao aceitos (UTF-8 ou Latin-1). Fora deles sao erro lexico. No console
  do Windows, o acento do erro pode aparecer diferente (code page);
  tokens.txt guarda os bytes exatos.
- O comando oficial, sem -std, funciona. O codigo usa recursos padronizados
  em C11 (_Static_assert). O gcc 15.2.0 testado tambem o aceitou em modos
  GNU anteriores compativeis, como gnu99, por extensao do compilador;
  nao foi testado em outros compiladores.
- A memoria foi revisada manualmente; nao houve execucao sob ferramenta
  de memoria (AddressSanitizer indisponivel neste MinGW).

------------------------------------------------------------
4. DECISOES DE DESIGN (resumo)
------------------------------------------------------------
- Linguagem C pura, sem geradores de analisadores nem bibliotecas externas.
- Analisador lexico caractere a caractere: lexemas fixos em tabelas e
  reconhecedores proprios para identificadores, numeros e strings;
  escolha do maior casamento (maximal munch).
- Sensivel a maiusculas/minusculas. "<" e ">" isolados e "." isolado
  sao erro lexico.
- Tabela de simbolos so de identificadores, sem limite fixo.
- Analisador sintatico descendente recursivo preditivo LL(1) (decisao do
  grupo; a gramatica ja foi validada como LL(1)). O sintatico pede tokens
  ao lexico sob demanda: nextToken() chama obterToken(), conforme o
  enunciado. Ha uma funcao por nao-terminal e um unico lookahead.
- Producoes vazias so sao escolhidas quando o lookahead pertence ao SELECT
  formal da producao epsilon; nao existe epsilon como caso default.
- A arvore de derivacao textual registra nao-terminais, P01-P91, terminais
  e epsilon, sem AST nem analise semantica.
- Gramatica: procedimentos/funcoes declarados antes de var/inicio; operador
  OU aceito no mesmo nivel de E; sem subtracao ("-" so em "passo -2").
- Detalhes e ambiguidades do material: docs/decisoes.md no repositorio;
  arquitetura do analisador lexico: docs/arquitetura.md.

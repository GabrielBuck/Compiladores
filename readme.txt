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

Situacao atual: FASES A, A.1, B (especificacao lexical) e C (gramatica)
concluidas - SOMENTE DOCUMENTACAO.

  Etapa 1 - Expressoes regulares ............... CONCLUIDAS (docs/especificacao-lexica.md)
  Etapa 1 - GLC ................................ CONCLUIDA (docs/gramatica.md)
  Etapa 1 - Validacao LL(1) / FIRST / FOLLOW ... PENDENTE (proxima fase)
  Etapa 2 - Analisador lexico .................. NAO IMPLEMENTADO
  Etapa 3 - Analisador sintatico ............... NAO IMPLEMENTADO

Nao existe ainda codigo executavel (compilador.c).

------------------------------------------------------------
2. COMO COMPILAR E EXECUTAR
------------------------------------------------------------
(Valido quando compilador.c existir.)

Compilar (MinGW):
  gcc -Wall -Wno-unused-result -g -Og compilador.c -o compilador

Executar (o nome do arquivo MiniVisualg e passado na linha de comando):
  compilador.exe programa.alg

------------------------------------------------------------
3. BUGS E ERROS CONHECIDOS
------------------------------------------------------------
Nenhum codigo implementado ate o momento.

------------------------------------------------------------
4. DECISOES DE DESIGN (resumo)
------------------------------------------------------------
- Linguagem C pura, sem geradores de analisadores nem bibliotecas externas.
- Analisador lexico caractere a caractere: lexemas fixos em tabela e
  reconhecedores proprios para identificadores, numeros e strings.
- Analisador sintatico descendente recursivo preditivo LL(1) (decisao do
  grupo). O sintatico pede tokens ao lexico sob demanda: nextToken() chama
  obterToken(), conforme o enunciado.
- Erro lexico ou sintatico encerra o processamento. O codigo de retorno do
  processo nesse caso ainda nao foi decidido (ponto a confirmar).
- Analisador lexico sensivel a maiusculas/minusculas.
- Gramatica: procedimentos/funcoes declarados antes de var/inicio; operador
  OU aceito no mesmo nivel de E; sem subtracao ("-" so em "passo -2").
- Detalhes e ambiguidades do material: docs/decisoes.md no repositorio.

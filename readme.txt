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

Situacao atual: FASES A, A.1, B, C, D e E concluidas - DOCUMENTACAO E
CASOS DE TESTE. O compilador ainda nao existe; nenhum teste foi executado
contra ele.

  Etapa 1 - ERs, GLC, FIRST/FOLLOW, LL(1) ...... CONCLUIDA
  Etapa 2 - Analisador lexico .................. NAO IMPLEMENTADO
            (casos de teste prontos em testes/lexico/)
  Etapa 3 - Analisador sintatico ............... NAO IMPLEMENTADO
            (casos de teste prontos em testes/sintatico/)

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

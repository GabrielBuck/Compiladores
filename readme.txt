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

Situacao atual: FASE A (requisitos e especificacao) concluida.

  Etapa 1 - Expressoes regulares e GLC ......... NAO INICIADA
  Etapa 2 - Analisador lexico .................. NAO INICIADA
  Etapa 3 - Analisador sintatico ............... NAO INICIADA

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
- Analisador sintatico descendente recursivo preditivo LL(1); o sintatico
  pede tokens ao lexico sob demanda (nextToken -> obterToken).
- Erro lexico ou sintatico encerra o processamento.
- Detalhes e ambiguidades do material: docs/decisoes.md no repositorio.

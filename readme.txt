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

Situacao atual: FASE A (requisitos), FASE A.1 (conferencia com o PDF
oficial) e FASE B (especificacao lexical) concluidas - SOMENTE DOCUMENTACAO.

  Etapa 1 - Expressoes regulares ............... ESPECIFICADAS (docs/especificacao-lexica.md)
  Etapa 1 - GLC ................................ NAO INICIADA
  Especificacao lexical (Fase B) ............... CONCLUIDA (documentalmente)
  Etapa 2 - Implementacao do analisador lexico . NAO INICIADA
  Etapa 3 - Analisador sintatico (parser) ...... NAO INICIADO

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
- Detalhes e ambiguidades do material: docs/decisoes.md no repositorio.

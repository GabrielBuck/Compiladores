# Gramática (Em construção)

A gramática precisa ser adaptada para a classe LL(1):
- Livre de ambiguidades.
- Sem recursão à esquerda.
- Fatorada (para garantir que a escolha da produção possa ser feita apenas com 1 token de lookahead).

## Categorias a cobrir
- Programa principal
- Seção de variáveis
- Subprogramas (procedimentos, funções)
- Declarações (se, enquanto, para, atribuição, chamadas, retornos)
- Expressões (lógicas, relacionais, aritméticas, acesso a vetor)

## Esboço Preliminar
*Nota: A construção completa da GLC LL(1) será realizada na próxima etapa do projeto.*

`Programa -> algoritmo STRING VarDecl Subprogramas inicio Comandos fimalgoritmo`
`VarDecl -> var DeclList | epsilon`

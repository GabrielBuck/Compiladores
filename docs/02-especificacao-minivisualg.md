# Especificação do MiniVisualg

MiniVisualg é um subconjunto da linguagem Visualg, focada na disciplina de Compiladores.

## Estrutura de um Algoritmo
```
algoritmo "Nome"
var
...
inicio
...
fimalgoritmo
```

## Comentários
`//` introduz um comentário de linha única.

## Tipos
- inteiro
- real
- caractere
- logico

## Valores Lógicos
- verdadeiro
- falso

## Entrada e Saída
- leia(...)
- escreva(...)
- escreval(...)

## Atribuição
`<-`

## Operadores Aritméticos
`+`, `-`, `*`, `/`, `\`, `MOD`

## Operadores Relacionais
`=`, `<>`, `<`, `>`, `<=`, `>=`

## Operadores Lógicos
`E`, `OU`

## Estruturas Condicionais
```
se (...) entao
...
fimse

se (...) entao
...
senao
...
fimse
```

## Laços de Repetição
```
para id de exp ate exp faca
...
fimpara

para id de exp ate exp passo exp faca
...
fimpara

enquanto (...) faca
...
fimenquanto
```

## Vetores
`vetor[1..3] de caractere`
Acesso: `nomes[1]`

## Procedimentos e Funções
```
procedimento nome(parametros)
inicio
...
fimprocedimento

funcao nome(parametros): tipo
inicio
retorne ...
fimfuncao
```

algoritmo "Reservadas"
procedimento aviso
inicio
escreval("aviso")
fimprocedimento
funcao eh_par(v: inteiro): logico
inicio
se (v MOD 2 = 0) entao
retorne verdadeiro
senao
retorne falso
fimse
fimfuncao
var
n: inteiro
inicio
n <- 0
enquanto (n <= 3) faca
aviso
n <- n + 1
fimenquanto
fimalgoritmo

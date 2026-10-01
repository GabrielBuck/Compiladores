algoritmo "Vetores"
var
nomes: vetor[1..3] de caractere
notas: vetor[1..4] de real
soma: real
i: inteiro
inicio
nomes[1] <- "Ana"
nomes[2] <- "Carlos"
nomes[3] <- "João"
escreval(nomes[2])
soma <- 0
para i de 1 ate 4 faca
leia(notas[i])
soma <- soma + notas[i]
fimpara
escreval(soma / 4)
fimalgoritmo

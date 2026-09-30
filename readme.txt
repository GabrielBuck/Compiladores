Projeto 1 - Compiladores - MiniVisualg

Integrantes:
- Gabriel Nottoli Buck
- Julia Andrade
- Joao vitor rocha miranda

Estágio atual: 
Fundação: compilada e testada.
Scanner: não implementado.
Parser: não implementado.

Como compilar:
gcc -Wall -Wno-unused-result -g -Og compilador.c -o compilador

Como executar:
./compilador testes/validos/minimo.alg
(No Windows: .\compilador.exe testes\validos\minimo.alg)

Decisões relevantes e Limitações:
- O scanner e parser ainda estão sob construção e no momento o programa relata explicitamente que é uma versão não funcional (stub).
- Todo o código está contido em compilador.c para simplificar a avaliação.

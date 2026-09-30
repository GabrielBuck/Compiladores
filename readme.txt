Projeto 1 - Compiladores - MiniVisualg

Integrantes:
- Gabriel Nottoli Buck
- Julia Andrade
- Joao vitor rocha miranda

Estágio atual: Estruturação base. O compilador ainda não realiza a análise completa, sendo apenas o esqueleto inicial com as estruturas de Token, Tabela de Símbolos preparadas.

Como compilar:
gcc -Wall -Wno-unused-result -g -Og compilador.c -o compilador

Como executar:
./compilador testes/validos/minimo.alg
(No Windows: .\compilador.exe testes\validos\minimo.alg)

Decisões relevantes:
- Adotado o modelo de leitura caractere a caractere no léxico para tratar corretamente pontuação justaposta a identificadores, apesar do enunciado sugerir separação por espaço.
- Todo o código contido em compilador.c para simplificar.

Bugs/Limitações:
- O scanner e parser ainda estão sob construção e imprimirão mensagens correspondentes.

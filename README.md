# Compilador MiniVisualg

Este é o **Projeto 1** da disciplina de Compiladores.

**Integrantes:**
- Gabriel Nottoli Buck
- Julia Andrade
- Joao vitor rocha miranda

## O que é o projeto
Desenvolvimento das duas primeiras fases do front-end de um compilador (Análise Léxica e Análise Sintática) para a linguagem **MiniVisualg**, um subconjunto simplificado da linguagem Visualg. O projeto é escrito totalmente em C e exige um parser descendente. O grupo adotará o parser descendente recursivo preditivo LL(1) com base na metodologia ensinada em aula. A análise léxica e sintática também será manual, sem a utilização de Flex/Bison/Yacc, para aderência à metodologia.

## Estado Atual
Fundação compilada/testada.
Especificação lexical em consolidação.
Scanner ainda não implementado.
Parser ainda não implementado.

## Estrutura do Repositório
- `compilador.c`: Arquivo principal com todo o código fonte.
- `docs/`: Especificações e documentações rigorosas para rastreabilidade de evidências.
- `testes/`: Casos de teste válidos e com erros intencionais.

## Compilação e Execução
Compilação obrigatória:
```bash
gcc -Wall -Wno-unused-result -g -Og compilador.c -o compilador
```

Execução:
```bash
./compilador testes/validos/minimo.alg
```

*(No Windows, utilize `.\compilador.exe testes\validos\minimo.alg`)*
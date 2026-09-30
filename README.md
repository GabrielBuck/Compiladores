# Compilador MiniVisualg

Este é o **Projeto 1** da disciplina de Compiladores.

**Integrantes:**
- Gabriel Nottoli Buck
- Julia Andrade
- Joao vitor rocha miranda

## O que é o projeto
Desenvolvimento das duas primeiras fases do front-end de um compilador (Análise Léxica e Análise Sintática descendente recursiva LL(1)) para a linguagem **MiniVisualg**, um subconjunto simplificado da linguagem Visualg. O projeto é escrito totalmente em C (sem a utilização de Flex/Bison).

## Estado Atual
Fase 0: Preparação da infraestrutura. Compilador criado e estruturado sem regras de análise completas.

## Estrutura do Repositório
- `compilador.c`: Arquivo principal com todo o código fonte.
- `docs/`: Especificações e documentações.
- `testes/`: Casos de teste válidos e com erros intencionais.

## Compilação e Execução
Compilação obrigatória sem warnings:
```bash
gcc -Wall -Wno-unused-result -g -Og compilador.c -o compilador
```

Execução:
```bash
./compilador testes/validos/minimo.alg
```

*(No Windows, utilize `.\compilador.exe testes\validos\minimo.alg`)*
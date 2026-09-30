# Contexto e Requisitos do Projeto

## Objetivo Acadêmico
Projeto 1 da disciplina de Compiladores.
Objetivo: Implementar as duas primeiras etapas do front-end de um compilador (análise léxica e análise sintática) para a linguagem MiniVisualg, um subconjunto do Visualg.
Linguagem de implementação: C.
Não é permitido o uso de Flex, Bison, Yacc, ou geradores de analisadores semelhantes.

## Integrantes
- Gabriel Nottoli Buck
- Julia Andrade
- Joao vitor rocha miranda

## Requisitos
1. **Léxico**: Reconhecer tokens e exibi-los no formato `Linha# TOKEN | ATRIBUTO`. Gerar arquivo de saída.
2. **Sintático**: Análise descendente preditiva LL(1).
3. **Erros**:
   - Léxico: Exibir "ERRO LÉXICO", linha e sequência. Interromper.
   - Sintático: Exibir "ERRO SINTÁTICO", linha e token. Interromper.
4. **Interação**: O parser chama `nextToken()`, que por sua vez obtém o token através de `obterToken()`.

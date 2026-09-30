# Contexto e Requisitos do Projeto

## Objetivo Acadêmico
Projeto 1 da disciplina de Compiladores.
Objetivo: Implementar as duas primeiras etapas do front-end de um compilador (análise léxica e análise sintática) para a linguagem MiniVisualg, um subconjunto restrito do Visualg. Somente as definições e estruturas utilizadas nos exemplos do Anexo I fazem parte do MiniVisualg deste projeto e devem ser seguidas rigorosamente.

## Integrantes
- Gabriel Nottoli Buck
- Julia Andrade
- Joao vitor rocha miranda

## Requisitos Obrigatórios do Enunciado
1. **Linguagem C**: Todo o código deve ser em C.
2. **Análise Léxica**: Reconhecer tokens, com tratamento para falhas.
3. **Análise Sintática Descendente**: O compilador usará uma abordagem descendente para a sintaxe.
4. **Interação**: O parser chama `nextToken()`, que por sua vez obtém o token através de `obterToken()`.
5. **Estrutura Token**: Uso de struct para representar os tokens.
6. **Saída em Arquivo e Tela**: Saída de tokens formatada no console e em arquivo (`NúmeroDaLinha# NomeToken | Atributo`).
7. **Tratamento de Erro Léxico**: Exibir "ERRO LÉXICO", linha, sequência inválida, e interromper a execução.
8. **Tratamento de Erro Sintático**: Exibir "ERRO SINTÁTICO", linha, token incorreto, e interromper a execução.
9. **Leitura por Linha de Comando**: O arquivo fonte é fornecido como argumento (ex: `argv[1]`).

## Decisões Arquiteturais do Grupo
1. **Sintático LL(1) Recursivo Preditivo**: Nossa escolha baseada fortemente na metodologia ensinada pela Profa. Daniela Cunha.
2. **Sem Geradores de Parser**: O grupo optou por implementação manual, sem Flex, Bison ou Yacc, por aderência ao método trabalhado nas aulas.
3. **Arquivo Único**: Manter `compilador.c` como arquivo principal único é decisão nossa para compatibilidade total com o comando de compilação fornecido pela professora.

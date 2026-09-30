# Gramática (Fase de Planejamento)

A Gramática Livre de Contexto completa do MiniVisualg será desenhada na FASE 2. No momento atual, apenas estruturamos as necessidades e restrições obrigatórias para um parser LL(1).

## Restrições e Natureza LL(1)
- **Livre de Ambiguidades**: Para qualquer não-terminal e um token de lookahead da entrada, no máximo uma produção pode ser disparada.
- **Sem Recursão à Esquerda**: Padrões como `Expr -> Expr + Termo` deverão ser adaptados (usualmente convertidos para iteração de bloco ou recursão à direita fatorada `Expr -> Termo ExprL`).
- **Fatoração**: Regras que se iniciam com os mesmos terminais (ex: chamadas `procedimento(...)` vs vetores ou blocos semelhantes) precisam ter o fator comum evidenciado em um prefixo para manter o lookahead unívoco.

## Componentes a Cobrir Baseados nas Evidências
- **Alojamento de Subprogramas**: Como `var` se relaciona a blocos de subprogramas.
- **Tipos de Dados e Variáveis**: Declaração de `inteiro`, `real`, `caractere`, `logico`, vetores.
- **Comandos de Repetição e Desvio**: `se/entao/senao`, `para/ate/passo`, `enquanto`.
- **E/S e Atribuições**: `leia`, `escreva/escreval`, `<-`.

## Expressões Reais Encontradas no Anexo I
A gramática de expressões e a precedência deverão comportar estritamente estes padrões:
1. `n1 + n2`, `contador + 1`
2. `n1 * n2`
3. `n1 / n2`, `soma / 4`
4. `n1 \ n2`
5. `idade >= 18`, `contador <= 5`
6. `senhaDigitada = 1234`, `nome <> "João"`
7. `(idade >= 12) E (altura >= 1.50)`
8. `v MOD 2 = 0`
9. `somar(10, 5)`, `eh_par(num)`
10. `notas[i]`

## Decisões e Ambiguidades Pendentes
- A posição correta de `procedimento` e `funcao` na hierarquia do arquivo (se aninhados localmente, logo após o `algoritmo`, no meio do `var`, etc.) devido à disposição visual atípica nos exemplos originais.
- A precedência final do `MOD`, `\`, `E`, `OU` frente aos operadores aritméticos clássicos no contexto do Visualg.

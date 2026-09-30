# Matriz de Evidências

Este documento lista todas as construções e lexemas do MiniVisualg e classifica o nível de evidência disponível no Anexo I.

| ID | Construção/Lexema | Categoria | Evidência no Anexo I | Status | Observação |
|---|---|---|---|---|---|
| EV-001 | `algoritmo` | palavra reservada | `algoritmo "Nome"` | CONFIRMADO | Estrutura básica |
| EV-002 | `var` | palavra reservada | bloco `var` | CONFIRMADO | Estrutura básica |
| EV-003 | `inicio` | palavra reservada | bloco `inicio` | CONFIRMADO | Estrutura básica |
| EV-004 | `fimalgoritmo` | palavra reservada | `fimalgoritmo` no final | CONFIRMADO | Estrutura básica |
| EV-005 | `inteiro` | tipo | `a: inteiro` | CONFIRMADO | Tipo primitivo |
| EV-006 | `real` | tipo | tipo real listado | MENCIONADO | Sem snippet explícito de uso da palavra, mas mencionado nos tipos |
| EV-007 | `caractere` | tipo | `vetor[1..3] de caractere` | CONFIRMADO | Tipo primitivo |
| EV-008 | `logico` | tipo | `funcao eh_par(v: inteiro): logico` | CONFIRMADO | Tipo primitivo |
| EV-009 | `verdadeiro` | palavra reservada / literal | listado como valor | MENCIONADO | Mencionado no material de apoio |
| EV-010 | `falso` | palavra reservada / literal | listado como valor | MENCIONADO | Mencionado no material de apoio |
| EV-011 | `leia` | palavra reservada | `leia(nome)` | CONFIRMADO | Comando de I/O |
| EV-012 | `escreva` | palavra reservada | comando de saída | MENCIONADO | Mencionado no texto |
| EV-013 | `escreval` | palavra reservada | `escreval("Olá, mundo!")` | CONFIRMADO | Comando de I/O |
| EV-014 | `se` | palavra reservada | `se (...) entao` | CONFIRMADO | Estrutura condicional |
| EV-015 | `entao` | palavra reservada | `se (...) entao` | CONFIRMADO | Estrutura condicional |
| EV-016 | `senao` | palavra reservada | bloco `senao` | CONFIRMADO | Estrutura condicional |
| EV-017 | `fimse` | palavra reservada | `fimse` | CONFIRMADO | Estrutura condicional |
| EV-018 | `para` | palavra reservada | `para identificador ...` | CONFIRMADO | Laço de repetição |
| EV-019 | `de` | palavra reservada | `de expressão ate` | CONFIRMADO | Laço de repetição |
| EV-020 | `ate` | palavra reservada | `ate expressão` | CONFIRMADO | Laço de repetição |
| EV-021 | `passo` | palavra reservada | `passo -2` | CONFIRMADO | Laço de repetição (opcional) |
| EV-022 | `faca` | palavra reservada | `faca` | CONFIRMADO | Laço de repetição |
| EV-023 | `fimpara` | palavra reservada | `fimpara` | CONFIRMADO | Laço de repetição |
| EV-024 | `enquanto` | palavra reservada | `enquanto (...) faca` | CONFIRMADO | Laço de repetição |
| EV-025 | `fimenquanto` | palavra reservada | `fimenquanto` | CONFIRMADO | Laço de repetição |
| EV-026 | `vetor` | palavra reservada | `vetor[1..3] de caractere` | CONFIRMADO | Declaração de array |
| EV-027 | `procedimento` | palavra reservada | `procedimento nome` | CONFIRMADO | Subprograma |
| EV-028 | `fimprocedimento` | palavra reservada | `fimprocedimento` | CONFIRMADO | Subprograma |
| EV-029 | `funcao` | palavra reservada | `funcao somar(...)` | CONFIRMADO | Subprograma com retorno |
| EV-030 | `fimfuncao` | palavra reservada | `fimfuncao` | CONFIRMADO | Subprograma com retorno |
| EV-031 | `retorne` | palavra reservada | `retorne a + b` | CONFIRMADO | Retorno de função |
| EV-032 | `MOD` | operador (palavra) | `v MOD 2 = 0` | CONFIRMADO | Operador aritmético |
| EV-033 | `E` | operador (palavra) | `(idade >= 12) E ...` | CONFIRMADO | Operador lógico |
| EV-034 | `OU` | operador (palavra) | mencionado textualmente | MENCIONADO | Operador lógico |
| EV-035 | `<-` | operador | `resultado <- somar(10, 5)` | CONFIRMADO | Atribuição |
| EV-036 | `+` | operador | `n1 + n2`, `contador + 1` | CONFIRMADO | Adição |
| EV-037 | `-` | operador | `n1 - n2`, `passo -2` | CONFIRMADO | Subtração / Menos Unário |
| EV-038 | `*` | operador | `n1 * n2` | CONFIRMADO | Multiplicação |
| EV-039 | `/` | operador | `n1 / n2` | CONFIRMADO | Divisão |
| EV-040 | `\` | operador | `n1 \ n2` | CONFIRMADO | Divisão inteira |
| EV-041 | `=` | operador | `senhaDigitada = 1234` | CONFIRMADO | Relacional (igualdade) |
| EV-042 | `<>` | operador | `nome <> "João"` | CONFIRMADO | Relacional (diferença) |
| EV-043 | `<=` | operador | `contador <= 5` | CONFIRMADO | Relacional |
| EV-044 | `>=` | operador | `idade >= 18` | CONFIRMADO | Relacional |
| EV-045 | `<` | operador | citado genericamente | AMBÍGUO | Não há snippet de uso isolado do `<` no material real |
| EV-046 | `>` | operador | citado genericamente | AMBÍGUO | Não há snippet de uso isolado do `>` no material real |
| EV-047 | `(` | pontuação | `(idade >= 12)` | CONFIRMADO | Agrupamento |
| EV-048 | `)` | pontuação | `(idade >= 12)` | CONFIRMADO | Agrupamento |
| EV-049 | `[` | pontuação | `notas[i]` | CONFIRMADO | Acesso a vetor |
| EV-050 | `]` | pontuação | `notas[i]` | CONFIRMADO | Acesso a vetor |
| EV-051 | `:` | pontuação | `a: inteiro` | CONFIRMADO | Declaração de tipo |
| EV-052 | `,` | pontuação | `somar(10, 5)` | CONFIRMADO | Separador |
| EV-053 | `..` | pontuação | `vetor[1..3]` | CONFIRMADO | Intervalo |
| EV-054 | `"` | pontuação | `"Olá, mundo!"` | CONFIRMADO | Delimitador de string |
| EV-055 | `//` | comentário | `// comentário` | CONFIRMADO | Comentário de linha |
| EV-056 | Identificadores | identificador | `nome`, `portaAberta` | CONFIRMADO | Expressão Regular precisa comportar os exemplos |
| EV-057 | Int. Numérico | literal | `0`, `1234`, `-2` | CONFIRMADO | Literal inteiro (sem sinal léxico intrínseco, cf. DEC-006) |
| EV-058 | Real Numérico | literal | `1.60`, `1.50` | CONFIRMADO | Literal de ponto flutuante |

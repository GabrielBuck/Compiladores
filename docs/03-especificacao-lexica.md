# Especificação Léxica

## Categorias de Tokens
1. **Palavras Reservadas (Keywords)**: `algoritmo`, `var`, `inicio`, `fimalgoritmo`, `inteiro`, `real`, `caractere`, `logico`, `verdadeiro`, `falso`, `leia`, `escreva`, `escreval`, `se`, `entao`, `senao`, `fimse`, `para`, `de`, `ate`, `passo`, `faca`, `fimpara`, `enquanto`, `fimenquanto`, `vetor`, `procedimento`, `fimprocedimento`, `funcao`, `fimfuncao`, `retorne`.
2. **Identificadores (ID)**: Sequências alfanuméricas começando por letra, podendo conter `_`.
3. **Números (INT, REAL)**: Literais numéricos sem sinal (sinal unário tratado na sintaxe).
4. **Cadeia de Caracteres (STRING)**: Textos entre aspas duplas `"..."`.
5. **Operadores e Pontuação**: `+`, `-`, `*`, `/`, `\`, `MOD`, `<`, `>`, `<=`, `>=`, `=`, `<>`, `<-`, `(`, `)`, `[`, `]`, `:`, `,`, `..`.

## Lexemas Fixos vs. Tokens de Classe
- Palavras reservadas serão identificadas após a leitura de um identificador comum (verificação na Tabela de Símbolos ou função de hash/comparação).
- `INT` e `REAL` têm regras específicas (Regex: `[0-9]+` e `[0-9]+ \. [0-9]+`).
- `STRING`: `"[^"]*"`
- `ID`: `[a-zA-Z] [a-zA-Z0-9_]*`

## Tabela de Símbolos (TS)
Na primeira ocorrência de um identificador, ele é incluído na TS, ganhando um índice. Esse índice compõe o atributo de `ID`.

## Espaços e Delimitadores
A separação de tokens pode se dar por espaços, tabulações, ou quebras de linha (` `, `\t`, `\n`). Além disso, pontuações (`(`, `)`, `+`, etc.) atuam como delimitadores naturais de identificadores e números.

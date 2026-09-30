# Especificação Léxica

## 1. Tokens de Classe

| Token | ER / Forma | Evidência | Atributo | Estratégia |
|---|---|---|---|---|
| `TOKEN_ID` | `[A-Za-z][A-Za-z0-9_]*` | `nome`, `portaAberta`, `eh_par` | Índice na TS | Acumular no buffer; checar catálogo fixo; se não for palavra reservada, alocar/reutilizar na TS e retornar `TOKEN_ID`. |
| `TOKEN_NUM_INT` | `[0-9]+` | `0`, `1234` | Valor Inteiro | Sequência de dígitos. O sinal negativo não faz parte do literal léxico (DEC-006). |
| `TOKEN_NUM_REAL`| `[0-9]+\.[0-9]+` | `1.50`, `1.60` | Valor Flutuante | Sequência de dígitos seguida de ponto obrigatório e mais dígitos. |
| `TOKEN_STRING` | `"[^"\n]*"` | `"João"`, `"Olá, mundo!"` | (Não exigido ainda) | Inicia com `"`. Acumula até `"`. Se encontrar quebra de linha ou EOF antes de fechar, gerar ERRO LÉXICO (DEC-005). |

## 2. Lexemas Fixos (Palavras Reservadas e Pontuação)

| Lexema | Token interno | Categoria | Status |
|---|---|---|---|
| algoritmo | `TOKEN_KW_ALGORITMO` | palavra reservada | CONFIRMADO |
| var | `TOKEN_KW_VAR` | palavra reservada | CONFIRMADO |
| inicio | `TOKEN_KW_INICIO` | palavra reservada | CONFIRMADO |
| fimalgoritmo | `TOKEN_KW_FIMALGORITMO` | palavra reservada | CONFIRMADO |
| inteiro | `TOKEN_KW_INTEIRO` | tipo | CONFIRMADO |
| real | `TOKEN_KW_REAL` | tipo | MENCIONADO |
| caractere | `TOKEN_KW_CARACTERE` | tipo | CONFIRMADO |
| logico | `TOKEN_KW_LOGICO` | tipo | CONFIRMADO |
| verdadeiro | `TOKEN_KW_VERDADEIRO` | palavra reservada / literal | MENCIONADO |
| falso | `TOKEN_KW_FALSO` | palavra reservada / literal | MENCIONADO |
| leia | `TOKEN_KW_LEIA` | palavra reservada | CONFIRMADO |
| escreva | `TOKEN_KW_ESCREVA` | palavra reservada | MENCIONADO |
| escreval | `TOKEN_KW_ESCREVAL` | palavra reservada | CONFIRMADO |
| se | `TOKEN_KW_SE` | palavra reservada | CONFIRMADO |
| entao | `TOKEN_KW_ENTAO` | palavra reservada | CONFIRMADO |
| senao | `TOKEN_KW_SENAO` | palavra reservada | CONFIRMADO |
| fimse | `TOKEN_KW_FIMSE` | palavra reservada | CONFIRMADO |
| para | `TOKEN_KW_PARA` | palavra reservada | CONFIRMADO |
| de | `TOKEN_KW_DE` | palavra reservada | CONFIRMADO |
| ate | `TOKEN_KW_ATE` | palavra reservada | CONFIRMADO |
| passo | `TOKEN_KW_PASSO` | palavra reservada | CONFIRMADO |
| faca | `TOKEN_KW_FACA` | palavra reservada | CONFIRMADO |
| fimpara | `TOKEN_KW_FIMPARA` | palavra reservada | CONFIRMADO |
| enquanto | `TOKEN_KW_ENQUANTO` | palavra reservada | CONFIRMADO |
| fimenquanto | `TOKEN_KW_FIMENQUANTO` | palavra reservada | CONFIRMADO |
| vetor | `TOKEN_KW_VETOR` | palavra reservada | CONFIRMADO |
| procedimento | `TOKEN_KW_PROCEDIMENTO` | palavra reservada | CONFIRMADO |
| fimprocedimento| `TOKEN_KW_FIMPROCEDIMENTO`| palavra reservada | CONFIRMADO |
| funcao | `TOKEN_KW_FUNCAO` | palavra reservada | CONFIRMADO |
| fimfuncao | `TOKEN_KW_FIMFUNCAO` | palavra reservada | CONFIRMADO |
| retorne | `TOKEN_KW_RETORNE` | palavra reservada | CONFIRMADO |
| MOD | `TOKEN_OP_MOD` | palavra / operador | CONFIRMADO |
| E | `TOKEN_OP_E` | palavra / operador | CONFIRMADO |
| OU | `TOKEN_OP_OU` | palavra / operador | MENCIONADO |
| <- | `TOKEN_ATRIBUICAO` | operador | CONFIRMADO |
| + | `TOKEN_MAIS` | operador | CONFIRMADO |
| - | `TOKEN_MENOS` | operador | CONFIRMADO |
| * | `TOKEN_VEZES` | operador | CONFIRMADO |
| / | `TOKEN_DIVISAO` | operador | CONFIRMADO |
| \ | `TOKEN_DIV_INT` | operador | CONFIRMADO |
| = | `TOKEN_IGUAL` | operador | CONFIRMADO |
| <> | `TOKEN_DIFERENTE` | operador | CONFIRMADO |
| <= | `TOKEN_MENOR_IGUAL` | operador | CONFIRMADO |
| >= | `TOKEN_MAIOR_IGUAL` | operador | CONFIRMADO |
| ( | `TOKEN_ABRE_PAR` | pontuação | CONFIRMADO |
| ) | `TOKEN_FECHA_PAR` | pontuação | CONFIRMADO |
| [ | `TOKEN_ABRE_COL` | pontuação | CONFIRMADO |
| ] | `TOKEN_FECHA_COL` | pontuação | CONFIRMADO |
| : | `TOKEN_DOIS_PONTOS` | pontuação | CONFIRMADO |
| , | `TOKEN_VIRGULA` | pontuação | CONFIRMADO |
| .. | `TOKEN_PONTO_PONTO` | pontuação | CONFIRMADO |

*Nota: Os operadores `<` e `>` foram marcados como AMBÍGUOS na Matriz de Evidências por falta de exemplos reais de uso e, no momento, não compõem um token de classe reconhecido de forma isolada sem evidências (a menos que seja como prefixo do Maximal Munch para `<=`, `<>`, etc).*

## Estratégias e Políticas Documentadas

1. **Maximal Munch**: Quando um caractere ambíguo inicia um token (ex: `<`), o scanner verifica agressivamente se é possível compor tokens maiores (como `<=`, `<>`). Se a correspondência maior falhar, ele retorna ao token simples (se esse token simples for suportado).
2. **Comentários**: Sequências iniciadas por `//` engolem todos os caracteres até o limite da linha atual (`\n`) ou `EOF` (Fim de Arquivo). Eles não produzem nenhum token.
3. **Formatos de Saída**: 
   - *Com atributo* (ex: `11# ID | 1`).
   - *Sem atributo* (ex: `12# FIMSE`). O formato impresso apenas omitirá o atributo.
4. **Tabela de Símbolos**: Dedicada a `IDENTIFICADORES`. As palavras reservadas ficarão em catálogo estático prévio na lógica e não poluirão a Tabela. A tabela armazenará a chave textual unívoca.

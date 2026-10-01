# Arquitetura do analisador léxico — Fase F

> **Escopo:** este documento descreve **apenas o que está implementado** em `compilador.c`: o analisador
> léxico. O analisador sintático **não existe ainda** (Fase G). O contrato que o código cumpre está em
> `especificacao-lexica.md`; os testes, em `testes.md`. **O código não define a linguagem**: onde houve
> dúvida, valeu o contrato.

## 1. Visão geral

Um único arquivo, `compilador.c` (REQ-08), com um módulo léxico de estado encapsulado. A interface pública é
`Token obterToken(void)` (nome do enunciado, REQ-28/REQ-32); o parser da Fase G terá um `nextToken()` que a
chamará sob demanda.

```
arquivo .alg --(fopen "rb")--> Scanner (lookahead) --> reconhecedores --> Token --> emitirToken --> tela + tokens.txt
                                      ^                      |                         |
                                      |                SymbolTable (só IDs)      (devolvido a quem chamou)
```

## 2. Organização de `compilador.c`

| # | Seção | Conteúdo |
|---|---|---|
| 1 | Includes e constantes | `ARQUIVO_SAIDA_TOKENS`, `EXIT_SOURCE_ERROR`, `EXIT_OPERATIONAL_ERROR`, `LOOKAHEAD_MAX` |
| 2 | Enums | `TokenName` (50), `OpRelType`, `OpMultType`, `AttributeKind` |
| 3 | Structs | `Token`, `LexemeEntry`, `LexemeBuffer`, `Scanner`, `SymbolTable` |
| 4 | Catálogos | `TOKEN_NAMES`, `OP_REL_NAMES`, `OP_MULT_NAMES`, `WORD_CATALOG`, `SYMBOL_CATALOG` |
| 5 | Estado | `scanner`, `symbol_table`, `token_output` |
| 6 | Erros operacionais e memória | `erroOperacional`, helpers ASCII, `duplicarTexto`, `buffer*` |
| 7 | Leitor | `peekChar`, `takeChar` |
| 8 | Tabela de símbolos | `tabelaSimbolosObter`, `tabelaSimbolosLiberar` |
| 9 | Token | `criarToken`, `criarTokenDeEntrada`, `liberarToken` |
| 10 | Saída e erro léxico | `formatarToken`, `emitirToken`, `formatarErroLexico`, `erroLexico` |
| 11 | Classes | `lerIdentificadorOuReservada`, `lerNumero`, `lerString` |
| 12 | Lexemas fixos | `buscarSimboloMaisLongo`, `lerSimbolo`, `erroCaractereInvalido` |
| 13 | `obterToken` | `pularEspacosEComentarios`, `obterToken` |
| 14 | Ciclo de vida | `iniciarAnalisadorLexico`, `fecharAnalisadorLexico` |
| 15 | Driver e `main` | `executarAnaliseLexica`, `main` |

Há cerca de 880 linhas (grande parte são os catálogos e o enum). Todas as funções cabem em uma tela. Não há `lexer.c`, `.h`, Makefile nem biblioteca externa
(DEC-02).

## 3. Tokens

### 3.1 `TokenName`

Os **50** nomes internos congelados na Fase B, na mesma ordem de `TOKEN_NAMES` (um `_Static_assert` confere o
tamanho). `TOKEN_COUNT` só dimensiona tabelas; não é um token. Não existem `TOKEN_LT`, `TOKEN_GT`,
`TOKEN_ASPAS`, `TOKEN_COMENTARIO`, `TOKEN_NAO` nem `TOKEN_ERRO`.

### 3.2 `Token`

```c
typedef struct {
    TokenName type;
    int line;
    AttributeKind attribute_kind;
    union {
        size_t table_index;       /* ATTR_SYMBOL_INDEX (1-based) */
        long long int_value;      /* ATTR_INT */
        double real_value;        /* ATTR_REAL */
        OpRelType op_rel;         /* ATTR_OP_REL */
        OpMultType op_mult;       /* ATTR_OP_MULT */
        const char *string_value; /* ATTR_STRING: alias de lexeme */
    } attribute;
    char *lexeme;
} Token;
```

Deriva da Figura 2 (tipo + linha + `union` de atributos) sem copiá-la (REQ-29): o atributo tem as variantes que o
Anexo I exige e há um `attribute_kind` explícito, que diz qual membro da `union` vale. O `lexeme` guarda o texto
original: é ele que sai na listagem para números e STRING (`1.60` continua `1.60`; a STRING sai com aspas).

### 3.3 Ownership

- **Cada token devolvido por `obterToken()` possui o seu `lexeme`**, alocado com `malloc`. A exceção é
  `TOKEN_EOF`, com `lexeme == NULL`.
- **Quem recebe o token chama `liberarToken()`** (faz `free` e zera a struct). O driver da Fase F faz isso a
  cada volta; na Fase G o parser liberará o lookahead anterior ao avançar.
- `string_value` só **aponta** para o `lexeme`: não tem dono próprio e não deve ser liberado.
- A tabela de símbolos guarda **cópias próprias** dos nomes (`duplicarTexto`); liberar o token não afeta a TS.

## 4. Leitor com lookahead (DEC-51)

`Scanner` = `FILE *`, linha corrente e um vetor de 4 `int` de lookahead.

- `peekChar(n)` devolve o byte `n` posições à frente **sem consumir e sem mexer na linha**; depois do fim devolve
  `EOF`.
- `takeChar()` consome um byte e **só `\n` incrementa a linha** (DEC-25): `\r` é whitespace e não conta, então
  CRLF vale uma linha e CR isolado não vale nenhuma.
- **Não se usa `ungetc`**: o padrão só garante um pushback e a regra do ponto (`1..4`) e os operadores de dois
  caracteres precisam olhar 2 bytes; a validação UTF-8 olha até 3.
- A fonte é aberta com `"rb"`: os bytes chegam exatamente como estão no disco e a conversão CRLF/LF fica sob
  controle do scanner, e não do C runtime.

## 5. Catálogos (DEC-05)

Ambos são **tabelas de dados** (`LexemeEntry`: lexema, tipo, tipo de atributo, subtipo), sem cadeia de `if/else`.
`_Static_assert` confere os tamanhos.

| Catálogo | Tamanho | Consulta |
|---|---|---|
| `WORD_CATALOG` | 34 = 31 reservadas + `MOD` + `E` + `OU` | depois de ler um identificador completo, por `strcmp` exato (case-sensitive; sem `strcasecmp`). `MOD` volta como `TOKEN_OP_MULT` + `OP_MOD`. |
| `SYMBOL_CATALOG` | 17 = 10 operadores + 7 pontuações | pelo primeiro byte que não é letra, dígito ou aspas, pelo **maior casamento** (`buscarSimboloMaisLongo`). Não contém `<`, `>` nem `.` isolados. |

## 6. Fluxo de `obterToken()`

```
pularEspacosEComentarios()            // espaço, TAB, CR, LF e "//" até antes de \r, \n ou EOF
c = peekChar(0); linha = scanner.line // a linha do token é a do seu PRIMEIRO byte
EOF            -> TOKEN_EOF (linha lógica do cursor; lexeme NULL; não é emitido)
[A-Za-z]       -> lerIdentificadorOuReservada   (ID ASCII; catálogo alfabético; senão TS)
[0-9]          -> lerNumero                      (NUM_INT | NUM_REAL, regra do ponto)
"              -> lerString                      (opaca, sem escapes)
qualquer outro -> lerSimbolo                     (catálogo simbólico, maior casamento; senão erro léxico)
emitirToken(&token)  // tela + tokens.txt, ANTES de devolver (DEC-56)
return token
```

- **Whitespace** é só `' ' \t \r \n`. Outros controles (`\x01`, `\f`…) **não** são whitespace: viram erro léxico.
- **Número:** lê todos os dígitos; só entra na parte decimal se vier `.` **seguido de dígito**. Assim `1..4` dá
  `NUM_INT` + `INTERVALO` + `NUM_INT`; `1.` dá `NUM_INT` e o `.` isolado é erro léxico na chamada seguinte;
  `1...4` dá `NUM_INT`, `INTERVALO` e erro em `.`; `1.5.3` dá `NUM_REAL(1.5)` e erro em `.`.
- **Maximal munch:** `buscarSimboloMaisLongo` compara cada entrada do catálogo com o lookahead e fica com a mais
  longa: `<=`, `<-`, `<>`, `>=`, `..` ganham de seus prefixos; `<` ou `>` sozinhos não casam com nada e viram
  erro. O comentário `//` é tratado **antes** da barra comum.
- **STRING:** guarda a aspa de abertura, copia bytes até a aspa de fechamento; `\r`, `\n` ou EOF antes dela é erro
  léxico, com a sequência indo da aspa de abertura até o ponto da detecção. Não há escapes.
- **ID:** `[A-Za-z][A-Za-z0-9_]*` com helpers ASCII explícitos (`ehLetraAscii` etc., sem `isalpha`/`isalnum`,
  que dependem de locale). `_x` falha no `_`; `preço` devolve `ID(pre)` e falha no `ç`.

## 7. Tabela de símbolos (DEC-50)

Vetor dinâmico de nomes (`char **`, capacidade dobrada, `realloc` com ponteiro temporário) e **busca linear**.
Só identificadores; 1ª ocorrência duplica o nome e devolve `posição + 1`; as seguintes reutilizam. Sem limite
arbitrário de identificadores. Liberada em `fecharAnalisadorLexico()`.

## 8. Lexemas dinâmicos (DEC-49)

`LexemeBuffer` (dados, tamanho, capacidade): cresce por capacidade dobrada, mantém sempre o `\0` final e entrega
a memória a quem chama (`bufferEntregar`). **Não há limite fixo** de tamanho de ID, número ou STRING (verificado
com um ID de 200 000 caracteres e uma STRING de 300 000). Falha de `malloc`/`realloc` **não é erro léxico**: é
falha operacional (`erroOperacional`).

## 9. Encoding (DEC-48, resolve AMB-08)

O scanner trabalha **por bytes**; a estrutura da linguagem é ASCII.

| Onde | Bytes ≥ 0x80 |
|---|---|
| dentro de STRING | conteúdo opaco, copiado **literalmente** (`"João"` em UTF-8 ou Latin-1) |
| dentro de comentário `//` | descartados com o resto do comentário |
| em qualquer outro lugar | **erro léxico** |

Para o erro dar uma sequência útil: se o byte inicia um UTF-8 **bem formado** de 2, 3 ou 4 bytes
(byte inicial `C2–DF`, `E0–EF` ou `F0–F4`, seguido do número certo de continuações `80–BF`), reporta-se a sequência
inteira (`ç` = `C3 A7`); caso contrário, **só o byte**. A verificação é **estrutural**: não rejeita formas
supérfluas nem substitutos. **Isto não é suporte geral a Unicode**: nenhum byte ≥ 0x80 forma identificador.

## 10. Saída

- **Uma função de formatação** (`formatarToken`) serve a tela e a `tokens.txt`, então os dois nunca divergem.
- Formato: `linha# NOME` e, se houver atributo, ` | atributo`; `ID` imprime o índice; `NUM_INT`, `NUM_REAL` e
  `STRING` imprimem o **lexema original**; `OP_REL` e `OP_MULT` imprimem o subtipo. Sem `| NULL`.
- `TOKEN_EOF` não é impresso. Nenhuma mensagem extra ("sucesso", "token encontrado") vai ao stdout.
- `tokens.txt`: diretório de trabalho atual, aberto com `"wb"`, **sobrescrito** a cada execução (DEC-52).
- A saída é emitida **dentro de `obterToken()`** (DEC-56): na Fase G o parser vai pedir tokens sob demanda e a
  listagem continuará saindo sem mudança.

Nota de console (Windows): `stdout` é aberto em modo texto, então o console traduz `\n` e pode exibir `ç` de forma
diferente conforme a *code page*. **`tokens.txt` preserva os bytes exatos** e é a referência confiável. Nenhuma API
do Windows foi usada para "embelezar" o terminal.

## 11. Erros

| Tipo | Quando | Saída | Status |
|---|---|---|---|
| **ERRO LÉXICO** | erro no código-fonte | `ERRO LÉXICO - linha <n> - sequência: <sequência>` em **stdout e `tokens.txt`** (DEC-26, DEC-53) | `EXIT_SOURCE_ERROR` (**provisoriamente 0**, DEC-55) |
| erro **operacional** | argumentos, arquivo, memória, leitura, representação numérica | texto em **stderr**; `ERRO INTERNO - …` quando já está analisando | `EXIT_OPERATIONAL_ERROR` (≠ 0) |

- Erro léxico **encerra imediatamente** (sem recuperação): grava a mensagem, fecha os arquivos, libera a TS e
  sai. Os tokens anteriores já emitidos permanecem na saída.
- **Overflow numérico:** se o lexema satisfaz a ER mas `strtoll`/`strtod` acusam `ERANGE`, **não** é erro léxico
  (o lexema é válido): é falha de **representação do implementador** (`long long` / `double`), mensagem
  `ERRO INTERNO - valor numerico fora da faixa representavel`, status ≠ 0. É limitação do C, **não regra do
  MiniVisualg**; `9223372036854775807` cabe, `99999999999999999999` não. `strtod` também sinaliza `ERANGE` em
  *underflow*, e o tratamento é o mesmo.
- **Status de erro léxico (AMB-13 segue aberta):** o critério de avaliação penaliza retorno ≠ 0, então, até a
  professora esclarecer, o erro **identificado** sai com 0. Está centralizado em
  `#define EXIT_SOURCE_ERROR 0`: trocar a política é mudar essa linha.

## 12. Ciclo de vida

```
main
  argc != 2                  -> "uso: ..." em stderr, status != 0
  iniciarAnalisadorLexico(p) -> abre fonte ("rb") e tokens.txt ("wb"); linha = 1; TS vazia
  executarAnaliseLexica()    -> obterToken() até TOKEN_EOF, liberarToken() a cada volta   [driver temporário]
  fecharAnalisadorLexico()   -> fecha os dois arquivos, libera a TS (pode ser chamada de novo sem efeito)
  return EXIT_SUCCESS
```

Fonte inexistente: mensagem em stderr, **sem** criar `tokens.txt`, status ≠ 0 (não é erro léxico).
`executarAnaliseLexica()` **não é o parser**: é só o laço que existe enquanto o parser não existe e será
substituído na Fase G.

## 13. Linha do `TOKEN_EOF` (DEC-54, resolve parte de AMB-14)

`TOKEN_EOF.line` é a **linha lógica do cursor** ao chegar ao fim, depois de descartar whitespace e comentários.
Verificado com um harness descartável: `a` sem `\n` final → EOF na linha 1; `a\n` → linha 2; `a\n\n\n` → linha 4;
`a // c` → 1; `a // c\n` → 2; CRLF conta uma vez; arquivo vazio → 1. Servirá ao erro "fim de arquivo inesperado".

## 14. Ambiente e compilação

- **Comando:** `gcc -Wall -Wno-unused-result -g -Og compilador.c -o compilador`
- **Compilador usado (real):** `gcc.exe (Rev8, Built by MSYS2 project) 15.2.0`, em `C:\msys64\mingw64\bin`
  (adicionado ao `PATH` apenas na sessão; nada foi instalado).
- **Resultado:** 0 erros, 0 warnings (o `gcc` não imprimiu nenhuma mensagem). Também limpo, só como inspeção,
  com `-Wextra -Wpedantic -Wshadow`.
- **Dialeto:** compila limpo em `-std=gnu99`, `gnu11`, `gnu17` e `c11`. **Exige C99 ou posterior**
  (declaração no `for`, `<stdbool.h>`) e usa `_Static_assert` (C11, aceito pelo `gcc` como extensão em C99).
  O modo `-std=gnu89` (padrão de `gcc` anterior à versão 5) **não** compila. Só foi testado com o GCC 15.2.0.
- Não há `locale`: `strtod` usa a localidade `"C"` (ponto decimal), que é o padrão do programa.

## 15. Revisão de memória (manual)

Sem AddressSanitizer neste MinGW (`-lasan` ausente) e sem Valgrind, a revisão foi **manual**, ponto a ponto:

- todo `lexeme` é liberado: `liberarToken()` no driver; nos erros, `erroLexico` libera o `LexemeBuffer` e
  `lerNumero` libera o token antes do erro operacional;
- os nomes da TS são liberados em `tabelaSimbolosLiberar()`;
- `realloc` usa sempre um ponteiro temporário; nenhum ponteiro é usado depois do `realloc` sem reatribuição;
- nenhum ponteiro para buffer local é devolvido; `string_value` não duplica ownership;
- nenhum `strcpy`/`sprintf` em buffer fixo: lexemas são `LexemeBuffer` dinâmicos; mensagens usam `fprintf`;
- os dois arquivos são fechados em `fecharAnalisadorLexico()` (inclusive nos caminhos de erro).

Limite desta revisão: não houve execução sob ferramenta de memória. O comportamento foi exercitado com 20 005
tokens, lexemas de 200 000 e 300 000 bytes e 20 000 identificadores distintos, sem falhas.

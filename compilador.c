/*
 * Projeto 1 - Compiladores
 * MiniVisualg - Analisador Lexico e Sintatico
 *
 * Integrantes:
 * Gabriel Nottoli Buck
 * Julia Andrade
 * Joao vitor rocha miranda
 *
 * Profa. Daniela Cunha
 *
 * Estado atual (Fase G): analisadores LEXICO e SINTATICO implementados.
 * O contrato lexico esta em docs/especificacao-lexica.md; a GLC e os conjuntos
 * SELECT estao em docs/gramatica.md e docs/analise-ll1.md.
 *
 * Compilar:  gcc -Wall -Wno-unused-result -g -Og compilador.c -o compilador
 * Executar:  compilador arquivo.alg      (gera tokens.txt e arvore.txt)
 */

/* ======================================================================== */
/* 1. Includes e constantes                                                  */
/* ======================================================================== */

#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQUIVO_SAIDA_TOKENS "tokens.txt"
#define ARQUIVO_SAIDA_ARVORE "arvore.txt"

/*
 * Codigos de saida. AMB-13 (codigo de retorno em erro de fonte) segue ABERTA:
 * o criterio de avaliacao penaliza retorno diferente de 0, entao, ate haver
 * esclarecimento da professora, um erro lexico ou sintatico IDENTIFICADO e
 * encerrado com
 * 0. Para trocar a politica basta mudar esta linha. Falhas operacionais
 * (argumentos, arquivo, memoria, representacao numerica) usam sempre != 0.
 */
#define EXIT_SOURCE_ERROR 0
#define EXIT_OPERATIONAL_ERROR EXIT_FAILURE

#define LOOKAHEAD_MAX 4        /* o maior olhar adiante usado e de 3 bytes (UTF-8) */
#define CAPACIDADE_INICIAL 32  /* capacidade inicial dos buffers dinamicos */

#define ARRAY_COUNT(a) (sizeof(a) / sizeof((a)[0]))

/* ======================================================================== */
/* 2. Enums                                                                  */
/* ======================================================================== */

/* Os 50 nomes de token internos congelados na Fase B (especificacao-lexica.md, secao 3). */
typedef enum {
    TOKEN_EOF,
    /* tokens de classe */
    TOKEN_ID,
    TOKEN_NUM_INT,
    TOKEN_NUM_REAL,
    TOKEN_STRING,
    /* palavras reservadas (31) */
    TOKEN_KW_ALGORITMO,
    TOKEN_KW_VAR,
    TOKEN_KW_INICIO,
    TOKEN_KW_FIMALGORITMO,
    TOKEN_KW_INTEIRO,
    TOKEN_KW_REAL,
    TOKEN_KW_CARACTERE,
    TOKEN_KW_LOGICO,
    TOKEN_KW_VERDADEIRO,
    TOKEN_KW_FALSO,
    TOKEN_KW_LEIA,
    TOKEN_KW_ESCREVA,
    TOKEN_KW_ESCREVAL,
    TOKEN_KW_SE,
    TOKEN_KW_ENTAO,
    TOKEN_KW_SENAO,
    TOKEN_KW_FIMSE,
    TOKEN_KW_PARA,
    TOKEN_KW_DE,
    TOKEN_KW_ATE,
    TOKEN_KW_PASSO,
    TOKEN_KW_FACA,
    TOKEN_KW_FIMPARA,
    TOKEN_KW_ENQUANTO,
    TOKEN_KW_FIMENQUANTO,
    TOKEN_KW_VETOR,
    TOKEN_KW_PROCEDIMENTO,
    TOKEN_KW_FIMPROCEDIMENTO,
    TOKEN_KW_FUNCAO,
    TOKEN_KW_FIMFUNCAO,
    TOKEN_KW_RETORNE,
    /* operadores-palavra (MOD e OP_MULT com atributo) */
    TOKEN_E,
    TOKEN_OU,
    /* operadores simbolicos */
    TOKEN_OP_REL,
    TOKEN_OP_MULT,
    TOKEN_MAIS,
    TOKEN_MENOS,
    TOKEN_ATRIBUICAO,
    /* pontuacao */
    TOKEN_ABRE_PAR,
    TOKEN_FECHA_PAR,
    TOKEN_ABRE_COL,
    TOKEN_FECHA_COL,
    TOKEN_DOIS_PONTOS,
    TOKEN_VIRGULA,
    TOKEN_INTERVALO,
    TOKEN_COUNT /* so para dimensionar tabelas; nao e um token */
} TokenName;

/* Atributo de TOKEN_OP_REL. Nao existe LT/GT: '<' e '>' isolados nao sao tokens (DEC-16). */
typedef enum { OP_EQ, OP_NE, OP_LE, OP_GE } OpRelType;

/* Atributo de TOKEN_OP_MULT. */
typedef enum { OP_MUL, OP_DIV_REAL, OP_DIV_INT, OP_MOD } OpMultType;

/* Diz qual membro da union do Token e valido. */
typedef enum {
    ATTR_NONE,
    ATTR_SYMBOL_INDEX,
    ATTR_INT,
    ATTR_REAL,
    ATTR_STRING,
    ATTR_OP_REL,
    ATTR_OP_MULT
} AttributeKind;

/* ======================================================================== */
/* 3. Structs                                                                */
/* ======================================================================== */

/*
 * Token: tipo + linha + atributo variavel (ideia da Figura 2 do enunciado),
 * adaptado ao MiniVisualg (REQ-29, DEC-06).
 *
 * lexeme: texto exato lido da entrada, alocado para cada token (NULL somente
 * em TOKEN_EOF). A listagem usa o lexeme para numeros e STRING, para que
 * 1.60 continue 1.60. Quem recebe o Token deve chamar liberarToken().
 *
 * string_value apenas APONTA para o lexeme (sem ownership proprio).
 */
typedef struct {
    TokenName type;
    int line;
    AttributeKind attribute_kind;
    union {
        size_t table_index;       /* ATTR_SYMBOL_INDEX: indice 1-based na tabela de simbolos */
        long long int_value;      /* ATTR_INT */
        double real_value;        /* ATTR_REAL */
        OpRelType op_rel;         /* ATTR_OP_REL */
        OpMultType op_mult;       /* ATTR_OP_MULT */
        const char *string_value; /* ATTR_STRING: alias de lexeme */
    } attribute;
    char *lexeme;
} Token;

/* Entrada dos catalogos de lexemas fixos (palavras e simbolos). */
typedef struct {
    const char *lexeme;
    TokenName type;
    AttributeKind attribute_kind;
    int subtype; /* OpRelType ou OpMultType, quando houver */
} LexemeEntry;

/* Buffer dinamico de bytes para montar lexemas sem limite fixo (DEC-49). */
typedef struct {
    char *data;
    size_t length;
    size_t capacity;
} LexemeBuffer;

/* Leitor de caracteres com lookahead proprio (DEC-51). */
typedef struct {
    FILE *source;
    int line;
    int lookahead[LOOKAHEAD_MAX];
    size_t lookahead_count;
} Scanner;

/* Tabela de simbolos: so identificadores, busca linear (DEC-50). */
typedef struct {
    char **names;
    size_t count;
    size_t capacity;
} SymbolTable;

/* ======================================================================== */
/* 4. Catalogos estaticos                                                    */
/* ======================================================================== */

/* Nomes impressos, na ordem exata de TokenName. */
static const char *const TOKEN_NAMES[] = {
    "EOF",
    "ID", "NUM_INT", "NUM_REAL", "STRING",
    "ALGORITMO", "VAR", "INICIO", "FIMALGORITMO",
    "INTEIRO", "REAL", "CARACTERE", "LOGICO",
    "VERDADEIRO", "FALSO",
    "LEIA", "ESCREVA", "ESCREVAL",
    "SE", "ENTAO", "SENAO", "FIMSE",
    "PARA", "DE", "ATE", "PASSO", "FACA", "FIMPARA",
    "ENQUANTO", "FIMENQUANTO",
    "VETOR",
    "PROCEDIMENTO", "FIMPROCEDIMENTO", "FUNCAO", "FIMFUNCAO", "RETORNE",
    "E", "OU",
    "OP_REL", "OP_MULT", "MAIS", "MENOS", "ATRIBUICAO",
    "ABRE_PAR", "FECHA_PAR", "ABRE_COL", "FECHA_COL", "DOIS_PONTOS", "VIRGULA", "INTERVALO"
};
_Static_assert(ARRAY_COUNT(TOKEN_NAMES) == TOKEN_COUNT, "TOKEN_NAMES fora de sincronia com TokenName");

static const char *const OP_REL_NAMES[] = { "EQ", "NE", "LE", "GE" };
_Static_assert(ARRAY_COUNT(OP_REL_NAMES) == OP_GE + 1, "OP_REL_NAMES fora de sincronia");

static const char *const OP_MULT_NAMES[] = { "MUL", "DIV_REAL", "DIV_INT", "MOD" };
_Static_assert(ARRAY_COUNT(OP_MULT_NAMES) == OP_MOD + 1, "OP_MULT_NAMES fora de sincronia");

/*
 * Catalogo alfabetico (34 lexemas): 31 reservadas + MOD, E, OU.
 * Consultado por comparacao EXATA depois de ler um identificador completo
 * (case-sensitive, DEC-13). MOD nao tem token proprio: e OP_MULT com atributo.
 */
static const LexemeEntry WORD_CATALOG[] = {
    { "algoritmo",       TOKEN_KW_ALGORITMO,       ATTR_NONE, 0 },
    { "var",             TOKEN_KW_VAR,             ATTR_NONE, 0 },
    { "inicio",          TOKEN_KW_INICIO,          ATTR_NONE, 0 },
    { "fimalgoritmo",    TOKEN_KW_FIMALGORITMO,    ATTR_NONE, 0 },
    { "inteiro",         TOKEN_KW_INTEIRO,         ATTR_NONE, 0 },
    { "real",            TOKEN_KW_REAL,            ATTR_NONE, 0 },
    { "caractere",       TOKEN_KW_CARACTERE,       ATTR_NONE, 0 },
    { "logico",          TOKEN_KW_LOGICO,          ATTR_NONE, 0 },
    { "verdadeiro",      TOKEN_KW_VERDADEIRO,      ATTR_NONE, 0 },
    { "falso",           TOKEN_KW_FALSO,           ATTR_NONE, 0 },
    { "leia",            TOKEN_KW_LEIA,            ATTR_NONE, 0 },
    { "escreva",         TOKEN_KW_ESCREVA,         ATTR_NONE, 0 },
    { "escreval",        TOKEN_KW_ESCREVAL,        ATTR_NONE, 0 },
    { "se",              TOKEN_KW_SE,              ATTR_NONE, 0 },
    { "entao",           TOKEN_KW_ENTAO,           ATTR_NONE, 0 },
    { "senao",           TOKEN_KW_SENAO,           ATTR_NONE, 0 },
    { "fimse",           TOKEN_KW_FIMSE,           ATTR_NONE, 0 },
    { "para",            TOKEN_KW_PARA,            ATTR_NONE, 0 },
    { "de",              TOKEN_KW_DE,              ATTR_NONE, 0 },
    { "ate",             TOKEN_KW_ATE,             ATTR_NONE, 0 },
    { "passo",           TOKEN_KW_PASSO,           ATTR_NONE, 0 },
    { "faca",            TOKEN_KW_FACA,            ATTR_NONE, 0 },
    { "fimpara",         TOKEN_KW_FIMPARA,         ATTR_NONE, 0 },
    { "enquanto",        TOKEN_KW_ENQUANTO,        ATTR_NONE, 0 },
    { "fimenquanto",     TOKEN_KW_FIMENQUANTO,     ATTR_NONE, 0 },
    { "vetor",           TOKEN_KW_VETOR,           ATTR_NONE, 0 },
    { "procedimento",    TOKEN_KW_PROCEDIMENTO,    ATTR_NONE, 0 },
    { "fimprocedimento", TOKEN_KW_FIMPROCEDIMENTO, ATTR_NONE, 0 },
    { "funcao",          TOKEN_KW_FUNCAO,          ATTR_NONE, 0 },
    { "fimfuncao",       TOKEN_KW_FIMFUNCAO,       ATTR_NONE, 0 },
    { "retorne",         TOKEN_KW_RETORNE,         ATTR_NONE, 0 },
    { "MOD",             TOKEN_OP_MULT,            ATTR_OP_MULT, OP_MOD },
    { "E",               TOKEN_E,                  ATTR_NONE, 0 },
    { "OU",              TOKEN_OU,                 ATTR_NONE, 0 }
};
_Static_assert(ARRAY_COUNT(WORD_CATALOG) == 34, "o catalogo alfabetico deve ter 34 lexemas");

/*
 * Catalogo simbolico (17 lexemas): operadores e pontuacao.
 * Escolhe-se o MAIOR casamento (maximal munch). Nao contem '<', '>' nem '.'
 * isolados: so existem como prefixo de "<-", "<>", "<=", ">=", "..".
 */
static const LexemeEntry SYMBOL_CATALOG[] = {
    { "<-", TOKEN_ATRIBUICAO, ATTR_NONE,    0 },
    { "<>", TOKEN_OP_REL,     ATTR_OP_REL,  OP_NE },
    { "<=", TOKEN_OP_REL,     ATTR_OP_REL,  OP_LE },
    { ">=", TOKEN_OP_REL,     ATTR_OP_REL,  OP_GE },
    { "..", TOKEN_INTERVALO,  ATTR_NONE,    0 },
    { "=",  TOKEN_OP_REL,     ATTR_OP_REL,  OP_EQ },
    { "+",  TOKEN_MAIS,       ATTR_NONE,    0 },
    { "-",  TOKEN_MENOS,      ATTR_NONE,    0 },
    { "*",  TOKEN_OP_MULT,    ATTR_OP_MULT, OP_MUL },
    { "/",  TOKEN_OP_MULT,    ATTR_OP_MULT, OP_DIV_REAL },
    { "\\", TOKEN_OP_MULT,    ATTR_OP_MULT, OP_DIV_INT },
    { "(",  TOKEN_ABRE_PAR,   ATTR_NONE,    0 },
    { ")",  TOKEN_FECHA_PAR,  ATTR_NONE,    0 },
    { "[",  TOKEN_ABRE_COL,   ATTR_NONE,    0 },
    { "]",  TOKEN_FECHA_COL,  ATTR_NONE,    0 },
    { ":",  TOKEN_DOIS_PONTOS, ATTR_NONE,   0 },
    { ",",  TOKEN_VIRGULA,    ATTR_NONE,    0 }
};
_Static_assert(ARRAY_COUNT(SYMBOL_CATALOG) == 17, "o catalogo simbolico deve ter 17 lexemas");

/* ======================================================================== */
/* 5. Estado do analisador lexico                                            */
/* ======================================================================== */

/* Um unico arquivo-fonte e um unico modulo lexico: estado agrupado, sem globais avulsas. */
static Scanner scanner;
static SymbolTable symbol_table;
static FILE *token_output;

/* Estado do parser: um unico lookahead e escrita direta da arvore em pre-ordem. */
static Token lookahead;
static bool lookahead_valido;
static FILE *arvore_output;
static int profundidade_arvore;

/* ======================================================================== */
/* 6. Erros operacionais e utilidades de memoria/string                      */
/* ======================================================================== */

static void fecharAnalisadorLexico(void);
static void fecharAnalisadorSintatico(void);

/*
 * Falha operacional (memoria, leitura, representacao): NAO e erro lexico.
 * Vai para stderr e termina com status != 0.
 */
static void erroOperacional(const char *mensagem)
{
    fprintf(stderr, "ERRO INTERNO - %s\n", mensagem);
    fecharAnalisadorSintatico();
    fecharAnalisadorLexico();
    exit(EXIT_OPERATIONAL_ERROR);
}

static bool ehLetraAscii(int c)
{
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

static bool ehDigitoAscii(int c)
{
    return c >= '0' && c <= '9';
}

static bool ehAlfanumericoAscii(int c)
{
    return ehLetraAscii(c) || ehDigitoAscii(c);
}

static bool ehEspaco(int c)
{
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

/* Copia n bytes para memoria nova terminada em '\0'. */
static char *duplicarTexto(const char *texto, size_t n)
{
    char *copia = malloc(n + 1);
    if (copia == NULL) {
        erroOperacional("memoria insuficiente");
    }
    memcpy(copia, texto, n);
    copia[n] = '\0';
    return copia;
}

static void bufferIniciar(LexemeBuffer *buffer)
{
    buffer->data = malloc(CAPACIDADE_INICIAL);
    if (buffer->data == NULL) {
        erroOperacional("memoria insuficiente");
    }
    buffer->data[0] = '\0';
    buffer->length = 0;
    buffer->capacity = CAPACIDADE_INICIAL;
}

/* Acrescenta um byte; dobra a capacidade quando necessario. Sempre mantem o '\0' final. */
static void bufferAdicionar(LexemeBuffer *buffer, char byte)
{
    if (buffer->length + 1 >= buffer->capacity) {
        size_t nova_capacidade = buffer->capacity * 2;
        char *novo = realloc(buffer->data, nova_capacidade);
        if (novo == NULL) {
            erroOperacional("memoria insuficiente");
        }
        buffer->data = novo;
        buffer->capacity = nova_capacidade;
    }
    buffer->data[buffer->length++] = byte;
    buffer->data[buffer->length] = '\0';
}

/* Entrega o texto montado a quem chamou (que passa a ser dono) e zera o buffer. */
static char *bufferEntregar(LexemeBuffer *buffer)
{
    char *texto = buffer->data;
    buffer->data = NULL;
    buffer->length = 0;
    buffer->capacity = 0;
    return texto;
}

static void bufferLiberar(LexemeBuffer *buffer)
{
    free(buffer->data);
    buffer->data = NULL;
    buffer->length = 0;
    buffer->capacity = 0;
}

/* ======================================================================== */
/* 7. Leitura de caracteres com lookahead                                    */
/* ======================================================================== */

/*
 * Devolve o byte na posicao 'deslocamento' a frente, sem consumir e sem mexer
 * na linha. Devolve EOF (-1) depois do fim. Nao usamos ungetc: o padrao so
 * garante um pushback e aqui precisamos de ate 3 bytes de olhar adiante.
 */
static int peekChar(size_t deslocamento)
{
    while (scanner.lookahead_count <= deslocamento) {
        int c = fgetc(scanner.source);
        if (c == EOF && ferror(scanner.source)) {
            erroOperacional("falha ao ler o arquivo fonte");
        }
        scanner.lookahead[scanner.lookahead_count++] = c;
    }
    return scanner.lookahead[deslocamento];
}

/* Consome um byte. Somente '\n' incrementa a linha ('\r' e whitespace; CRLF conta uma vez). */
static int takeChar(void)
{
    int c = peekChar(0);
    if (c == EOF) {
        return EOF;
    }
    for (size_t i = 1; i < scanner.lookahead_count; i++) {
        scanner.lookahead[i - 1] = scanner.lookahead[i];
    }
    scanner.lookahead_count--;
    if (c == '\n') {
        scanner.line++;
    }
    return c;
}

/* ======================================================================== */
/* 8. Tabela de simbolos                                                     */
/* ======================================================================== */

/*
 * Devolve o indice (1-based) do identificador, inserindo-o na primeira
 * ocorrencia. Palavras reservadas nunca chegam aqui. Case-sensitive.
 */
static size_t tabelaSimbolosObter(const char *nome)
{
    for (size_t i = 0; i < symbol_table.count; i++) {
        if (strcmp(symbol_table.names[i], nome) == 0) {
            return i + 1;
        }
    }
    if (symbol_table.count == symbol_table.capacity) {
        size_t nova_capacidade = symbol_table.capacity == 0 ? 16 : symbol_table.capacity * 2;
        char **novo = realloc(symbol_table.names, nova_capacidade * sizeof(char *));
        if (novo == NULL) {
            erroOperacional("memoria insuficiente");
        }
        symbol_table.names = novo;
        symbol_table.capacity = nova_capacidade;
    }
    symbol_table.names[symbol_table.count] = duplicarTexto(nome, strlen(nome));
    symbol_table.count++;
    return symbol_table.count;
}

static void tabelaSimbolosLiberar(void)
{
    for (size_t i = 0; i < symbol_table.count; i++) {
        free(symbol_table.names[i]);
    }
    free(symbol_table.names);
    symbol_table.names = NULL;
    symbol_table.count = 0;
    symbol_table.capacity = 0;
}

/* ======================================================================== */
/* 9. Criacao e liberacao de Token                                           */
/* ======================================================================== */

/* Cria um token sem atributo. O token passa a ser dono de 'lexeme' (pode ser NULL). */
static Token criarToken(TokenName type, int line, char *lexeme)
{
    Token token;
    memset(&token, 0, sizeof token);
    token.type = type;
    token.line = line;
    token.attribute_kind = ATTR_NONE;
    token.lexeme = lexeme;
    return token;
}

/* Cria o token de uma entrada dos catalogos, copiando tipo e atributo. */
static Token criarTokenDeEntrada(const LexemeEntry *entrada, int line, char *lexeme)
{
    Token token = criarToken(entrada->type, line, lexeme);
    token.attribute_kind = entrada->attribute_kind;
    if (entrada->attribute_kind == ATTR_OP_REL) {
        token.attribute.op_rel = (OpRelType)entrada->subtype;
    } else if (entrada->attribute_kind == ATTR_OP_MULT) {
        token.attribute.op_mult = (OpMultType)entrada->subtype;
    }
    return token;
}

/* Libera o lexeme do token (todo token, exceto TOKEN_EOF, possui o seu). */
void liberarToken(Token *token)
{
    free(token->lexeme);
    memset(token, 0, sizeof *token);
}

/* ======================================================================== */
/* 10. Saida de tokens e erro lexico                                         */
/* ======================================================================== */

/*
 * Formato congelado (LEX-17): "<linha># <NOME>" e, se houver atributo,
 * " | <atributo>". Numeros e STRING saem pelo lexema original (1.60 continua
 * 1.60; a STRING sai com as aspas). Uma unica funcao serve a tela e ao arquivo,
 * para que os dois nunca divirjam.
 */
static void formatarToken(const Token *token, FILE *destino)
{
    fprintf(destino, "%d# %s", token->line, TOKEN_NAMES[token->type]);
    switch (token->attribute_kind) {
    case ATTR_SYMBOL_INDEX:
        fprintf(destino, " | %zu", token->attribute.table_index);
        break;
    case ATTR_INT:
    case ATTR_REAL:
    case ATTR_STRING:
        fprintf(destino, " | %s", token->lexeme);
        break;
    case ATTR_OP_REL:
        fprintf(destino, " | %s", OP_REL_NAMES[token->attribute.op_rel]);
        break;
    case ATTR_OP_MULT:
        fprintf(destino, " | %s", OP_MULT_NAMES[token->attribute.op_mult]);
        break;
    case ATTR_NONE:
        break;
    }
    fputc('\n', destino);
}

/* Emite o token na tela e em tokens.txt (TOKEN_EOF nunca e emitido: DEC-23). */
static void emitirToken(const Token *token)
{
    formatarToken(token, stdout);
    formatarToken(token, token_output);
}

/* "ERRO LÉXICO - linha <n> - sequência: <sequência>" (DEC-26); a sequencia vai em bytes, sem '%s'. */
static void formatarErroLexico(FILE *destino, int line, const LexemeBuffer *sequencia)
{
    fprintf(destino, "ERRO LÉXICO - linha %d - sequência: ", line);
    fwrite(sequencia->data, 1, sequencia->length, destino);
    fputc('\n', destino);
}

/*
 * Erro lexico: espelha a mensagem na tela e em tokens.txt, encerra o
 * processamento (sem recuperacao) e sai com EXIT_SOURCE_ERROR (AMB-13 aberta).
 */
static void erroLexico(int line, LexemeBuffer *sequencia)
{
    formatarErroLexico(stdout, line, sequencia);
    formatarErroLexico(token_output, line, sequencia);
    bufferLiberar(sequencia);
    fecharAnalisadorSintatico();
    fecharAnalisadorLexico();
    exit(EXIT_SOURCE_ERROR);
}

/* ======================================================================== */
/* 11. Reconhecimento de classes (ID, numeros, STRING)                       */
/* ======================================================================== */

static const LexemeEntry *buscarPalavra(const char *lexeme)
{
    for (size_t i = 0; i < ARRAY_COUNT(WORD_CATALOG); i++) {
        if (strcmp(WORD_CATALOG[i].lexeme, lexeme) == 0) {
            return &WORD_CATALOG[i];
        }
    }
    return NULL;
}

/* ID = [A-Za-z][A-Za-z0-9_]*; depois consulta o catalogo alfabetico; senao e ID da tabela de simbolos. */
static Token lerIdentificadorOuReservada(int line)
{
    LexemeBuffer buffer;
    bufferIniciar(&buffer);
    while (ehAlfanumericoAscii(peekChar(0)) || peekChar(0) == '_') {
        bufferAdicionar(&buffer, (char)takeChar());
    }
    char *lexeme = bufferEntregar(&buffer);

    const LexemeEntry *palavra = buscarPalavra(lexeme);
    if (palavra != NULL) {
        return criarTokenDeEntrada(palavra, line, lexeme);
    }
    Token token = criarToken(TOKEN_ID, line, lexeme);
    token.attribute_kind = ATTR_SYMBOL_INDEX;
    token.attribute.table_index = tabelaSimbolosObter(lexeme);
    return token;
}

static void lerDigitos(LexemeBuffer *buffer)
{
    while (ehDigitoAscii(peekChar(0))) {
        bufferAdicionar(buffer, (char)takeChar());
    }
}

/*
 * NUM_INT = [0-9]+ ; NUM_REAL = [0-9]+\.[0-9]+ (LEX-04, LEX-05, LEX-06).
 * Depois dos digitos, so entra na parte decimal se vier '.' seguido de digito;
 * "1..4" devolve NUM_INT e deixa ".." para o proximo token; "1." devolve
 * NUM_INT e deixa o '.' isolado, que sera erro lexico na chamada seguinte.
 */
static Token lerNumero(int line)
{
    LexemeBuffer buffer;
    bufferIniciar(&buffer);
    lerDigitos(&buffer);

    bool real = false;
    if (peekChar(0) == '.' && ehDigitoAscii(peekChar(1))) {
        real = true;
        bufferAdicionar(&buffer, (char)takeChar());
        lerDigitos(&buffer);
    }

    Token token = criarToken(real ? TOKEN_NUM_REAL : TOKEN_NUM_INT, line, bufferEntregar(&buffer));
    errno = 0;
    if (real) {
        token.attribute_kind = ATTR_REAL;
        token.attribute.real_value = strtod(token.lexeme, NULL);
    } else {
        token.attribute_kind = ATTR_INT;
        token.attribute.int_value = strtoll(token.lexeme, NULL, 10);
    }
    if (errno == ERANGE) {
        /* O lexema e valido para o MiniVisualg; o limite e do tipo C usado (long long / double). */
        liberarToken(&token);
        erroOperacional("valor numerico fora da faixa representavel");
    }
    return token;
}

/*
 * STRING = "[^"\r\n]*" (LEX-07): o conteudo e opaco (bytes >= 0x80 sao copiados),
 * sem escapes. Aberta e nao fechada ate \r, \n ou EOF e erro lexico, com a
 * sequencia indo da aspa de abertura ate o ponto da deteccao.
 */
static Token lerString(int line)
{
    LexemeBuffer buffer;
    bufferIniciar(&buffer);
    bufferAdicionar(&buffer, (char)takeChar()); /* aspa de abertura */

    for (;;) {
        int c = peekChar(0);
        if (c == EOF || c == '\r' || c == '\n') {
            erroLexico(line, &buffer);
        }
        bufferAdicionar(&buffer, (char)takeChar());
        if (c == '"') {
            break;
        }
    }
    Token token = criarToken(TOKEN_STRING, line, bufferEntregar(&buffer));
    token.attribute_kind = ATTR_STRING;
    token.attribute.string_value = token.lexeme;
    return token;
}

/* ======================================================================== */
/* 12. Reconhecimento de lexemas fixos e caracteres invalidos                */
/* ======================================================================== */

static bool casaComLookahead(const char *lexeme, size_t tamanho)
{
    for (size_t i = 0; i < tamanho; i++) {
        if (peekChar(i) != (unsigned char)lexeme[i]) {
            return false;
        }
    }
    return true;
}

/* Maximal munch no catalogo simbolico: devolve a entrada mais longa que casa com a entrada, ou NULL. */
static const LexemeEntry *buscarSimboloMaisLongo(void)
{
    const LexemeEntry *melhor = NULL;
    size_t melhor_tamanho = 0;
    for (size_t i = 0; i < ARRAY_COUNT(SYMBOL_CATALOG); i++) {
        size_t tamanho = strlen(SYMBOL_CATALOG[i].lexeme);
        if (tamanho > melhor_tamanho && casaComLookahead(SYMBOL_CATALOG[i].lexeme, tamanho)) {
            melhor = &SYMBOL_CATALOG[i];
            melhor_tamanho = tamanho;
        }
    }
    return melhor;
}

/* Tamanho de uma sequencia UTF-8 pelo byte inicial (1 se nao for um inicio valido). */
static size_t tamanhoUtf8(int primeiro)
{
    if (primeiro >= 0xC2 && primeiro <= 0xDF) {
        return 2;
    }
    if (primeiro >= 0xE0 && primeiro <= 0xEF) {
        return 3;
    }
    if (primeiro >= 0xF0 && primeiro <= 0xF4) {
        return 4;
    }
    return 1;
}

static bool ehContinuacaoUtf8(int c)
{
    return c >= 0x80 && c <= 0xBF;
}

/*
 * Nenhum lexema comeca aqui: caractere invalido (DEC-48). Fora de STRING e
 * comentario so ha ASCII. Para dar uma sequencia util, um byte >= 0x80 que
 * inicia um UTF-8 bem formado e reportado inteiro (o 'ç' de "preço"); caso
 * contrario, so o byte. Nao e suporte a Unicode: nenhum desses bytes forma ID.
 */
static void erroCaractereInvalido(int line)
{
    LexemeBuffer sequencia;
    bufferIniciar(&sequencia);

    int primeiro = takeChar();
    bufferAdicionar(&sequencia, (char)primeiro);

    size_t tamanho = (primeiro >= 0x80) ? tamanhoUtf8(primeiro) : 1;
    bool utf8_valido = tamanho > 1;
    for (size_t i = 1; utf8_valido && i < tamanho; i++) {
        utf8_valido = ehContinuacaoUtf8(peekChar(i - 1));
    }
    if (utf8_valido) {
        for (size_t i = 1; i < tamanho; i++) {
            bufferAdicionar(&sequencia, (char)takeChar());
        }
    }
    erroLexico(line, &sequencia);
}

static Token lerSimbolo(int line)
{
    const LexemeEntry *entrada = buscarSimboloMaisLongo();
    if (entrada == NULL) {
        erroCaractereInvalido(line);
    }
    size_t tamanho = strlen(entrada->lexeme);
    for (size_t i = 0; i < tamanho; i++) {
        takeChar();
    }
    return criarTokenDeEntrada(entrada, line, duplicarTexto(entrada->lexeme, tamanho));
}

/* ======================================================================== */
/* 13. obterToken()                                                          */
/* ======================================================================== */

/* Descarta whitespace e comentarios "//" (ate antes de \r, \n ou EOF); eles nunca viram token. */
static void pularEspacosEComentarios(void)
{
    for (;;) {
        int c = peekChar(0);
        if (ehEspaco(c)) {
            takeChar();
        } else if (c == '/' && peekChar(1) == '/') {
            while (peekChar(0) != EOF && peekChar(0) != '\r' && peekChar(0) != '\n') {
                takeChar();
            }
        } else {
            return;
        }
    }
}

/*
 * Interface do analisador lexico (REQ-32): devolve o proximo token. Cada
 * token, exceto TOKEN_EOF, e emitido na tela e em tokens.txt ANTES de ser
 * devolvido (DEC-56), de modo que o parser da Fase G chamara obterToken() sob
 * demanda e a listagem continuara sendo produzida. Quem recebe o token deve
 * chamar liberarToken(). TOKEN_EOF tem a linha logica do cursor no fim da
 * entrada (DEC-54).
 */
Token obterToken(void)
{
    pularEspacosEComentarios();

    int c = peekChar(0);
    int line = scanner.line; /* linha do PRIMEIRO byte do lexema */

    if (c == EOF) {
        return criarToken(TOKEN_EOF, line, NULL);
    }

    Token token;
    if (ehLetraAscii(c)) {
        token = lerIdentificadorOuReservada(line);
    } else if (ehDigitoAscii(c)) {
        token = lerNumero(line);
    } else if (c == '"') {
        token = lerString(line);
    } else {
        token = lerSimbolo(line);
    }
    emitirToken(&token);
    return token;
}

/* ======================================================================== */
/* 14. Inicializacao e finalizacao                                           */
/* ======================================================================== */

/* Abre a fonte (em binario: bytes preservados, CRLF sob controle do scanner) e tokens.txt. */
static bool iniciarAnalisadorLexico(const char *caminho_fonte)
{
    scanner.source = fopen(caminho_fonte, "rb");
    if (scanner.source == NULL) {
        fprintf(stderr, "erro: nao foi possivel abrir o arquivo '%s'\n", caminho_fonte);
        return false;
    }
    token_output = fopen(ARQUIVO_SAIDA_TOKENS, "wb");
    if (token_output == NULL) {
        fprintf(stderr, "erro: nao foi possivel criar o arquivo '%s'\n", ARQUIVO_SAIDA_TOKENS);
        fclose(scanner.source);
        scanner.source = NULL;
        return false;
    }
    scanner.line = 1;
    scanner.lookahead_count = 0;
    memset(&symbol_table, 0, sizeof symbol_table);
    return true;
}

/* Fecha arquivos e libera a tabela de simbolos. Pode ser chamada mais de uma vez. */
static void fecharAnalisadorLexico(void)
{
    fflush(stdout);
    if (token_output != NULL) {
        fclose(token_output);
        token_output = NULL;
    }
    if (scanner.source != NULL) {
        fclose(scanner.source);
        scanner.source = NULL;
    }
    tabelaSimbolosLiberar();
}

/* ======================================================================== */
/* 15. Infraestrutura do analisador sintatico                                */
/* ======================================================================== */

static void escreverIndentacao(void)
{
    for (int i = 0; i < profundidade_arvore; i++) {
        fputs("  ", arvore_output);
    }
}

/* Registra o nao-terminal e a producao escolhida antes de expandir a RHS. */
static void abrirNo(const char *nome, int producao)
{
    escreverIndentacao();
    fprintf(arvore_output, "%s [P%02d]\n", nome, producao);
    profundidade_arvore++;
}

static void fecharNo(void)
{
    profundidade_arvore--;
}

/* A folha e escrita enquanto o lexema ainda pertence ao lookahead. */
static void registrarTerminal(const Token *token)
{
    escreverIndentacao();
    fputs(TOKEN_NAMES[token->type], arvore_output);
    switch (token->type) {
    case TOKEN_ID:
    case TOKEN_NUM_INT:
    case TOKEN_NUM_REAL:
    case TOKEN_STRING:
    case TOKEN_OP_REL:
    case TOKEN_OP_MULT:
        fprintf(arvore_output, " %s", token->lexeme);
        break;
    default:
        break;
    }
    fputc('\n', arvore_output);
}

static void registrarEpsilon(void)
{
    escreverIndentacao();
    fputs("\xCE\xB5\n", arvore_output); /* epsilon em UTF-8 */
}

static void registrarErroNaArvore(void)
{
    if (arvore_output != NULL) {
        escreverIndentacao();
        fputs("<ERRO SINTATICO>\n", arvore_output);
    }
}

/* Libera exatamente uma vez o lookahead e fecha a arvore, se existirem. */
static void fecharAnalisadorSintatico(void)
{
    if (lookahead_valido) {
        liberarToken(&lookahead);
        lookahead_valido = false;
    }
    if (arvore_output != NULL) {
        fclose(arvore_output);
        arvore_output = NULL;
    }
    profundidade_arvore = 0;
}

/* Erro sintatico fica apenas em stdout; tokens.txt continua sendo saida lexical. */
static void erroSintatico(const char *esperado)
{
    registrarErroNaArvore();
    printf("ERRO SINTÁTICO - linha %d - token: %s", lookahead.line,
           TOKEN_NAMES[lookahead.type]);
    if (esperado != NULL) {
        printf(" - esperado: %s", esperado);
    }
    fputc('\n', stdout);
    fecharAnalisadorSintatico();
    fecharAnalisadorLexico();
    exit(EXIT_SOURCE_ERROR);
}

/* Unica funcao do parser que chama obterToken(). */
static void nextToken(void)
{
    if (lookahead_valido) {
        liberarToken(&lookahead);
        lookahead_valido = false;
    }
    lookahead = obterToken();
    lookahead_valido = true;
}

/* Centraliza o casamento de terminais, a folha da arvore e o avanco. */
static void consome(TokenName esperado)
{
    if (lookahead.type != esperado) {
        erroSintatico(TOKEN_NAMES[esperado]);
    }
    registrarTerminal(&lookahead);
    nextToken();
}

static void exigirFimArquivo(void)
{
    if (lookahead.type != TOKEN_EOF) {
        erroSintatico("EOF");
    }
}

/* FIRST(<comando>) = C, em docs/analise-ll1.md. */
static bool ehInicioComando(TokenName tipo)
{
    return tipo == TOKEN_ID || tipo == TOKEN_KW_LEIA || tipo == TOKEN_KW_ESCREVA ||
           tipo == TOKEN_KW_ESCREVAL || tipo == TOKEN_KW_SE || tipo == TOKEN_KW_PARA ||
           tipo == TOKEN_KW_ENQUANTO || tipo == TOKEN_KW_RETORNE;
}

/* FOLLOW(<lista_comandos>) = F. */
static bool ehFimListaComandos(TokenName tipo)
{
    return tipo == TOKEN_KW_FIMALGORITMO || tipo == TOKEN_KW_SENAO ||
           tipo == TOKEN_KW_FIMSE || tipo == TOKEN_KW_FIMPARA ||
           tipo == TOKEN_KW_FIMENQUANTO || tipo == TOKEN_KW_FIMPROCEDIMENTO ||
           tipo == TOKEN_KW_FIMFUNCAO;
}

static bool ehInicioExpressao(TokenName tipo)
{
    return tipo == TOKEN_ID || tipo == TOKEN_NUM_INT || tipo == TOKEN_NUM_REAL ||
           tipo == TOKEN_STRING || tipo == TOKEN_KW_VERDADEIRO ||
           tipo == TOKEN_KW_FALSO || tipo == TOKEN_ABRE_PAR;
}

/* X = FOLLOW(<expressao>): C U F U fechamentos/separadores de expressao. */
static bool estaEmX(TokenName tipo)
{
    return ehInicioComando(tipo) || ehFimListaComandos(tipo) || tipo == TOKEN_KW_ATE ||
           tipo == TOKEN_KW_PASSO || tipo == TOKEN_KW_FACA || tipo == TOKEN_FECHA_PAR ||
           tipo == TOKEN_FECHA_COL || tipo == TOKEN_VIRGULA;
}

static bool estaNoSelectP75(TokenName tipo)
{
    return estaEmX(tipo) || tipo == TOKEN_E || tipo == TOKEN_OU;
}

static bool estaNoSelectP78(TokenName tipo)
{
    return estaNoSelectP75(tipo) || tipo == TOKEN_OP_REL;
}

static bool estaNoSelectP81(TokenName tipo)
{
    return estaNoSelectP78(tipo) || tipo == TOKEN_MAIS;
}

static bool estaNoSelectP91(TokenName tipo)
{
    return estaNoSelectP81(tipo) || tipo == TOKEN_OP_MULT;
}

/* ======================================================================== */
/* 16. Prototipos dos 50 nao-terminais                                       */
/* ======================================================================== */

static void programa(void);
static void corpoPrograma(void);
static void secaoVar(void);
static void secaoVarOpcional(void);
static void bloco(void);
static void listaDeclaracoes(void);
static void declaracao(void);
static void listaIds(void);
static void listaIdsCauda(void);
static void tipoDecl(void);
static void tipoVetor(void);
static void tipoSimples(void);
static void listaRotinas(void);
static void rotina(void);
static void procedimento(void);
static void parametrosProcedimento(void);
static void funcao(void);
static void listaParametros(void);
static void listaParametrosCauda(void);
static void parametro(void);
static void listaComandos(void);
static void listaComandosCauda(void);
static void comando(void);
static void cmdId(void);
static void caudaComandoId(void);
static void cmdLeia(void);
static void referencia(void);
static void indiceOpcional(void);
static void cmdEscreva(void);
static void cmdEscreval(void);
static void cmdSe(void);
static void senaoOpcional(void);
static void cmdPara(void);
static void passoOpcional(void);
static void numeroPasso(void);
static void cmdEnquanto(void);
static void cmdRetorne(void);
static void listaArgumentos(void);
static void listaArgumentosCauda(void);
static void expressao(void);
static void exprLogica(void);
static void exprLogicaCauda(void);
static void exprRelacional(void);
static void exprRelCauda(void);
static void exprAditiva(void);
static void exprAditivaCauda(void);
static void exprMult(void);
static void exprMultCauda(void);
static void primario(void);
static void caudaPrimario(void);

/* ======================================================================== */
/* 17. Programa, declaracoes e sub-rotinas (P01-P32)                         */
/* ======================================================================== */

static void programa(void)
{
    if (lookahead.type != TOKEN_KW_ALGORITMO) {
        erroSintatico("ALGORITMO");
    }
    abrirNo("<programa>", 1);
    consome(TOKEN_KW_ALGORITMO);
    consome(TOKEN_STRING);
    corpoPrograma();
    consome(TOKEN_KW_FIMALGORITMO);
    fecharNo();
}

static void corpoPrograma(void)
{
    if (lookahead.type == TOKEN_KW_VAR) { /* P02 */
        abrirNo("<corpo_programa>", 2);
        secaoVar();
        bloco();
    } else if (lookahead.type == TOKEN_KW_PROCEDIMENTO ||
               lookahead.type == TOKEN_KW_FUNCAO) { /* P03 */
        abrirNo("<corpo_programa>", 3);
        rotina();
        listaRotinas();
        secaoVarOpcional();
        bloco();
    } else {
        erroSintatico("VAR, PROCEDIMENTO ou FUNCAO");
    }
    fecharNo();
}

static void secaoVar(void)
{
    if (lookahead.type != TOKEN_KW_VAR) {
        erroSintatico("VAR");
    }
    abrirNo("<secao_var>", 4);
    consome(TOKEN_KW_VAR);
    listaDeclaracoes();
    fecharNo();
}

static void secaoVarOpcional(void)
{
    if (lookahead.type == TOKEN_KW_VAR) { /* P05 */
        abrirNo("<secao_var_opcional>", 5);
        secaoVar();
    } else if (lookahead.type == TOKEN_KW_INICIO) { /* P06: epsilon */
        abrirNo("<secao_var_opcional>", 6);
        registrarEpsilon();
    } else {
        erroSintatico("VAR ou INICIO");
    }
    fecharNo();
}

static void bloco(void)
{
    if (lookahead.type != TOKEN_KW_INICIO) {
        erroSintatico("INICIO");
    }
    abrirNo("<bloco>", 7);
    consome(TOKEN_KW_INICIO);
    listaComandos();
    fecharNo();
}

static void listaDeclaracoes(void)
{
    if (lookahead.type == TOKEN_ID) { /* P08 */
        abrirNo("<lista_declaracoes>", 8);
        declaracao();
        listaDeclaracoes();
    } else if (lookahead.type == TOKEN_KW_INICIO) { /* P09: epsilon */
        abrirNo("<lista_declaracoes>", 9);
        registrarEpsilon();
    } else {
        erroSintatico("ID ou INICIO");
    }
    fecharNo();
}

static void declaracao(void)
{
    if (lookahead.type != TOKEN_ID) {
        erroSintatico("ID");
    }
    abrirNo("<declaracao>", 10);
    listaIds();
    consome(TOKEN_DOIS_PONTOS);
    tipoDecl();
    fecharNo();
}

static void listaIds(void)
{
    if (lookahead.type != TOKEN_ID) {
        erroSintatico("ID");
    }
    abrirNo("<lista_ids>", 11);
    consome(TOKEN_ID);
    listaIdsCauda();
    fecharNo();
}

static void listaIdsCauda(void)
{
    if (lookahead.type == TOKEN_VIRGULA) { /* P12 */
        abrirNo("<lista_ids_cauda>", 12);
        consome(TOKEN_VIRGULA);
        consome(TOKEN_ID);
        listaIdsCauda();
    } else if (lookahead.type == TOKEN_DOIS_PONTOS) { /* P13: epsilon */
        abrirNo("<lista_ids_cauda>", 13);
        registrarEpsilon();
    } else {
        erroSintatico("VIRGULA ou DOIS_PONTOS");
    }
    fecharNo();
}

static void tipoDecl(void)
{
    if (lookahead.type == TOKEN_KW_INTEIRO || lookahead.type == TOKEN_KW_REAL ||
        lookahead.type == TOKEN_KW_CARACTERE || lookahead.type == TOKEN_KW_LOGICO) { /* P14 */
        abrirNo("<tipo_decl>", 14);
        tipoSimples();
    } else if (lookahead.type == TOKEN_KW_VETOR) { /* P15 */
        abrirNo("<tipo_decl>", 15);
        tipoVetor();
    } else {
        erroSintatico("INTEIRO, REAL, CARACTERE, LOGICO ou VETOR");
    }
    fecharNo();
}

static void tipoVetor(void)
{
    if (lookahead.type != TOKEN_KW_VETOR) {
        erroSintatico("VETOR");
    }
    abrirNo("<tipo_vetor>", 16);
    consome(TOKEN_KW_VETOR);
    consome(TOKEN_ABRE_COL);
    consome(TOKEN_NUM_INT);
    consome(TOKEN_INTERVALO);
    consome(TOKEN_NUM_INT);
    consome(TOKEN_FECHA_COL);
    consome(TOKEN_KW_DE);
    tipoSimples();
    fecharNo();
}

static void tipoSimples(void)
{
    TokenName tipo = lookahead.type;
    int producao;
    if (tipo == TOKEN_KW_INTEIRO) {
        producao = 17;
    } else if (tipo == TOKEN_KW_REAL) {
        producao = 18;
    } else if (tipo == TOKEN_KW_CARACTERE) {
        producao = 19;
    } else if (tipo == TOKEN_KW_LOGICO) {
        producao = 20;
    } else {
        erroSintatico("INTEIRO, REAL, CARACTERE ou LOGICO");
        return;
    }
    abrirNo("<tipo_simples>", producao);
    consome(tipo);
    fecharNo();
}

static void listaRotinas(void)
{
    if (lookahead.type == TOKEN_KW_PROCEDIMENTO || lookahead.type == TOKEN_KW_FUNCAO) { /* P21 */
        abrirNo("<lista_rotinas>", 21);
        rotina();
        listaRotinas();
    } else if (lookahead.type == TOKEN_KW_VAR || lookahead.type == TOKEN_KW_INICIO) { /* P22 */
        abrirNo("<lista_rotinas>", 22);
        registrarEpsilon();
    } else {
        erroSintatico("PROCEDIMENTO, FUNCAO, VAR ou INICIO");
    }
    fecharNo();
}

static void rotina(void)
{
    if (lookahead.type == TOKEN_KW_PROCEDIMENTO) { /* P23 */
        abrirNo("<rotina>", 23);
        procedimento();
    } else if (lookahead.type == TOKEN_KW_FUNCAO) { /* P24 */
        abrirNo("<rotina>", 24);
        funcao();
    } else {
        erroSintatico("PROCEDIMENTO ou FUNCAO");
    }
    fecharNo();
}

static void procedimento(void)
{
    if (lookahead.type != TOKEN_KW_PROCEDIMENTO) {
        erroSintatico("PROCEDIMENTO");
    }
    abrirNo("<procedimento>", 25);
    consome(TOKEN_KW_PROCEDIMENTO);
    consome(TOKEN_ID);
    parametrosProcedimento();
    bloco();
    consome(TOKEN_KW_FIMPROCEDIMENTO);
    fecharNo();
}

static void parametrosProcedimento(void)
{
    if (lookahead.type == TOKEN_ABRE_PAR) { /* P26 */
        abrirNo("<parametros_procedimento>", 26);
        consome(TOKEN_ABRE_PAR);
        listaParametros();
        consome(TOKEN_FECHA_PAR);
    } else if (lookahead.type == TOKEN_KW_INICIO) { /* P27: epsilon */
        abrirNo("<parametros_procedimento>", 27);
        registrarEpsilon();
    } else {
        erroSintatico("ABRE_PAR ou INICIO");
    }
    fecharNo();
}

static void funcao(void)
{
    if (lookahead.type != TOKEN_KW_FUNCAO) {
        erroSintatico("FUNCAO");
    }
    abrirNo("<funcao>", 28);
    consome(TOKEN_KW_FUNCAO);
    consome(TOKEN_ID);
    consome(TOKEN_ABRE_PAR);
    listaParametros();
    consome(TOKEN_FECHA_PAR);
    consome(TOKEN_DOIS_PONTOS);
    tipoSimples();
    bloco();
    consome(TOKEN_KW_FIMFUNCAO);
    fecharNo();
}

static void listaParametros(void)
{
    if (lookahead.type != TOKEN_ID) {
        erroSintatico("ID");
    }
    abrirNo("<lista_parametros>", 29);
    parametro();
    listaParametrosCauda();
    fecharNo();
}

static void listaParametrosCauda(void)
{
    if (lookahead.type == TOKEN_VIRGULA) { /* P30 */
        abrirNo("<lista_parametros_cauda>", 30);
        consome(TOKEN_VIRGULA);
        parametro();
        listaParametrosCauda();
    } else if (lookahead.type == TOKEN_FECHA_PAR) { /* P31: epsilon */
        abrirNo("<lista_parametros_cauda>", 31);
        registrarEpsilon();
    } else {
        erroSintatico("VIRGULA ou FECHA_PAR");
    }
    fecharNo();
}

static void parametro(void)
{
    if (lookahead.type != TOKEN_ID) {
        erroSintatico("ID");
    }
    abrirNo("<parametro>", 32);
    consome(TOKEN_ID);
    consome(TOKEN_DOIS_PONTOS);
    tipoSimples();
    fecharNo();
}

/* ======================================================================== */
/* 18. Comandos e argumentos (P33-P67)                                       */
/* ======================================================================== */

static void listaComandos(void)
{
    if (!ehInicioComando(lookahead.type)) {
        erroSintatico("inicio de comando");
    }
    abrirNo("<lista_comandos>", 33);
    comando();
    listaComandosCauda();
    fecharNo();
}

static void listaComandosCauda(void)
{
    if (ehInicioComando(lookahead.type)) { /* P34 */
        abrirNo("<lista_comandos_cauda>", 34);
        comando();
        listaComandosCauda();
    } else if (ehFimListaComandos(lookahead.type)) { /* P35: epsilon */
        abrirNo("<lista_comandos_cauda>", 35);
        registrarEpsilon();
    } else {
        erroSintatico("inicio de comando ou fechamento de bloco");
    }
    fecharNo();
}

static void comando(void)
{
    int producao;
    if (lookahead.type == TOKEN_ID) {
        producao = 36;
    } else if (lookahead.type == TOKEN_KW_LEIA) {
        producao = 37;
    } else if (lookahead.type == TOKEN_KW_ESCREVA) {
        producao = 38;
    } else if (lookahead.type == TOKEN_KW_ESCREVAL) {
        producao = 39;
    } else if (lookahead.type == TOKEN_KW_SE) {
        producao = 40;
    } else if (lookahead.type == TOKEN_KW_PARA) {
        producao = 41;
    } else if (lookahead.type == TOKEN_KW_ENQUANTO) {
        producao = 42;
    } else if (lookahead.type == TOKEN_KW_RETORNE) {
        producao = 43;
    } else {
        erroSintatico("inicio de comando");
        return;
    }

    abrirNo("<comando>", producao);
    switch (producao) {
    case 36: cmdId(); break;
    case 37: cmdLeia(); break;
    case 38: cmdEscreva(); break;
    case 39: cmdEscreval(); break;
    case 40: cmdSe(); break;
    case 41: cmdPara(); break;
    case 42: cmdEnquanto(); break;
    case 43: cmdRetorne(); break;
    }
    fecharNo();
}

static void cmdId(void)
{
    if (lookahead.type != TOKEN_ID) {
        erroSintatico("ID");
    }
    abrirNo("<cmd_id>", 44);
    consome(TOKEN_ID);
    caudaComandoId();
    fecharNo();
}

static void caudaComandoId(void)
{
    if (lookahead.type == TOKEN_ATRIBUICAO) { /* P45 */
        abrirNo("<cauda_comando_id>", 45);
        consome(TOKEN_ATRIBUICAO);
        expressao();
    } else if (lookahead.type == TOKEN_ABRE_COL) { /* P46 */
        abrirNo("<cauda_comando_id>", 46);
        consome(TOKEN_ABRE_COL);
        expressao();
        consome(TOKEN_FECHA_COL);
        consome(TOKEN_ATRIBUICAO);
        expressao();
    } else if (lookahead.type == TOKEN_ABRE_PAR) { /* P47 */
        abrirNo("<cauda_comando_id>", 47);
        consome(TOKEN_ABRE_PAR);
        listaArgumentos();
        consome(TOKEN_FECHA_PAR);
    } else if (ehInicioComando(lookahead.type) || ehFimListaComandos(lookahead.type)) { /* P48 */
        abrirNo("<cauda_comando_id>", 48);
        registrarEpsilon();
    } else {
        erroSintatico("ATRIBUICAO, ABRE_COL, ABRE_PAR ou fim de comando");
    }
    fecharNo();
}

static void cmdLeia(void)
{
    if (lookahead.type != TOKEN_KW_LEIA) {
        erroSintatico("LEIA");
    }
    abrirNo("<cmd_leia>", 49);
    consome(TOKEN_KW_LEIA);
    consome(TOKEN_ABRE_PAR);
    referencia();
    consome(TOKEN_FECHA_PAR);
    fecharNo();
}

static void referencia(void)
{
    if (lookahead.type != TOKEN_ID) {
        erroSintatico("ID");
    }
    abrirNo("<referencia>", 50);
    consome(TOKEN_ID);
    indiceOpcional();
    fecharNo();
}

static void indiceOpcional(void)
{
    if (lookahead.type == TOKEN_ABRE_COL) { /* P51 */
        abrirNo("<indice_opcional>", 51);
        consome(TOKEN_ABRE_COL);
        expressao();
        consome(TOKEN_FECHA_COL);
    } else if (lookahead.type == TOKEN_FECHA_PAR) { /* P52: epsilon */
        abrirNo("<indice_opcional>", 52);
        registrarEpsilon();
    } else {
        erroSintatico("ABRE_COL ou FECHA_PAR");
    }
    fecharNo();
}

static void cmdEscreva(void)
{
    if (lookahead.type != TOKEN_KW_ESCREVA) {
        erroSintatico("ESCREVA");
    }
    abrirNo("<cmd_escreva>", 53);
    consome(TOKEN_KW_ESCREVA);
    consome(TOKEN_ABRE_PAR);
    listaArgumentos();
    consome(TOKEN_FECHA_PAR);
    fecharNo();
}

static void cmdEscreval(void)
{
    if (lookahead.type != TOKEN_KW_ESCREVAL) {
        erroSintatico("ESCREVAL");
    }
    abrirNo("<cmd_escreval>", 54);
    consome(TOKEN_KW_ESCREVAL);
    consome(TOKEN_ABRE_PAR);
    listaArgumentos();
    consome(TOKEN_FECHA_PAR);
    fecharNo();
}

static void cmdSe(void)
{
    if (lookahead.type != TOKEN_KW_SE) {
        erroSintatico("SE");
    }
    abrirNo("<cmd_se>", 55);
    consome(TOKEN_KW_SE);
    consome(TOKEN_ABRE_PAR);
    expressao();
    consome(TOKEN_FECHA_PAR);
    consome(TOKEN_KW_ENTAO);
    listaComandos();
    senaoOpcional();
    consome(TOKEN_KW_FIMSE);
    fecharNo();
}

static void senaoOpcional(void)
{
    if (lookahead.type == TOKEN_KW_SENAO) { /* P56 */
        abrirNo("<senao_opcional>", 56);
        consome(TOKEN_KW_SENAO);
        listaComandos();
    } else if (lookahead.type == TOKEN_KW_FIMSE) { /* P57: epsilon */
        abrirNo("<senao_opcional>", 57);
        registrarEpsilon();
    } else {
        erroSintatico("SENAO ou FIMSE");
    }
    fecharNo();
}

static void cmdPara(void)
{
    if (lookahead.type != TOKEN_KW_PARA) {
        erroSintatico("PARA");
    }
    abrirNo("<cmd_para>", 58);
    consome(TOKEN_KW_PARA);
    consome(TOKEN_ID);
    consome(TOKEN_KW_DE);
    expressao();
    consome(TOKEN_KW_ATE);
    expressao();
    passoOpcional();
    consome(TOKEN_KW_FACA);
    listaComandos();
    consome(TOKEN_KW_FIMPARA);
    fecharNo();
}

static void passoOpcional(void)
{
    if (lookahead.type == TOKEN_KW_PASSO) { /* P59 */
        abrirNo("<passo_opcional>", 59);
        consome(TOKEN_KW_PASSO);
        numeroPasso();
    } else if (lookahead.type == TOKEN_KW_FACA) { /* P60: epsilon */
        abrirNo("<passo_opcional>", 60);
        registrarEpsilon();
    } else {
        erroSintatico("PASSO ou FACA");
    }
    fecharNo();
}

static void numeroPasso(void)
{
    if (lookahead.type == TOKEN_NUM_INT) { /* P61 */
        abrirNo("<numero_passo>", 61);
        consome(TOKEN_NUM_INT);
    } else if (lookahead.type == TOKEN_MENOS) { /* P62 */
        abrirNo("<numero_passo>", 62);
        consome(TOKEN_MENOS);
        consome(TOKEN_NUM_INT);
    } else {
        erroSintatico("NUM_INT ou MENOS");
    }
    fecharNo();
}

static void cmdEnquanto(void)
{
    if (lookahead.type != TOKEN_KW_ENQUANTO) {
        erroSintatico("ENQUANTO");
    }
    abrirNo("<cmd_enquanto>", 63);
    consome(TOKEN_KW_ENQUANTO);
    consome(TOKEN_ABRE_PAR);
    expressao();
    consome(TOKEN_FECHA_PAR);
    consome(TOKEN_KW_FACA);
    listaComandos();
    consome(TOKEN_KW_FIMENQUANTO);
    fecharNo();
}

static void cmdRetorne(void)
{
    if (lookahead.type != TOKEN_KW_RETORNE) {
        erroSintatico("RETORNE");
    }
    abrirNo("<cmd_retorne>", 64);
    consome(TOKEN_KW_RETORNE);
    expressao();
    fecharNo();
}

static void listaArgumentos(void)
{
    if (!ehInicioExpressao(lookahead.type)) {
        erroSintatico("inicio de expressao");
    }
    abrirNo("<lista_argumentos>", 65);
    expressao();
    listaArgumentosCauda();
    fecharNo();
}

static void listaArgumentosCauda(void)
{
    if (lookahead.type == TOKEN_VIRGULA) { /* P66 */
        abrirNo("<lista_argumentos_cauda>", 66);
        consome(TOKEN_VIRGULA);
        expressao();
        listaArgumentosCauda();
    } else if (lookahead.type == TOKEN_FECHA_PAR) { /* P67: epsilon */
        abrirNo("<lista_argumentos_cauda>", 67);
        registrarEpsilon();
    } else {
        erroSintatico("VIRGULA ou FECHA_PAR");
    }
    fecharNo();
}

/* ======================================================================== */
/* 19. Expressoes (P68-P91)                                                  */
/* ======================================================================== */

static void expressao(void)
{
    if (!ehInicioExpressao(lookahead.type)) {
        erroSintatico("inicio de expressao");
    }
    abrirNo("<expressao>", 68);
    exprLogica();
    fecharNo();
}

static void exprLogica(void)
{
    if (!ehInicioExpressao(lookahead.type)) {
        erroSintatico("inicio de expressao");
    }
    abrirNo("<expr_logica>", 69);
    exprRelacional();
    exprLogicaCauda();
    fecharNo();
}

static void exprLogicaCauda(void)
{
    if (lookahead.type == TOKEN_E) { /* P70 */
        abrirNo("<expr_logica_cauda>", 70);
        consome(TOKEN_E);
        exprRelacional();
        exprLogicaCauda();
    } else if (lookahead.type == TOKEN_OU) { /* P71 */
        abrirNo("<expr_logica_cauda>", 71);
        consome(TOKEN_OU);
        exprRelacional();
        exprLogicaCauda();
    } else if (estaEmX(lookahead.type)) { /* P72: epsilon */
        abrirNo("<expr_logica_cauda>", 72);
        registrarEpsilon();
    } else {
        erroSintatico("E, OU ou fim de expressao");
    }
    fecharNo();
}

static void exprRelacional(void)
{
    if (!ehInicioExpressao(lookahead.type)) {
        erroSintatico("inicio de expressao");
    }
    abrirNo("<expr_relacional>", 73);
    exprAditiva();
    exprRelCauda();
    fecharNo();
}

static void exprRelCauda(void)
{
    if (lookahead.type == TOKEN_OP_REL) { /* P74 */
        abrirNo("<expr_rel_cauda>", 74);
        consome(TOKEN_OP_REL);
        exprAditiva();
    } else if (estaNoSelectP75(lookahead.type)) { /* P75: epsilon */
        abrirNo("<expr_rel_cauda>", 75);
        registrarEpsilon();
    } else {
        erroSintatico("OP_REL ou fim do nivel relacional");
    }
    fecharNo();
}

static void exprAditiva(void)
{
    if (!ehInicioExpressao(lookahead.type)) {
        erroSintatico("inicio de expressao");
    }
    abrirNo("<expr_aditiva>", 76);
    exprMult();
    exprAditivaCauda();
    fecharNo();
}

static void exprAditivaCauda(void)
{
    if (lookahead.type == TOKEN_MAIS) { /* P77 */
        abrirNo("<expr_aditiva_cauda>", 77);
        consome(TOKEN_MAIS);
        exprMult();
        exprAditivaCauda();
    } else if (estaNoSelectP78(lookahead.type)) { /* P78: epsilon */
        abrirNo("<expr_aditiva_cauda>", 78);
        registrarEpsilon();
    } else {
        erroSintatico("MAIS ou fim do nivel aditivo");
    }
    fecharNo();
}

static void exprMult(void)
{
    if (!ehInicioExpressao(lookahead.type)) {
        erroSintatico("inicio de expressao");
    }
    abrirNo("<expr_mult>", 79);
    primario();
    exprMultCauda();
    fecharNo();
}

static void exprMultCauda(void)
{
    if (lookahead.type == TOKEN_OP_MULT) { /* P80 */
        abrirNo("<expr_mult_cauda>", 80);
        consome(TOKEN_OP_MULT);
        primario();
        exprMultCauda();
    } else if (estaNoSelectP81(lookahead.type)) { /* P81: epsilon */
        abrirNo("<expr_mult_cauda>", 81);
        registrarEpsilon();
    } else {
        erroSintatico("OP_MULT ou fim do nivel multiplicativo");
    }
    fecharNo();
}

static void primario(void)
{
    int producao;
    TokenName terminal = lookahead.type;
    if (terminal == TOKEN_ID) {
        producao = 82;
    } else if (terminal == TOKEN_NUM_INT) {
        producao = 83;
    } else if (terminal == TOKEN_NUM_REAL) {
        producao = 84;
    } else if (terminal == TOKEN_STRING) {
        producao = 85;
    } else if (terminal == TOKEN_KW_VERDADEIRO) {
        producao = 86;
    } else if (terminal == TOKEN_KW_FALSO) {
        producao = 87;
    } else if (terminal == TOKEN_ABRE_PAR) {
        producao = 88;
    } else {
        erroSintatico("ID, NUM_INT, NUM_REAL, STRING, VERDADEIRO, FALSO ou ABRE_PAR");
        return;
    }

    abrirNo("<primario>", producao);
    if (producao == 82) {
        consome(TOKEN_ID);
        caudaPrimario();
    } else if (producao == 88) {
        consome(TOKEN_ABRE_PAR);
        expressao();
        consome(TOKEN_FECHA_PAR);
    } else {
        consome(terminal);
    }
    fecharNo();
}

static void caudaPrimario(void)
{
    if (lookahead.type == TOKEN_ABRE_COL) { /* P89 */
        abrirNo("<cauda_primario>", 89);
        consome(TOKEN_ABRE_COL);
        expressao();
        consome(TOKEN_FECHA_COL);
    } else if (lookahead.type == TOKEN_ABRE_PAR) { /* P90 */
        abrirNo("<cauda_primario>", 90);
        consome(TOKEN_ABRE_PAR);
        listaArgumentos();
        consome(TOKEN_FECHA_PAR);
    } else if (estaNoSelectP91(lookahead.type)) { /* P91: epsilon */
        abrirNo("<cauda_primario>", 91);
        registrarEpsilon();
    } else {
        erroSintatico("ABRE_COL, ABRE_PAR ou fim de primario");
    }
    fecharNo();
}

/* ======================================================================== */
/* 20. Driver integrado e main                                               */
/* ======================================================================== */

static bool executarAnaliseSintatica(void)
{
    arvore_output = fopen(ARQUIVO_SAIDA_ARVORE, "wb");
    if (arvore_output == NULL) {
        fprintf(stderr, "erro: nao foi possivel criar o arquivo '%s'\n", ARQUIVO_SAIDA_ARVORE);
        return false;
    }
    profundidade_arvore = 0;
    lookahead_valido = false;

    nextToken();
    programa();
    exigirFimArquivo();
    fecharAnalisadorSintatico();
    return true;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "uso: %s arquivo.alg\n", argc > 0 ? argv[0] : "compilador");
        return EXIT_OPERATIONAL_ERROR;
    }
    if (!iniciarAnalisadorLexico(argv[1])) {
        return EXIT_OPERATIONAL_ERROR;
    }
    if (!executarAnaliseSintatica()) {
        fecharAnalisadorLexico();
        return EXIT_OPERATIONAL_ERROR;
    }
    fecharAnalisadorLexico();
    return EXIT_SUCCESS;
}

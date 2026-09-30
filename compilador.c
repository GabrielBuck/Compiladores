/*
 * PROJETO 1 - COMPILADORES
 * 
 * INTEGRANTES:
 * - Gabriel Nottoli Buck
 * - Julia Andrade
 * - Joao vitor rocha miranda
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ==========================================================================
 * CONSTANTES E MACROS
 * ========================================================================== */
#define MAX_LEXEMA 256
#define MAX_TS 1000

/* ==========================================================================
 * ENUMS
 * ========================================================================== */

/* Enumeração dos tipos de token */
typedef enum {
    TOKEN_EOF,
    TOKEN_ERRO,
    
    /* Identificador, Literais */
    TOKEN_ID,
    TOKEN_INT,
    TOKEN_REAL,
    TOKEN_STRING,
    
    /* Palavras Reservadas */
    TOKEN_KW_ALGORITMO,
    TOKEN_KW_VAR,
    TOKEN_KW_INICIO,
    TOKEN_KW_FIMALGORITMO,
    TOKEN_KW_ESCREVAL
    
    /* TODO: Adicionar os demais tokens de palavras reservadas e pontuação */
} TokenName;

/* Tipo do atributo quando aplicável */
typedef enum {
    ATTR_NONE,
    ATTR_INT,
    ATTR_FLOAT,
    ATTR_TS_INDEX,
    ATTR_OP_CODE
} AttrType;

/* ==========================================================================
 * STRUCTS
 * ========================================================================== */

/* Estrutura do Token */
typedef struct {
    TokenName type;
    int line;
    AttrType attr_type;
    
    union {
        int table_index;
        int int_value;
        double float_value;
        int op_code; /* Representação em enum ou int dos op rel/aritméticos */
    } attribute;
} Token;

/* Estrutura da Tabela de Símbolos */
typedef struct {
    char lexema[MAX_LEXEMA];
    /* TODO: campos adicionais se necessário para fases posteriores */
} Simbolo;

/* ==========================================================================
 * VARIÁVEIS GLOBAIS (ESTADO)
 * ========================================================================== */
FILE* arquivo_fonte = NULL;
int linha_atual = 1;

Simbolo tabela_simbolos[MAX_TS];
int total_simbolos = 0;

Token token_atual;

/* ==========================================================================
 * PROTÓTIPOS
 * ========================================================================== */
void inicializar_TS(void);
Token obterToken(void);
void nextToken(void);
void analisar_programa(void);

/* ==========================================================================
 * FUNÇÕES AUXILIARES / TABELA DE SÍMBOLOS
 * ========================================================================== */

void inicializar_TS(void) {
    total_simbolos = 0;
    /* TODO: Pré-carregar palavras reservadas se utilizarmos a TS para isso, 
       ou deixá-las isoladas e usar TS apenas para identificadores. */
}

/* ==========================================================================
 * ANALISADOR LÉXICO
 * ========================================================================== */

Token obterToken(void) {
    Token t;
    t.type = TOKEN_EOF;
    t.line = linha_atual;
    t.attr_type = ATTR_NONE;

    /* TODO: Implementar lógica de leitura caractere a caractere */
    
    /* Temporariamente, apenas simulamos o fim do arquivo ou retorno vazio 
       já que o analisador não está construído ainda. */
    int c = fgetc(arquivo_fonte);
    if (c == EOF) {
        return t;
    }

    /* Pseudo-consumo para não travar num laço infinito de teste */
    while (c != EOF) {
        c = fgetc(arquivo_fonte);
    }
    
    return t;
}

/* ==========================================================================
 * ANALISADOR SINTÁTICO E INTERAÇÃO
 * ========================================================================== */

void nextToken(void) {
    token_atual = obterToken();
    
    /* Impressão exigida pelo projeto (por enquanto comentada até gerar dados reais) */
    /* printf("%d# %d | ...\n", token_atual.line, token_atual.type); */
}

void analisar_programa(void) {
    printf("[MENSAGEM] O Parser ainda esta em construcao. Nenhuma regra analisada.\n");
    /* TODO: Iniciar a cadeia de funções preditivas:
       Programa();
    */
}

/* ==========================================================================
 * MAIN
 * ========================================================================== */

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <arquivo_fonte>\n", argv[0]);
        return 1;
    }

    arquivo_fonte = fopen(argv[1], "r");
    if (!arquivo_fonte) {
        fprintf(stderr, "Erro ao abrir o arquivo: %s\n", argv[1]);
        return 1;
    }

    inicializar_TS();

    /* Prepara o primeiro token para o parser */
    nextToken();

    /* Inicia a análise sintática */
    analisar_programa();

    fclose(arquivo_fonte);
    return 0;
}

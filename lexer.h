#pragma once

#include <stdio.h>

#define TOKEN_SIZE 256
#define MINUS_SIGN '-'

typedef enum {
    T_LPAREN,
    T_RPAREN,
    T_STRING,
    T_NUMBER,
    T_SYMBOL,
    T_EOF
} TokenType;

typedef struct {
    TokenType type;
    char value[TOKEN_SIZE];
    int line;
    int column;
} Token;

typedef struct {
    const char* source;
    size_t index;
    int line;
    int column;
} Lexer;

void init_lexer(Lexer* lexer, const char* source);

char curr_char(Lexer* lexer);

char next_char(Lexer* lexer);

int is_symbol(char c);

Token next_token(Lexer* lexer);

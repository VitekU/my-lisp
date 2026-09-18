#pragma once
#include <stdio.h>
#include "lexer.h"

#define INIT_LIST_CAPACITY 4

#define DEF_KEY "def"
#define FN_KEY "fn"
#define IF_KEY "if"
#define PLUS "+"
#define MINUS "-"
#define DIVIDE "/"
#define MULT "*"
#define EQUALS "=="
#define GREATER_THAN ">"
#define LESSER_THAN "<"

typedef enum {
    N_NUMBER,
    N_STRING,
    N_SYMBOL,
    N_LIST,
    N_NIL
} NodeType;

typedef enum {
    S_DEF,
    S_FN,
    S_IF,
    S_PLUS,
    S_MINUS,
    S_DIVIDE,
    S_MULT,
    S_EQUALS,
    S_GREATER_THAN,
    S_LESSER_THAN
} SymbolID;

typedef struct Node {
    NodeType type;
    union {
        int number;
        char* string;
        struct {
            char* symbol;
            SymbolID symbolID;
        } symbol;
        struct {
            struct Node **elements;
            size_t count;
            size_t capacity;
        } list;
    };
} Node;

typedef struct {
    Token token;
    Lexer *lexer;
} Parser;

void init_parser(Parser* parser, Lexer* lexer);
void parser_next(Parser* parser);
Token parser_curr(Parser* parser);

Node* create_number(int n);
Node* create_string(char* s);
Node* create_symbol(char* s);
Node* create_list();
Node* create_nil();
void add_to_list(Node* list, Node* element);

Node* parse_expression(Parser* parser);
Node* parse_list(Parser* parser);

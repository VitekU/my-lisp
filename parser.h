#pragma once
#include <stdio.h>
#include "lexer.h"

#define INIT_LIST_CAPACITY 4

typedef enum {
    N_NUMBER,
    N_STRING,
    N_SYMBOL,
    N_LIST,
    N_NIL
} NodeType;

typedef struct Node {
    NodeType type;
    union {
        int number;
        char* string;
        char* symbol;
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

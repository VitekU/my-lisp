#include "parser.h"
#include "arena.h"
#include "lexer.h"
#include <ctype.h>
#include <err.h>
#include <stdlib.h>
#include <string.h>


Node *create_number(int n) {
    Node *node = arena_allocate(sizeof(Node));
    node->type = N_NUMBER;
    node->number = n;
    return node;
}
Node *create_string(char *s) {
    Node *node = arena_allocate(sizeof(Node));
    node->type = N_STRING;
    node->string = strdup(s);
    return node;
}
Node *create_symbol(char *s) {
    Node *node = arena_allocate(sizeof(Node));
    node->type = N_SYMBOL;
    node->symbol = strdup(s);
    return node;
}
Node *create_list() {
    Node *node = arena_allocate(sizeof(Node));
    node->type = N_LIST;
    node->list.count = 0;
    node->list.capacity = INIT_LIST_CAPACITY;
    node->list.elements = arena_allocate(sizeof(Node*) * INIT_LIST_CAPACITY);
    return node;
}

Node *create_nil() {
    Node *node = arena_allocate(sizeof(Node));
    node->type = N_NIL;
    return node;
}

void add_to_list(Node* list, Node* element) {
    // reallocates the memory in case of the dynamic array being full
    if (list->list.count >= list->list.capacity) {
        list->list.capacity *= 2;
        list->list.elements = arena_allocate(sizeof(Node*) * list->list.capacity);
    }

    list->list.elements[list->list.count] = element;
    list->list.count++;
}

void init_parser(Parser* parser, Lexer* lexer) {
    parser->token = next_token(lexer);
    parser->lexer = lexer;
}

void parser_next(Parser* parser) {
    Token t = next_token(parser->lexer);
    parser->token = t;
}

Token parser_curr(Parser* parser) {
    return parser->token;
}

Node* parse_list(Parser* parser) {
    Node* list = create_list();

    // move past the L_PAREN token
    parser_next(parser);


    while (parser_curr(parser).type != T_RPAREN && parser_curr(parser).type != T_EOF) {
        add_to_list(list, parse_expression(parser));
    }

    if (parser_curr(parser).type != T_RPAREN) {
        errx(1, "Syntax error, expected ')'\n");
    }

    // move past the R_PAREN token
    parser_next(parser);

    return list;
}

Node* parse_expression(Parser* parser) {
    Token token = parser_curr(parser);

    switch (token.type) {
        case (T_NUMBER):
            parser_next(parser);
            return create_number(atoi(token.value));
        case (T_STRING):
            parser_next(parser);
            return create_string(token.value);
        case (T_SYMBOL):
            parser_next(parser);
            return create_symbol(token.value);
        case (T_LPAREN): {
            return parse_list(parser);
        }
        case (T_EOF):
            return create_nil();
        default:
            errx(1, "Unexpected token %s", token.value);
    }
}

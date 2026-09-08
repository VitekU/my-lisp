#pragma once

#include "parser.h"

typedef enum {
    V_NUMBER,
    V_STRING,
    V_FUNCTION,
    V_NIL
} ValueType;

typedef struct {
    ValueType type;
    union {
        int number;
        char *string;
    };
} Value;

typedef struct Pair {
    char *key;
    Value *value;
} Pair;

typedef struct Environment {
    struct Environment *parent;
    Pair *pairs;
} Environment;

typedef struct {
    Node *params;
    Node *body;
    Environment *env;
} Function;

Value *value_number(int n);
Value *value_string(char *s);

Value *eval(Node *node);

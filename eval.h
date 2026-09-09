#pragma once

#include "parser.h"

#define INIT_ENV_CAPACITY 4

typedef struct Function Function;
typedef struct Environment Environment;

typedef enum {
    V_NUMBER,
    V_STRING,
    V_FUNCTION,
    V_NIL
} ValueType;

struct Function {
    Node *params;
    Node *body;
    Environment *env;
};

typedef struct {
    ValueType type;
    union {
        int number;
        char *string;
        Function fn;
    };
} Value;

typedef struct Pair {
    char *key;
    Value *value;
} Pair;

struct Environment {
    struct Environment *parent;
    Pair *pairs;
    size_t count;
    size_t capacity;
};


Value *value_number(int n);
Value *value_string(char *s);
Value *value_function(Node *params, Node *body, Environment *env);

Environment *env_new(Environment *parent);
void set_variable(Environment *env, char *key, Value *val);
Value *get_variable(Environment *env, char *key);


Value *eval(Node *node, Environment *env);

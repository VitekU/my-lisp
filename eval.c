#include <_string.h>
#include <err.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "eval.h"
#include "parser.h"

Value *value_number(int n) {
    Value *value = malloc(sizeof(Value));
    value->type = V_NUMBER;
    value->number = n;
    return value;
}

Value *value_string(char *s) {
    Value *value = malloc(sizeof(Value));
    value->type = V_STRING;
    value->string = s;
    return value;
}

Value *value_nil() {
    Value *value = malloc(sizeof(Value));
    value->type = V_NIL;
    return value;
}

Environment *env_new(Environment *parent) {
    Environment *env = malloc(sizeof(Environment));
    env->parent = parent;
    env->count = 0;
    env->capacity = 4;
    env->pairs = malloc(sizeof(Pair) * INIT_ENV_CAPACITY);
    return env;
}

void set_variable(Environment *env, char *key, Value *val) {
    for (size_t i = 0; i < env->count; ++i) {
        if (strcmp(key, env->pairs[i].key) == 0) {
            env->pairs[i].value = val;
            return;
        }
    }

    if (env->count >= env->capacity) {
        env->capacity *= 2;
        env->pairs = realloc(env->pairs, sizeof(Pair) * env->capacity);
    }

    env->pairs[env->count].key = strdup(key);
    env->pairs[env->count].value = val;
    env->count++;
}

Value *get_variable(Environment *env, char *key) {
    for (size_t i = 0; i < env->count; ++i) {
        if (strcmp(key, env->pairs[i].key) == 0) {
            return env->pairs[i].value;
        }
    }
    if (env->parent != NULL) {
        return get_variable(env->parent, key);
    }
    errx(1, "Undefined variable");
}

Value *value_function(Node *params, Node *body, Environment *env) {
    Value *value = malloc(sizeof(Value));
    value->type = V_FUNCTION;
    value->fn.params = params;
    value->fn.body = body;
    value->fn.env = env;

    return value;
}


Value *eval(Node *node, Environment *env) {
    if (node->type == N_NUMBER) {
        return value_number(node->number);
    }
    if (node->type == N_STRING) {
        return value_string(node->string);
    }

    if (node->type == N_SYMBOL) {
        return get_variable(env, node->symbol);
    }

    if (node->type == N_LIST) {
        Node *keyword = node->list.elements[0];

        if (keyword->type == N_SYMBOL) {
            if (strcmp(keyword->symbol, "def") == 0) {
                char *name = node->list.elements[1]->symbol;
                Value *value = eval(node->list.elements[2], env);
                set_variable(env, name, value);
                return value;
            }

            if (strcmp(keyword->symbol, "fn") == 0) {
                Node *params = node->list.elements[1];
                Node *body = node->list.elements[2];
                return value_function(params, body, env);
            }

            if (strcmp(keyword->symbol, "+") == 0) {
                int n = eval(node->list.elements[1], env)->number + eval(node->list.elements[2], env)->number;
                return value_number(n);
            }
            else if (strcmp(keyword->symbol, "-") == 0) {
                int n = eval(node->list.elements[1], env)->number - eval(node->list.elements[2], env)->number;
                return value_number(n);
            }
            else if (strcmp(keyword->symbol, "*") == 0) {
                int n = eval(node->list.elements[1], env)->number * eval(node->list.elements[2], env)->number;
                return value_number(n);
            }
            else if (strcmp(keyword->symbol, "/") == 0) {
                int n = eval(node->list.elements[1], env)->number / eval(node->list.elements[2], env)->number;
                return value_number(n);
            }

            Value *function_to_eval = eval(keyword, env);
            if (function_to_eval->type == V_FUNCTION) {
                Function fn = function_to_eval->fn;

                Environment *fn_env = env_new(fn.env);

                for (int i = 0; i < fn.params->list.count; ++i) {
                    char *arg_name = fn.params->list.elements[i]->symbol;
                    Value * arg_value = eval(node->list.elements[i + 1], env);
                    set_variable(fn_env, arg_name, arg_value);
                }

                return eval(fn.body, fn_env);
            }
        }
    }
    return value_nil();
}

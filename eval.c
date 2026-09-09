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
        // checks for an empty list
        if (node->list.count == 0) {
            printf("Warning: empty list\n");
            return value_nil();
        }

        Node *keyword = node->list.elements[0];

        if (keyword->type == N_SYMBOL) {
            if (strcmp(keyword->symbol, DEF_KEY) == 0) {
                if (node->list.count < 3) {
                    errx(1, "Error: %s needs 2 arguments.", DEF_KEY);
                }
                char *name = node->list.elements[1]->symbol;
                Value *value = eval(node->list.elements[2], env);
                set_variable(env, name, value);
                return value;
            }

            if (strcmp(keyword->symbol, FN_KEY) == 0) {
                if (node->list.count < 3) {
                    errx(1, "Error: %s needs 2 arguments.", FN_KEY);
                }
                Node *params = node->list.elements[1];
                Node *body = node->list.elements[2];
                return value_function(params, body, env);
            }

            if (strcmp(keyword->symbol, IF_KEY) == 0) {
                if (node->list.count < 3) {
                    errx(1, "Error: %s needs atleast 2 arguments.", IF_KEY);
                }
                Value *condition = eval(node->list.elements[1], env);
                int result = 0;
                if (condition->type == V_NUMBER && condition->number == 1) {
                    result = 1;
                }
                if (condition->type == V_STRING && strcmp(condition->string, "true") == 0) {
                    result = 1;
                }

                if (result == 1) {
                    return eval(node->list.elements[2], env);
                }
                else if (node->list.count > 3) {
                    return eval(node->list.elements[3], env);
                }
                return value_nil();
            }

            if (strcmp(keyword->symbol, EQUALS) == 0) {
                if (node->list.count < 3) {
                    errx(1, "Error: %s needs 2 arguments.", EQUALS);
                }
                Value *arg1 = eval(node->list.elements[1], env);
                Value *arg2 = eval(node->list.elements[2], env);

                if (arg1->type != arg2->type) {
                    errx(1, "Error: %s can't compare 2 different types.", EQUALS);
                }
                if (arg1->type == V_NUMBER) {
                    if (arg1->number == arg2->number) {
                        return value_number(1);
                    }
                }
                if (arg2->type == V_STRING) {
                    if (strcmp(arg1->string, arg2->string) == 0) {
                        return value_number(1);
                    }
                }
                return value_number(0);
            }

            if (strcmp(keyword->symbol, GREATER_THAN) == 0) {
                if (node->list.count < 3) {
                    errx(1, "Error: %s needs 2 arguments.", GREATER_THAN);
                }
                Value *arg1 = eval(node->list.elements[1], env);
                Value *arg2 = eval(node->list.elements[2], env);

                if (arg1->type != arg2->type) {
                    errx(1, "Error: %s can't compare 2 different types.", GREATER_THAN);
                }
                if (arg1->type == V_NUMBER) {
                    if (arg1->number > arg2->number) {
                        return value_number(1);
                    }
                }
                if (arg2->type == V_STRING) {
                    if (strcmp(arg1->string, arg2->string) > 0) {
                        return value_number(1);
                    }
                }
                return value_number(0);
            }

            if (strcmp(keyword->symbol, LESSER_THAN) == 0) {
                if (node->list.count < 3) {
                    errx(1, "Error: %s needs 2 arguments.", LESSER_THAN);
                }
                Value *arg1 = eval(node->list.elements[1], env);
                Value *arg2 = eval(node->list.elements[2], env);

                if (arg1->type != arg2->type) {
                    errx(1, "Error: %s can't compare 2 different types.", LESSER_THAN);
                }
                if (arg1->type == V_NUMBER) {
                    if (arg1->number < arg2->number) {
                        return value_number(1);
                    }
                }
                if (arg2->type == V_STRING) {
                    if (strcmp(arg1->string, arg2->string) < 0) {
                        return value_number(1);
                    }
                }
                return value_number(0);
            }

            if (strcmp(keyword->symbol, PLUS) == 0) {
                int n = 0;
                for (size_t i = 1; i < node->list.count; ++i) {
                    n += eval(node->list.elements[i], env)->number;
                }
                return value_number(n);
            }
            else if (strcmp(keyword->symbol, MINUS) == 0) {
                int n = 0;
                if (node->list.count < 2) {
                    return value_number(n);
                }
                n = eval(node->list.elements[1], env)->number;
                for (size_t i = 2; i < node->list.count; ++i) {
                    n -= eval(node->list.elements[i], env)->number;
                }
                return value_number(n);
            }
            else if (strcmp(keyword->symbol, MULT) == 0) {
                int n = 1;
                for (size_t i = 1; i < node->list.count; ++i) {
                    n *= eval(node->list.elements[i], env)->number;
                }
                return value_number(n);
            }
            else if (strcmp(keyword->symbol, DIVIDE) == 0) {
                int n = 0;
                if (node->list.count < 2) {
                    return value_number(n);
                }
                n = eval(node->list.elements[1], env)->number;
                for (size_t i = 2; i < node->list.count; ++i) {
                    n /= eval(node->list.elements[i], env)->number;
                }
                return value_number(n);
            }


            Value *function_to_eval = eval(keyword, env);
            if (function_to_eval->type == V_FUNCTION) {
                Function fn = function_to_eval->fn;

                Environment *fn_env = env_new(fn.env);

                for (size_t i = 0; i < fn.params->list.count; ++i) {
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

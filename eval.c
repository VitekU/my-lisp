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


Value *eval(Node *node) {
    if (node->type == N_NUMBER) {
        return value_number(node->number);
    }
    if (node->type == N_STRING) {
        return value_string(node->string);
    }

    if (node->type == N_LIST) {
        if (node->list.elements[0]->type == N_SYMBOL) {
            Node *op = node->list.elements[0];
            if (strcmp(op->symbol, "+") == 0) {
                int n = eval(node->list.elements[1])->number + eval(node->list.elements[2])->number;
                return value_number(n);
            }
            else if (strcmp(op->symbol, "-") == 0) {
                int n = eval(node->list.elements[1])->number - eval(node->list.elements[2])->number;
                return value_number(n);
            }
            else if (strcmp(op->symbol, "*") == 0) {
                int n = eval(node->list.elements[1])->number * eval(node->list.elements[2])->number;
                return value_number(n);
            }
            else if (strcmp(op->symbol, "/") == 0) {
                int n = eval(node->list.elements[1])->number / eval(node->list.elements[2])->number;
                return value_number(n);
            }
        }
    }

    return value_nil();
}

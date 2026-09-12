#pragma once
#include <stdio.h>
#include "eval.h"

/*
 * creates and validates the file pointer *fp
 * returns error upon fail
 */

FILE* create_file_ptr(const char *name);

void print_result(Value* value);

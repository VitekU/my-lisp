#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <err.h>

#include "helper.h"
#include "lexer.h"
#include "parser.h"
#include "eval.h"

#define REPL_BUFFER 8192
const char *NAMES[] = {"T_LPAREN", "T_RPAREN", "T_STRING", "T_NUMBER", "T_SYMBOL", "T_EOF"};


int main(int argc, char **argv) {
    char *fileName = argv[1];
    char buffer[REPL_BUFFER];

    // FILE input
    if (fileName != NULL) {
        FILE *fp = create_file_ptr(fileName);

        fseek(fp, 0, SEEK_END);
        long size = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        char *buffer = malloc(size + 1);

        if (!buffer) {
            fclose(fp);
            errx(1, "Memory allocation error.");
        }

        fread(buffer, 1, size, fp);
        buffer[size] = '\0';
        fclose(fp);

        Lexer lexer;
        init_lexer(&lexer, buffer);

        Token token;
        token = next_token(&lexer);
        while (token.type != T_EOF) {
            printf("%s, %s, %d, %d\n", token.value,  NAMES[token.type], token.line, token.column);
            token = next_token(&lexer);
        }
        return 0;
    }

    // REPL
    Lexer lexer;
    Parser parser;
    Environment *env = env_new(NULL);
    while (1) {
        printf(">>> ");
        if (fgets(buffer, REPL_BUFFER, stdin) == NULL) {
            break;
        }
        buffer[strcspn(buffer, "\n")] = '\0';
        init_lexer(&lexer, buffer);
        init_parser(&parser, &lexer);

        Node *root = parse_expression(&parser);

        while (root->type != N_NIL) {
            Value *result = eval(root, env);
            print_result(result);
            root = parse_expression(&parser);
        }
    }

    return 0;
}

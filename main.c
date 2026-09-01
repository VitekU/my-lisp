#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <err.h>

#include "helper.h"
#include "lexer.h"

int main(int argc, char **argv) {
    char *fileName = argv[1];
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
        printf("%s, %d, %d, %d\n", token.value, token.type, token.line, token.column);
        token = next_token(&lexer);
    }

    return 0;
}

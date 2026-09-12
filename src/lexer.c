#include "lexer.h"
#include <ctype.h>
#include <err.h>
#include <string.h>

// initialises the lexer structure
void init_lexer(Lexer* lexer, const char* source) {
    lexer->source = source;
    lexer->index = 0;
    lexer->line = 0;
    lexer->column = 0;
}

int is_symbol(char c) {
    if (c == '\0') {
        return 0;
    }
    if(isalnum((unsigned char)c)) {
        return 1;
    }
    if (strchr("+-*/=<>!", c) != NULL) {
        return 1;
    }
    return 0;

}

// returns the current character in the lexer stream
char curr_char(Lexer* lexer) {
    return lexer->source[lexer->index];
}

// returns the current chracter and advances the index in the lexer stream
char next_char(Lexer* lexer) {
    char curr_char = lexer->source[lexer->index++];
    if (curr_char == '\n') {
        lexer->line++;
        lexer->column = 1;
    }
    else {
        lexer->column++;
    }
    return curr_char;
}

// returns the next token
Token next_token(Lexer* lexer) {
    Token token;

    // skips the whitespaces
    while (1) {
        char c = curr_char(lexer);
        if (c == '\0') {
            break;
        }
        if (isspace((unsigned char)c)) {
            next_char(lexer);
        }
        else {
            break;
        }
    }

    char c = curr_char(lexer);

    token.line = lexer->line;
    token.column = lexer->column;

    // EOF token
    if (c == '\0') {
        token.type = T_EOF;
        return token;
    }

    // open parenthesis token
    if (c == '(') {
        next_char(lexer);
        strcpy(token.value, "(");
        token.type = T_LPAREN;
        return token;
    }

    // close parenthesis token
    if (c == ')') {
        next_char(lexer);
        strcpy(token.value, ")");
        token.type = T_RPAREN;
        return token;
    }

    // STRING token - checks for the opening quote
    if (c == '"') {
        next_char(lexer);
        int i = 0;

        // read the string literal until the closing quote or until it hits the buffer limit
        while (curr_char(lexer) != '"' && curr_char(lexer) != '\0') {
            if (i < TOKEN_SIZE - 1) {
                token.value[i++] = next_char(lexer);
            }
            else {
                next_char(lexer);
            }
        }

        if (curr_char(lexer) == '\0') {
            errx(1, "Unterminated string literal; line %d, column %d", token.line, token.column);
        }

        next_char(lexer);
        token.value[i] = '\0';
        token.type = T_STRING;
        return token;
    }

    // NUMBER token - checks for a number or a minus sign followed by a number
    if (isdigit((unsigned char)c) || (c == MINUS_SIGN && isdigit((unsigned char)lexer->source[lexer->index + 1]))) {
        int i = 0;
        token.value[i++] = next_char(lexer);
        while (isdigit(curr_char(lexer))) {
            if (i < TOKEN_SIZE - 1) {
                token.value[i++] = next_char(lexer);
            }
            else {
                break;
            }
        }

        token.value[i] = '\0';
        token.type = T_NUMBER;
        return token;
    }

    // SYMBOL token - checks for the lisp operators, keywords or variable names
    if (is_symbol(c)) {
        int i = 0;
        while (is_symbol(curr_char(lexer))) {
            if (i < TOKEN_SIZE - 1) {
                token.value[i++] = next_char(lexer);
            }
            else {
                break;
            }
        }

        token.value[i] = '\0';
        token.type = T_SYMBOL;
        return token;
    }

    errx(1, "Unrecognized character; line %d, column %d", token.line, token.column);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"


static int pos = 0;     //lexer position
Token current_token;    // current token

static Token token_gen(TokenType type, char* value){
    return (Token){type, value};
}

static char* copy_string(char* string, int length){
    char* new_string = malloc(length + 1);
    memcpy(new_string, string, length);
    new_string[length] = '\0';
    return new_string;
}
static void skip_space(char * source){
    while (source[pos] == ' ' || source[pos] == '\t' || source[pos] == '\n' || source[pos] == '\r'){
        pos++;
    }
}
Token lex(char* source){
    skip_space(source);
    if (source[pos] == '\0'){
        return token_gen(TOKEN_EOF, NULL);
    }
    if (source[pos] == 'p' && strncmp(source + pos + 1, "rint", 4) == 0){
        pos +=5;
        return token_gen(TOKEN_PRINT, "print");
    }
    if (source[pos] == '('){
        pos++;
        return token_gen(TOKEN_LPAREN, "(");
    }
    if (source[pos] == ')'){
        pos++;
        return token_gen(TOKEN_RPAREN, ")");
    }
    if (source[pos] == '"'){
        char* start = source + pos + 1;
        pos++;
        while(source[pos] != '"' && source[pos] != '\0'){
            pos++;
        }
        if (source[pos] == '\0'){
            return token_gen(TOKEN_ERROR, "unterminated string");
        }
        char* end = source + pos;
        pos++;
        return token_gen(TOKEN_STRING, copy_string(start, end - start));
    }
    return token_gen(TOKEN_ERROR, "unknown character");
}
void advance (char* source){
    current_token = lex(source);
}

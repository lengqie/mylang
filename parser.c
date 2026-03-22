#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "lexer.h"

static void expect(TokenType type, char* source){
    if (current_token.type != type){
        printf("parser error!");
        exit(1);
    }
    advance(source);
}

static void parse_print(char* source){
    expect(TOKEN_PRINT, source);
    expect(TOKEN_LPAREN, source);

    if(current_token.type != TOKEN_STRING){
        printf("syntax error!");
    }

    printf("%s\n",current_token.value);
    free(current_token.value);

    advance(source);
    expect(TOKEN_RPAREN, source);
}

void parser(char* source){
    advance(source);

    while (current_token.type != TOKEN_EOF){
        parse_print(source);
    }
}
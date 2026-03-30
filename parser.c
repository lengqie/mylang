#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "lexer.h"
#include "env.h"

static void syntax_error(const char* msg){
    printf("syntax error: %s, got '%s'\n", msg, current_token.value);
    exit(1);
}
static void expect(TokenType type, char* source){
    if (current_token.type == TOKEN_ERROR){
        syntax_error(current_token.value);
    }
    if (current_token.type != type){
        syntax_error("unexpected token");
    }
    advance(source);
}

static void parse_print(char* source){
    expect(TOKEN_PRINT, source);
    expect(TOKEN_LPAREN, source);
    const char* val = NULL;
    if(current_token.type == TOKEN_STRING){
        val = current_token.value;
    } else if (current_token.type == TOKEN_IDENT){
        val = env_get(current_token.value);
        if (!val){
            syntax_error("undefined identifier");
            exit(1);
        }
    } else {
        syntax_error("expected string or variable in print()");
        exit(1);
    }
    printf("%s\n",val);
    free(current_token.value);
    advance(source);
    expect(TOKEN_RPAREN, source);
}

static void parse_assign(char* source, char* name){
    expect(TOKEN_ASSIGN, source);
    if (current_token.type == TOKEN_STRING || 
        current_token.type == TOKEN_NUMBER){
        env_set(name, current_token.value);
        free(current_token.value);
        advance(source);
    } else {
        syntax_error("expected string or number after '='");
        exit(1);
    }
}
void parser(char* source){
    advance(source);

    while (current_token.type != TOKEN_EOF){
        if (current_token.type == TOKEN_PRINT){
            parse_print(source);
        } else if(current_token.type == TOKEN_IDENT){
            char* name = current_token.value;
            advance(source);
            parse_assign(source,name);
        } else {
            syntax_error("unexpected statement");
            exit(1);
        }
    }
    env_free();
}
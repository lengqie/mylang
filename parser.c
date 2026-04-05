#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "lexer.h"
#include "env.h"

static void syntax_error(const char* msg);
static void expect(TokenType type, char* source);
static int parse_primary(char* source);
static int parse_term(char* source);
static int parse_expr(char* source);
static void parse_print(char* source);
static void parse_assign(char* source, char* name);
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
static int parse_primary(char* source){
    if (current_token.type == TOKEN_NUMBER){
        int val = atoi(current_token.value);
        free(current_token.value);
        advance(source);
        return val;
    }
    if (current_token.type == TOKEN_IDENT){
        const Var* var = env_get(current_token.value);
        if (!var){
            syntax_error("undefined identifier");
        }
        if (var->type != VAR_INT){
            syntax_error("expected int variable in expression");
        }
        int val =  atoi(var->value);
        free(current_token.value);
        advance(source);
        return val;
    }
    if (current_token.type == TOKEN_LPAREN){
        advance(source);
        int val = parse_expr(source);
        expect(TOKEN_RPAREN, source);
        return val;
    }
    
    syntax_error("expected number, variable or '('");
    return 0;
    
}

static int parse_term(char* source){
    int left = parse_primary(source);

    while (current_token.type == TOKEN_STAR || current_token.type == TOKEN_SLASH){
        TokenType op = current_token.type;
        advance(source);
        int right = parse_primary(source);

        if (op == TOKEN_STAR){
            left *= right;
        } else {
            if (right == 0){
                syntax_error("division by zero");
                exit(1);
            }
            left /= right;
        }
        
    }
    return left;
}

static int parse_expr(char* source){
    int left = parse_term(source);
    
    while (current_token.type == TOKEN_PLUS || current_token.type == TOKEN_MINUS){
        TokenType op = current_token.type;
        advance(source);
        int right = parse_term(source);
        
        if (op == TOKEN_PLUS){
            left += right;
        } else {
            left -= right;
        }
    }
    return left;
}
static void parse_print(char* source){
    expect(TOKEN_PRINT, source);
    expect(TOKEN_LPAREN, source);
    if(current_token.type == TOKEN_STRING){
        printf("%s\n", current_token.value);
        free(current_token.value);
        advance(source);
    }  else if (current_token.type == TOKEN_IDENT){
        const Var* var = env_get(current_token.value);
        if(!var){
            syntax_error("undefined identifier");
        }
        if(var->type == VAR_STRING){
            printf("%s\n", var->value);
            free(current_token.value);
            advance(source);
        } else {
            int result = parse_expr(source);
            printf("%d\n", result);
        }
    } else {
        int result = parse_expr(source);
        printf("%d\n", result);
    }
    expect(TOKEN_RPAREN, source);
}

static void parse_assign(char* source, char* name){
    expect(TOKEN_ASSIGN, source);
    if (current_token.type == TOKEN_STRING){
        env_set(name, current_token.value, VAR_STRING);
        free(current_token.value);
        advance(source);
    } else if (current_token.type == TOKEN_NUMBER){
        env_set(name, current_token.value, VAR_INT);
        free(current_token.value);
        advance(source);
    } else {
        int result = parse_expr(source);
        char buffer[32];
        snprintf(buffer, sizeof(buffer), "%d", result);
        env_set(name, buffer,VAR_INT);
    }
}
void parser(char* source){
    lexer_reset();
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

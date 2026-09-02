#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "lexer.h"
#include "env.h"

static void syntax_error(const char* msg);
static void expect(TokenType type, char* source);
static int parse_primary(char* source);
static int parse_term(char* source);
static int parse_addsub(char* source);
static int parse_comparison(char* source);
static int parse_expr(char* source);
static void parse_print(char* source);
static void parse_assign(char* source, char* name);
static void parse_statement(char* source);
static void parse_block(char* source);
static void skip_block(char* source);
static void parse_if(char* source);
static void parse_while(char* source);
static void parse_for(char* source);

static void syntax_error(const char* msg){
    if (current_token.value != NULL){
        printf("syntax error: %s, got '%s'\n", msg, current_token.value);
    } else {
        printf("syntax error: %s\n", msg);
    }
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

// ---- expressions ----

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
        int val = atoi(var->value);
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
            }
            left /= right;
        }

    }
    return left;
}

static int parse_addsub(char* source){
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

static int parse_comparison(char* source){
    int left = parse_addsub(source);

    while (current_token.type == TOKEN_EQ || current_token.type == TOKEN_NEQ ||
           current_token.type == TOKEN_LT || current_token.type == TOKEN_GT ||
           current_token.type == TOKEN_LTE || current_token.type == TOKEN_GTE){
        TokenType op = current_token.type;
        advance(source);
        int right = parse_addsub(source);

        switch (op){
            case TOKEN_EQ:  left = (left == right); break;
            case TOKEN_NEQ: left = (left != right); break;
            case TOKEN_LT:  left = (left < right);  break;
            case TOKEN_GT:  left = (left > right);  break;
            case TOKEN_LTE: left = (left <= right); break;
            case TOKEN_GTE: left = (left >= right); break;
            default: break;
        }
    }
    return left;
}

static int parse_expr(char* source){
    return parse_comparison(source);
}

// ---- statements ----

static void parse_print(char* source){
    expect(TOKEN_PRINT, source);
    expect(TOKEN_LPAREN, source);
    if (current_token.type == TOKEN_STRING){
        printf("%s\n", current_token.value);
        free(current_token.value);
        advance(source);
    } else if (current_token.type == TOKEN_IDENT){
        const Var* var = env_get(current_token.value);
        if (!var){
            syntax_error("undefined identifier");
        }
        if (var->type == VAR_STRING){
            printf("%s\n", var->value);
            free(current_token.value);
            advance(source);
            expect(TOKEN_RPAREN, source);
            return;
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
    } else {
        int result = parse_expr(source);
        char buffer[32];
        snprintf(buffer, sizeof(buffer), "%d", result);
        env_set(name, buffer, VAR_INT);
    }
}

static void parse_block(char* source){
    expect(TOKEN_LBRACE, source);
    while (current_token.type != TOKEN_RBRACE && current_token.type != TOKEN_EOF){
        parse_statement(source);
    }
    expect(TOKEN_RBRACE, source);
}

static void skip_block(char* source){
    expect(TOKEN_LBRACE, source);
    int depth = 1;
    while (depth > 0){
        if (current_token.type == TOKEN_EOF){
            syntax_error("unexpected EOF in block");
        }
        if (current_token.type == TOKEN_LBRACE) depth++;
        else if (current_token.type == TOKEN_RBRACE) depth--;
        advance(source);
    }
}

static void parse_if(char* source){
    advance(source); // consume 'if'
    expect(TOKEN_LPAREN, source);
    int cond = parse_expr(source);
    expect(TOKEN_RPAREN, source);

    if (cond){
        // execute if-block
        parse_block(source);
        if (current_token.type == TOKEN_ELSE){
            advance(source); // consume 'else'
            skip_block(source);
        }
    } else {
        skip_block(source);
        if (current_token.type == TOKEN_ELSE){
            advance(source); // consume 'else'
            parse_block(source);
        }
    }
}

static void parse_while(char* source){
    advance(source); // consume 'while'
    if (current_token.type != TOKEN_LPAREN){
        syntax_error("expected '(' after while");
    }
    int save_pos = lexer_get_pos(); // point to start of condition

    for (;;){
        lexer_set_pos(save_pos);
        advance(source);

        int cond = parse_expr(source);
        expect(TOKEN_RPAREN, source);

        if (cond){
            parse_block(source);
        } else {
            skip_block(source);
            break;
        }
    }
}

static void parse_for(char* source){
    advance(source); // consume 'for'
    if (current_token.type != TOKEN_LPAREN){
        syntax_error("expected '(' after for");
    }
    advance(source); // read first token of init

    // init: optional assignment
    if (current_token.type == TOKEN_IDENT){
        char* name = current_token.value;
        advance(source);
        parse_assign(source, name);
    }
    if (current_token.type != TOKEN_SEMICOLON){
        syntax_error("expected ';' after init");
    }
    int cond_pos = lexer_get_pos(); // point to start of condition (after ';')
    advance(source); // read first token of condition

    for (;;){
        int cond = parse_expr(source);
        if (current_token.type != TOKEN_SEMICOLON){
            syntax_error("expected ';' after condition");
        }
        int incr_pos = lexer_get_pos(); // point to start of increment
        advance(source); // read first token of increment

        if (cond){
            // skip increment to ')', execute block
            while (current_token.type != TOKEN_RPAREN && current_token.type != TOKEN_EOF){
                advance(source);
            }
            expect(TOKEN_RPAREN, source);
            parse_block(source);
            // execute increment
            lexer_set_pos(incr_pos);
            advance(source);
            if (current_token.type == TOKEN_IDENT){
                char* name = current_token.value;
                advance(source);
                parse_assign(source, name);
            }
        } else {
            // skip increment, ')' and block
            while (current_token.type != TOKEN_RPAREN && current_token.type != TOKEN_EOF){
                advance(source);
            }
            expect(TOKEN_RPAREN, source);
            skip_block(source);
            break;
        }
        lexer_set_pos(cond_pos);
        advance(source);
    }
}

static void parse_statement(char* source){
    if (current_token.type == TOKEN_PRINT){
        parse_print(source);
    } else if (current_token.type == TOKEN_IDENT){
        char* name = current_token.value;
        advance(source);
        parse_assign(source, name);
    } else if (current_token.type == TOKEN_IF){
        parse_if(source);
    } else if (current_token.type == TOKEN_WHILE){
        parse_while(source);
    } else if (current_token.type == TOKEN_FOR){
        parse_for(source);
    } else {
        syntax_error("unexpected statement");
    }
}

void parser(char* source){
    lexer_reset();
    advance(source);

    while (current_token.type != TOKEN_EOF){
        parse_statement(source);
    }
    env_free();
}

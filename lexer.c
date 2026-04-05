#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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
    if (strncmp(source + pos, "print", 5) == 0 &&
        (source[pos + 5] == '(' || 
         source[pos + 5] == ' ' ||
         source[pos + 5] == '\t' ||
         source[pos + 5] == '\r') ){
        pos +=5;
        return token_gen(TOKEN_PRINT, "print");
    }
    if ((isalpha((unsigned char) source[pos])) || source[pos] =='_'){
        int start = pos;
        while ((isalnum((unsigned char) source[pos])) || source[pos] =='_'){
            pos++;
        }
        return token_gen(TOKEN_IDENT,copy_string(source + start, pos - start));
    }
    if (isdigit((unsigned char) source[pos])){
        int start = pos;
        while (isdigit((unsigned char) source[pos])){
            pos++;
        }
        return token_gen(TOKEN_NUMBER,copy_string(source + start, pos - start));
    }
    if (source[pos] == '"'){
        int start = pos + 1;
        pos++;
        while(source[pos] != '"' && source[pos] != '\0'){
            pos++;
        }
        if (source[pos] == '\0'){
            return token_gen(TOKEN_ERROR, "unterminated string");
        }
        pos++;
        return token_gen(TOKEN_STRING, copy_string(source + start, pos - start - 1));
    }
    switch (source[pos++]){
        case '=': return token_gen(TOKEN_ASSIGN, "=");
        case '(': return token_gen(TOKEN_LPAREN, "(");
        case ')': return token_gen(TOKEN_RPAREN, ")");
        case '+': return token_gen(TOKEN_PLUS,   "+");
        case '-': return token_gen(TOKEN_MINUS,  "-");
        case '*': return token_gen(TOKEN_STAR,   "*");
        case '/': return token_gen(TOKEN_SLASH,  "/");
        default:  return token_gen(TOKEN_ERROR,  "unknown character");
    }
}
void advance (char* source){
    current_token = lex(source);
}

void lexer_reset(void){
    pos = 0;
}

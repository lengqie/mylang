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
    for(;;){
        while (source[pos] == ' ' || source[pos] == '\t' || source[pos] == '\n' || source[pos] == '\r'){
            pos++;
        }
        // single-line comment: // ...
        if (source[pos] == '/' && source[pos + 1] == '/'){
            while (source[pos] != '\n' && source[pos] != '\0'){
                pos++;
            }
            continue;
        }
        break;
    }
}
static Token keyword(TokenType type, const char* word, int len){
    pos += len;
    return token_gen(type, (char*)word);
}
Token lex(char* source){
    skip_space(source);
    if (source[pos] == '\0'){
        return token_gen(TOKEN_EOF, NULL);
    }
    // keywords
    if (strncmp(source + pos, "print", 5) == 0 &&
        (source[pos + 5] == '(' ||
         source[pos + 5] == ' ' ||
         source[pos + 5] == '\t' ||
         source[pos + 5] == '\r' ||
         source[pos + 5] == '\n') ){
        return keyword(TOKEN_PRINT, "print", 5);
    }
    if (strncmp(source + pos, "if", 2) == 0 &&
        !isalnum((unsigned char)source[pos + 2]) && source[pos + 2] != '_'){
        return keyword(TOKEN_IF, "if", 2);
    }
    if (strncmp(source + pos, "else", 4) == 0 &&
        !isalnum((unsigned char)source[pos + 4]) && source[pos + 4] != '_'){
        return keyword(TOKEN_ELSE, "else", 4);
    }
    if (strncmp(source + pos, "while", 5) == 0 &&
        !isalnum((unsigned char)source[pos + 5]) && source[pos + 5] != '_'){
        return keyword(TOKEN_WHILE, "while", 5);
    }
    if (strncmp(source + pos, "for", 3) == 0 &&
        !isalnum((unsigned char)source[pos + 3]) && source[pos + 3] != '_'){
        return keyword(TOKEN_FOR, "for", 3);
    }
    if ((isalpha((unsigned char) source[pos])) || source[pos] =='_'){
        int start = pos;
        while ((isalnum((unsigned char) source[pos])) || source[pos] =='_'){
            pos++;
        }
        return token_gen(TOKEN_IDENT, copy_string(source + start, pos - start));
    }
    if (isdigit((unsigned char) source[pos])){
        int start = pos;
        while (isdigit((unsigned char) source[pos])){
            pos++;
        }
        return token_gen(TOKEN_NUMBER, copy_string(source + start, pos - start));
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
    // two-character operators
    if (source[pos] == '=' && source[pos + 1] == '='){
        pos += 2;
        return token_gen(TOKEN_EQ, "==");
    }
    if (source[pos] == '!' && source[pos + 1] == '='){
        pos += 2;
        return token_gen(TOKEN_NEQ, "!=");
    }
    if (source[pos] == '<' && source[pos + 1] == '='){
        pos += 2;
        return token_gen(TOKEN_LTE, "<=");
    }
    if (source[pos] == '>' && source[pos + 1] == '='){
        pos += 2;
        return token_gen(TOKEN_GTE, ">=");
    }
    switch (source[pos++]){
        case '=': return token_gen(TOKEN_ASSIGN, "=");
        case '(': return token_gen(TOKEN_LPAREN, "(");
        case ')': return token_gen(TOKEN_RPAREN, ")");
        case '+': return token_gen(TOKEN_PLUS,   "+");
        case '-': return token_gen(TOKEN_MINUS,  "-");
        case '*': return token_gen(TOKEN_STAR,   "*");
        case '/': return token_gen(TOKEN_SLASH,  "/");
        case '<': return token_gen(TOKEN_LT,     "<");
        case '>': return token_gen(TOKEN_GT,     ">");
        case ';': return token_gen(TOKEN_SEMICOLON, ";");
        case '{': return token_gen(TOKEN_LBRACE, "{");
        case '}': return token_gen(TOKEN_RBRACE, "}");
        default:  return token_gen(TOKEN_ERROR,  "unknown character");
    }
}
void advance (char* source){
    current_token = lex(source);
}

void lexer_reset(void){
    pos = 0;
}
int lexer_get_pos(void){
    return pos;
}
void lexer_set_pos(int p){
    pos = p;
}

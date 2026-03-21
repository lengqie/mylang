#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum{
    TOKEN_EOF,      // \0
    TOKEN_LPAREN,   // (
    TOKEN_RPAREN,   // )
    TOKEN_STRING,   // string type
    TOKEN_PRINT,    // print()
    TOKEN_ERROR,    // error
} TokenType;

typedef struct {
    TokenType type;
    char* value;
} Token;
static char* read_source(char* filename){
    FILE* f = fopen(filename, "rb");
    if(f == NULL){
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* buffer = malloc(size + 1);
    fread(buffer, 1, size, f);
    buffer[size] = '\0';
    fclose(f);
    return buffer;

}
static int pos = 0;     //lexer position

static Token token_gen(TokenType type, char* value){
    return (Token){type, value};
}

static char* copy_string(char* string, int length){
    char* new_string = malloc(length + 1);
    memcpy(new_string, string, length);
    new_string[length] = '\0';
    return new_string;
}
static char* skip_space(char * source){
    while (source[pos] == ' ' && source[pos] == '\t' && source[pos] == '\n'){
        pos++;
    }
}
static Token lex(char* source){
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
int main(int argc, char *argv[])
{
    if(argc > 1){
        char* source = read_source(argv[1]);
        // printf("%s", source);
        lex(source);
    } else {
        printf("Hello World");
    }
    return 0;
}
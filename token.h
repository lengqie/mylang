#ifndef TOKEN_H
#define TOKEN_H

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

#endif
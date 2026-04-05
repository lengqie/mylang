#ifndef TOKEN_H
#define TOKEN_H

typedef enum{
    TOKEN_EOF,      // \0
    TOKEN_LPAREN,   // (
    TOKEN_RPAREN,   // )
    TOKEN_STRING,   // string type
    TOKEN_PRINT,    // print()
    TOKEN_ERROR,    // error
    TOKEN_IDENT,    // identifier
    TOKEN_NUMBER,    // number
    TOKEN_ASSIGN,   // =
    TOKEN_PLUS,     // +
    TOKEN_MINUS,    // -
    TOKEN_STAR,     // *
    TOKEN_SLASH,    // /
} TokenType;

typedef struct {
    TokenType type;
    char* value;
} Token;

#endif
#ifndef LEXER_H
#define LEXER_H

#include "token.h"

extern Token current_token;

void advance(char* source);
Token lex(char* source);

#endif
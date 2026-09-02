#ifndef LEXER_H
#define LEXER_H

#include "token.h"

extern Token current_token;

void advance(char* source);
Token lex(char* source);
void lexer_reset(void);
int lexer_get_pos(void);
void lexer_set_pos(int p);

#endif

#ifndef MATHMANIA_LEXER_H_
#define MATHMANIA_LEXER_H_

#include "mathmania/tokenarray.h"
#include <stddef.h>

TokenArray Lex(const char *source, size_t length);

#endif

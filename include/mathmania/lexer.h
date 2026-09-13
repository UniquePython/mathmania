#ifndef MATHMANIA_LEXER_H_
#define MATHMANIA_LEXER_H_

#include "mathmania/tokenarray.h"
#include "mathmania/source.h"
#include <stdbool.h>

bool Lex(Source source, TokenArray *tokArr);

#endif

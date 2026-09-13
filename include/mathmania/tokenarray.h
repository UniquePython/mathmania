#ifndef MATHMANIA_TOKENARRAY_H_
#define MATHMANIA_TOKENARRAY_H_

#include <stddef.h>
#include "mathmania/token.h"
#include <stdbool.h>

typedef struct
{
    Token *tokens;
    size_t count;
    size_t capacity;

} TokenArray;

bool TokenArrayInit(TokenArray *tokArr);
void TokenArrayFree(TokenArray *tokArr);

bool TokenArrayPush(TokenArray *tokArr, Token token);

#endif

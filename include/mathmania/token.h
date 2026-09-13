#ifndef MATHMANIA_TOKEN_H_
#define MATHMANIA_TOKEN_H_

#include "mathmania/span.h"
#include "mathmania/tokenkind.h"

typedef struct
{
    TokenKind kind;
    Span span;

} Token;

#endif

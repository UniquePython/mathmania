#ifndef MATHMANIA_EXPR_H_
#define MATHMANIA_EXPR_H_

#include "mathmania/span.h"
#include "mathmania/exprkind.h"

typedef struct
{
    ExprKind kind;

    union
    {
        struct
        {
            Span span;
        } natural;

    } as;

} Expr;

#endif

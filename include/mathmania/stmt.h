#ifndef MATHMANIA_STMT_H_
#define MATHMANIA_STMT_H_

#include "mathmania/span.h"
#include "mathmania/stmtkind.h"
#include "mathmania/expr.h"

typedef struct
{
    StmtKind kind;

    union
    {
        struct
        {
            Expr *expr;
            Span span;
        } show;

    } as;

} Stmt;

#endif

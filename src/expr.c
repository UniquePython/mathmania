#include "mathmania/expr.h"
#include "mathmania/exprkind.h"
#include "mathmania/memory.h"

void ExprFree(Expr **expr)
{
    if (expr == NULL || *expr == NULL)
        return;

    switch ((*expr)->kind)
    {
    case EK_NATURAL_LITERAL:
        break;

    case EK_COUNT:
        break;
    }

    Release(expr);
}

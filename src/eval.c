#include "mathmania/eval.h"
#include "mathmania/natural.h"
#include "mathmania/expr.h"
#include "mathmania/exprkind.h"
#include "mathmania/stmt.h"
#include "mathmania/stmtkind.h"
#include "mathmania/error.h"

static bool EvalExpr(Source source, Expr *expr, Natural *out)
{
    switch (expr->kind)
    {
    case EK_NATURAL_LITERAL:
    {
        Span span = expr->as.natural.span;
        size_t length = span.end - span.start;

        return NaturalFromStr(source.code + span.start, length, out);
    }

    case EK_COUNT:
        break;
    }

    Error(source, expr->as.natural.span, "Internal error: unhandled expression kind");
    return false; // unreachable
}

bool Eval(Source source, Program *program)
{
    for (size_t i = 0; i < program->count; i++)
    {
        Stmt stmt = program->stmts[i];

        switch (stmt.kind)
        {
        case SK_SHOW:
        {
            Natural value;

            if (!EvalExpr(source, stmt.as.show.expr, &value))
                return false;

            NaturalShow(value);
            NaturalFree(&value);
            break;
        }

        case SK_COUNT:
            break;
        }
    }

    return true;
}

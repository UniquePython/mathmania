#include "mathmania/parser.h"
#include "mathmania/memory.h"
#include "mathmania/error.h"
#include <stddef.h>

typedef struct
{
    Source source;
    const TokenArray *tokArr;
    size_t position;

} Parser;

static inline bool atEnd(Parser *parser)
{
    return parser->tokArr->tokens[parser->position].kind == TK_EOF;
}

static inline Token peek(Parser *parser)
{
    return parser->tokArr->tokens[parser->position];
}

static inline Token advance(Parser *parser)
{
    Token tok = peek(parser);

    if (!atEnd(parser))
        parser->position++;

    return tok;
}

static Token expect(Parser *parser, TokenKind kind, const char *msg)
{
    Token tok = peek(parser);

    if (tok.kind != kind)
    {
        Error(parser->source, tok.span, "%s", msg);
        // unreachable
    }

    return advance(parser);
}

static bool parseExpr(Parser *parser, Expr **out)
{
    Token tok = expect(parser, TK_NATURAL, "Expected a Natural literal");

    Expr *expr;

    if (!Allocate(sizeof(Expr), &expr))
        return false;

    expr->kind = EK_NATURAL_LITERAL;
    expr->as.natural.span = tok.span;

    *out = expr;
    return true;
}

static bool parseStmt(Parser *parser, Stmt *out)
{
    Token showTok = expect(parser, TK_SHOW, "Expected 'show'");

    Expr *expr;

    if (!parseExpr(parser, &expr))
        return false;

    out->kind = SK_SHOW;
    out->as.show.expr = expr;
    out->as.show.span = (Span){showTok.span.start, expr->as.natural.span.end};

    return true;
}

bool Parse(Source source, const TokenArray *tokens, Program *out)
{
    Parser parser = {source, tokens, 0};

    while (!atEnd(&parser))
    {
        Stmt stmt;

        if (!parseStmt(&parser, &stmt))
            return false;

        if (!ProgramPush(out, stmt))
            return false;
    }

    return true;
}

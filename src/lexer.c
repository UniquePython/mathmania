#include "mathmania/lexer.h"
#include "mathmania/error.h"
#include <string.h>

typedef struct
{
    Source source;
    size_t position;

} Lexer;

static inline bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}

static inline bool isIdentifierStart(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static inline bool isIdentifierPart(char c)
{
    return isIdentifierStart(c) || isDigit(c);
}

static inline bool isWhitespace(char c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

static inline bool atEnd(Lexer *lexer)
{
    return lexer->position >= lexer->source.length;
}

static inline char peek(Lexer *lexer)
{
    return lexer->source.code[lexer->position];
}

static void skipWhitespace(Lexer *lexer)
{
    while (!atEnd(lexer) && isWhitespace(peek(lexer)))
        lexer->position++;
}

static void skipComment(Lexer *lexer)
{
    lexer->position += 2;

    while (!atEnd(lexer) && peek(lexer) != '\n')
        lexer->position++;
}

static void skipIgnored(Lexer *lexer)
{
    for (;;)
    {
        skipWhitespace(lexer);

        if (lexer->position + 1 >= lexer->source.length || peek(lexer) != '/' || lexer->source.code[lexer->position + 1] != '/')
            return;

        skipComment(lexer);
    }
}

static bool lexNumber(Lexer *lexer, TokenArray *tokArr)
{
    size_t start = lexer->position;

    while (!atEnd(lexer) && isDigit(peek(lexer)))
        lexer->position++;

    if (!TokenArrayPush(tokArr, (Token){TK_NATURAL, {start, lexer->position}}))
        return false;

    return true;
}

typedef struct
{
    const char *text;
    size_t length;
    TokenKind kind;

} Keyword;

static const Keyword keywords[] = {
    {"show", 4, TK_SHOW},
};

static TokenKind keywordKind(const char *text, size_t length)
{
    for (size_t i = 0; i < sizeof(keywords) / sizeof(*keywords); i++)
    {
        if (keywords[i].length == length && memcmp(keywords[i].text, text, length) == 0)
            return keywords[i].kind;
    }

    return TK_COUNT;
}

static bool lexIdentifier(Lexer *lexer, TokenArray *tokArr)
{
    size_t start = lexer->position;

    lexer->position++;

    while (!atEnd(lexer) && isIdentifierPart(peek(lexer)))
        lexer->position++;

    Span span = {start, lexer->position};
    size_t length = span.end - span.start;
    TokenKind kind = keywordKind(lexer->source.code + start, length);

    if (kind == TK_COUNT)
    {
        Error(lexer->source, span, "Unknown identifier: '%.*s'", (int)length, lexer->source.code + start);
        return false; // unreachable
    }

    if (!TokenArrayPush(tokArr, (Token){kind, span}))
        return false;

    return true;
}

bool Lex(Source source, TokenArray *tokArr)
{
    if (tokArr == NULL)
        return false;

    Lexer lexer = {source, 0};

    while (!atEnd(&lexer))
    {
        skipIgnored(&lexer);

        if (atEnd(&lexer))
            break;

        char c = peek(&lexer);

        if (isDigit(c))
        {
            if (!lexNumber(&lexer, tokArr))
                return false;
        }

        else if (isIdentifierStart(c))
        {
            if (!lexIdentifier(&lexer, tokArr))
                return false;
        }

        else
        {
            Error(lexer.source, (Span){lexer.position, lexer.position + 1}, "Illegal character: '%c'", c);
            return false; // Unreachable
        }
    }

    if (!TokenArrayPush(tokArr, (Token){TK_EOF, {lexer.source.length, lexer.source.length}}))
        return false;

    return true;
}

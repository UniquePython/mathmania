#include "mathmania/lexer.h"
#include "mathmania/error.h"
#include <string.h>

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

static void skipWhitespace(Source source, size_t *position)
{
    while (*position < source.length)
    {
        char c = source.code[*position];

        if (isWhitespace(c))
            break;

        (*position)++;
    }
}

static void skipComment(Source source, size_t *position)
{
    *position += 2;

    while (*position < source.length && source.code[*position] != '\n')
        (*position)++;
}

static void skipIgnored(Source source, size_t *position)
{
    for (;;)
    {
        skipWhitespace(source, position);

        if (*position + 1 >= source.length || source.code[*position] != '/' || source.code[*position + 1] != '/')
            return;

        skipComment(source, position);
    }
}

static bool lexNumber(Source source, size_t *position, TokenArray *tokArr)
{
    size_t start = *position;

    while (*position < source.length && isDigit(source.code[*position]))
        (*position)++;

    if (!TokenArrayPush(tokArr, (Token){TK_NATURAL, {start, *position}}))
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

static bool lexIdentifier(Source source, size_t *position, TokenArray *tokArr)
{
    size_t start = *position;

    (*position)++;

    while (*position < source.length && isIdentifierPart(source.code[*position]))
        (*position)++;

    Span span = {start, *position};
    size_t length = span.end - span.start;
    TokenKind kind = keywordKind(source.code + start, length);

    if (kind == TK_COUNT)
    {
        Error(source, span, "Unknown identifier: '%.*s'", (int)length, source.code + start);
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

    size_t position = 0;

    while (position < source.length)
    {
        skipIgnored(source, &position);

        if (position >= source.length)
            break;

        char c = source.code[position];

        if (isDigit(c))
        {
            if (!lexNumber(source, &position, tokArr))
                return false;
        }

        else if (isIdentifierStart(c))
        {
            if (!lexIdentifier(source, &position, tokArr))
                return false;
        }

        else
        {
            Error(source, (Span){position, position + 1}, "Illegal character: '%c'", c);
            return false; // Unreachable
        }
    }

    if (!TokenArrayPush(tokArr, (Token){TK_EOF, {source.length, source.length}}))
        return false;

    return true;
}

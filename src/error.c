#include "mathmania/error.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

static size_t lineNumber(Source source, size_t position)
{
    size_t line = 1;

    for (size_t i = 0; i < position; i++)
        if (source.code[i] == '\n')
            line++;

    return line;
}

static size_t lineStart(Source source, size_t position)
{
    for (size_t i = position; i > 0; --i)
        if (source.code[i - 1] == '\n')
            return i;

    return 0;
}

static size_t lineEnd(Source source, size_t position)
{
    for (size_t i = position; i < source.length; ++i)
        if (source.code[i] == '\n')
            return i;

    return source.length;
}

static size_t decimalDigits(size_t value)
{
    size_t digits = 1;

    while (value >= 10)
    {
        value /= 10;
        digits++;
    }

    return digits;
}

static size_t lineCount(Source source)
{
    size_t lines = 1;

    for (size_t i = 0; i < source.length; i++)
        if (source.code[i] == '\n')
            lines++;

    return lines;
}

void Error(Source source, Span offender, const char *fmt, ...)
{
    size_t line = lineNumber(source, offender.start);
    size_t start = lineStart(source, offender.start);
    size_t end = lineEnd(source, offender.start);

    size_t column = offender.start - start;
    size_t width = decimalDigits(lineCount(source));

    fprintf(stderr, "%s:%zu:%zu: error: ", source.path, line, column + 1);

    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    fputc('\n', stderr);
    fprintf(stderr, "%*zu | %.*s\n", (int)width, line, (int)(end - start), source.code + start);
    fprintf(stderr, "%*s | ", (int)width, "");

    for (size_t i = 0; i < column; i++)
        fputc(' ', stderr);

    size_t underlineLength = offender.end - offender.start;

    if (underlineLength == 0)
        underlineLength = 1;

    for (size_t i = 0; i < underlineLength; i++)
        fputc('^', stderr);

    fputc('\n', stderr);

    exit(1);
}

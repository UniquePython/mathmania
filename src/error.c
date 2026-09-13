#include "mathmania/error.h"
#include "mathmania/memory.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

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

static bool formatMessage(char **out, const char *fmt, va_list args)
{
    if (out == NULL || fmt == NULL)
        return false;

    va_list argsCopy;
    va_copy(argsCopy, args);
    int needed = vsnprintf(NULL, 0, fmt, argsCopy);
    va_end(argsCopy);

    if (needed < 0)
        return false;

    size_t size = (size_t)needed + 1;
    char *buffer;

    if (!Allocate(size, &buffer))
        return false;

    vsnprintf(buffer, size, fmt, args);

    *out = buffer;
    return true;
}

bool MathmaniaErrorNew(Source source, Span span, MathmaniaError *out, const char *fmt, ...)
{
    if (out == NULL || fmt == NULL)
        return false;

    char *message;

    va_list args;
    va_start(args, fmt);
    bool ok = formatMessage(&message, fmt, args);
    va_end(args);

    if (!ok)
        return false;

    out->hasSpan = true;
    out->source = source;
    out->span = span;
    out->message = message;

    return true;
}

bool MathmaniaErrorSimple(MathmaniaError *out, const char *fmt, ...)
{
    if (out == NULL || fmt == NULL)
        return false;

    char *message;

    va_list args;
    va_start(args, fmt);
    bool ok = formatMessage(&message, fmt, args);
    va_end(args);

    if (!ok)
        return false;

    out->hasSpan = false;
    out->message = message;

    return true;
}

bool MathmaniaErrorFormat(MathmaniaError error, char **out)
{
    if (out == NULL || error.message == NULL)
        return false;

    if (!error.hasSpan)
    {
        int needed = snprintf(NULL, 0, "error: %s\n", error.message);

        if (needed < 0)
            return false;

        size_t size = (size_t)needed + 1;
        char *buffer;

        if (!Allocate(size, &buffer))
            return false;

        snprintf(buffer, size, "error: %s\n", error.message);

        *out = buffer;
        return true;
    }

    Source source = error.source;
    Span span = error.span;

    size_t line = lineNumber(source, span.start);
    size_t start = lineStart(source, span.start);
    size_t end = lineEnd(source, span.start);

    size_t column = span.start - start;
    size_t width = decimalDigits(lineCount(source));

    size_t underlineLength = span.end - span.start;

    if (underlineLength == 0)
        underlineLength = 1;

    int headerNeeded = snprintf(
        NULL, 0,
        "%s:%zu:%zu: error: %s\n%*zu | %.*s\n%*s | ",
        source.path, line, column + 1, error.message,
        (int)width, line, (int)(end - start), source.code + start,
        (int)width, "");

    if (headerNeeded < 0)
        return false;

    size_t headerSize = (size_t)headerNeeded;
    size_t trailerSize = column + underlineLength + 1; // spaces + carets + newline
    size_t totalSize = headerSize + trailerSize + 1;   // + nul terminator

    char *buffer;

    if (!Allocate(totalSize, &buffer))
        return false;

    int written = snprintf(
        buffer, headerSize + 1,
        "%s:%zu:%zu: error: %s\n%*zu | %.*s\n%*s | ",
        source.path, line, column + 1, error.message,
        (int)width, line, (int)(end - start), source.code + start,
        (int)width, "");

    if (written < 0 || (size_t)written != headerSize)
    {
        Release(&buffer);
        return false;
    }

    size_t pos = headerSize;

    for (size_t i = 0; i < column; i++)
        buffer[pos++] = ' ';

    for (size_t i = 0; i < underlineLength; i++)
        buffer[pos++] = '^';

    buffer[pos++] = '\n';
    buffer[pos] = '\0';

    *out = buffer;
    return true;
}

void MathmaniaErrorFree(MathmaniaError *error)
{
    if (error == NULL)
        return;

    Release(&error->message);
}

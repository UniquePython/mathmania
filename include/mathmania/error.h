#ifndef MATHMANIA_ERROR_H_
#define MATHMANIA_ERROR_H_

#include "mathmania/source.h"
#include "mathmania/token.h"
#include <stdbool.h>

typedef struct
{
    bool hasSpan;
    Source source;
    Span span;
    char *message;

} MathmaniaError;

bool MathmaniaErrorNew(Source source, Span span, MathmaniaError *out, const char *fmt, ...);
bool MathmaniaErrorSimple(MathmaniaError *out, const char *fmt, ...);

bool MathmaniaErrorFormat(MathmaniaError error, char **out);

void MathmaniaErrorFree(MathmaniaError *error);

#endif

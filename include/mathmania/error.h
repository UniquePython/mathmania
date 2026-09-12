#ifndef MATHMANIA_ERROR_H_
#define MATHMANIA_ERROR_H_

#include "mathmania/source.h"
#include "mathmania/token.h"

void Error(Source source, Span offender, const char *fmt, ...);

#endif

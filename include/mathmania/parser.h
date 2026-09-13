#ifndef MATHMANIA_PARSER_H_
#define MATHMANIA_PARSER_H_

#include "mathmania/source.h"
#include "mathmania/tokenarray.h"
#include "mathmania/program.h"
#include <stdbool.h>

bool Parse(Source source, const TokenArray *tokArr, Program *out);

#endif

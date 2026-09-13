#ifndef MATHMANIA_PROGRAM_H_
#define MATHMANIA_PROGRAM_H_

#include <stddef.h>
#include "mathmania/stmt.h"
#include <stdbool.h>

typedef struct
{
    Stmt *stmts;
    size_t count;
    size_t capacity;

} Program;

bool ProgramInit(Program *prog);
void ProgramFree(Program *prog);

bool ProgramPush(Program *prog, Stmt stmt);

#endif

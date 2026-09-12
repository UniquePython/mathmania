#include "mathmania/program.h"
#include "mathmania/memory.h"

bool ProgramInit(Program *prog)
{
    if (prog == NULL)
        return false;

    prog->stmts = NULL;
    prog->count = 0;
    prog->capacity = 0;

    return true;
}

void ProgramFree(Program *prog)
{
    if (prog == NULL)
        return;

    Release(&prog->stmts);
    prog->count = 0;
    prog->capacity = 0;

    return;
}

static bool stmtArrayGrow(Program *prog)
{
    if (prog == NULL)
        return false;

    size_t newCapacity = prog->capacity == 0 ? 8 : prog->capacity * 2;
    if (newCapacity < prog->capacity)
        return false;

    size_t size = newCapacity * sizeof(*prog->stmts);
    if (size < newCapacity)
        return false;

    if (!Reallocate(&prog->stmts, size))
        return false;

    prog->capacity = newCapacity;

    return true;
}

bool ProgramPush(Program *prog, Stmt stmt)
{
    if (prog == NULL)
        return false;

    if (prog->count == prog->capacity)
        if (!stmtArrayGrow(prog))
            return false;

    prog->stmts[prog->count++] = stmt;

    return true;
}

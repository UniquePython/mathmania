#include "mathmania/tokenarray.h"
#include "mathmania/memory.h"

bool TokenArrayInit(TokenArray *tokArr)
{
    if (tokArr == NULL)
        return false;

    tokArr->tokens = NULL;
    tokArr->count = 0;
    tokArr->capacity = 0;

    return true;
}

void TokenArrayFree(TokenArray *tokArr)
{
    if (tokArr == NULL)
        return;

    Release(&tokArr->tokens);
    tokArr->count = 0;
    tokArr->capacity = 0;

    return;
}

static bool tokenArrayGrow(TokenArray *tokArr)
{
    if (tokArr == NULL)
        return false;

    size_t newCapacity = tokArr->capacity == 0 ? 8 : tokArr->capacity * 2;
    if (newCapacity < tokArr->capacity)
        return false;

    size_t size = newCapacity * sizeof(*tokArr->tokens);
    if (size < newCapacity)
        return false;

    if (!Reallocate(&tokArr->tokens, size))
        return false;

    tokArr->capacity = newCapacity;

    return true;
}

bool TokenArrayPush(TokenArray *tokArr, Token token)
{
    if (tokArr == NULL)
        return false;

    if (tokArr->count == tokArr->capacity)
        if (!tokenArrayGrow(tokArr))
            return false;

    tokArr->tokens[tokArr->count++] = token;

    return true;
}

#include "mathmania/memory.h"
#include <stdlib.h>

bool allocate(size_t size, void **out)
{
    if (size == 0 || out == NULL)
        return false;

    void *ptr = malloc(size);
    if (ptr == NULL)
        return false;

    *out = ptr;
    return true;
}

bool reallocate(void **ptr, size_t size)
{
    if (ptr == NULL || size == 0)
        return false;

    void *temp = realloc(*ptr, size);
    if (temp == NULL)
        return false;

    *ptr = temp;
    return true;
}

void release(void **ptr)
{
    if (ptr == NULL || *ptr == NULL)
        return;

    free(*ptr);
    *ptr = NULL;

    return;
}

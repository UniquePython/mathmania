#ifndef MATHMANIA_MEMORY_H_
#define MATHMANIA_MEMORY_H_

#include <stdbool.h>
#include <stddef.h>

bool allocate(size_t size, void **out);
bool reallocate(void **ptr, size_t size);
void release(void **ptr);

#define Allocate(size, out) allocate((size), (void **)(out))
#define Reallocate(ptr, size) reallocate((void **)(ptr), (size))
#define Release(ptr) release((void **)(ptr))

#endif

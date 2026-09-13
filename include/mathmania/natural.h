#ifndef MATHMANIA_NATURAL_H_
#define MATHMANIA_NATURAL_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct
{
    uint32_t *limbs;
    size_t length;
} Natural;

bool NaturalFromStr(const char *digits, size_t length, Natural *out);
void NaturalFree(Natural *natural);
void NaturalShow(Natural natural);

#endif

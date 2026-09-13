#ifndef MATHMANIA_VERSION_H_
#define MATHMANIA_VERSION_H_

#include <stddef.h>
#include <stdbool.h>

typedef struct
{
    size_t breaking;
    size_t feature;
    size_t internal;
    size_t patch;

} MathmaniaVersion;

// clang-format off
#define MM_VERSION(b, f, i, p) (MathmaniaVersion) { (b), (f), (i), (p) }
// clang-format on

#define MM_VERSION_FMT "%zu.%zu.%zu.%zu"
#define MM_VERSION_ARG(v) (v).breaking, (v).feature, (v).internal, (v).patch

extern const MathmaniaVersion currentMathmaniaVersion;

int MathmaniaVersionCompare(MathmaniaVersion a, MathmaniaVersion b);
bool MathmaniaVersionIsCompatible(MathmaniaVersion required, MathmaniaVersion actual);

#endif

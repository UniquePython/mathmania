#include "mathmania/version.h"

const MathmaniaVersion currentMathmaniaVersion = {
    .breaking = 0,
    .feature = 1,
    .internal = 0,
    .patch = 0,
};

int MathmaniaVersionCompare(MathmaniaVersion a, MathmaniaVersion b)
{
    if (a.breaking != b.breaking)
        return a.breaking < b.breaking ? -1 : 1;

    if (a.feature != b.feature)
        return a.feature < b.feature ? -1 : 1;

    if (a.internal != b.internal)
        return a.internal < b.internal ? -1 : 1;

    if (a.patch != b.patch)
        return a.patch < b.patch ? -1 : 1;

    return 0;
}

bool MathmaniaVersionIsCompatible(MathmaniaVersion required, MathmaniaVersion actual)
{
    if (actual.breaking != required.breaking)
        return false;

    if (actual.feature < required.feature)
        return false;

    return true;
}

#include "mathmania/natural.h"
#include "mathmania/memory.h"

#include <stdio.h>
#include <string.h>

static bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}

static bool mulSmallAddSmall(uint32_t **limbs, size_t *length, uint32_t factor, uint32_t addend)
{
    uint64_t carry = addend;
    for (size_t i = 0; i < *length; i++)
    {
        uint64_t product = (uint64_t)(*limbs)[i] * (uint64_t)factor + carry;
        (*limbs)[i] = (uint32_t)(product & 0xFFFFFFFFu);
        carry = product >> 32;
    }

    if (carry > 0)
    {
        if (!Reallocate(limbs, (*length + 1) * sizeof(uint32_t)))
            return false;

        (*limbs)[*length] = (uint32_t)carry;
        (*length)++;
    }

    return true;
}

static uint32_t divSmallInPlace(uint32_t *limbs, size_t length, uint32_t divisor)
{
    uint64_t remainder = 0;

    for (size_t i = length; i > 0; i--)
    {
        uint64_t current = (remainder << 32) | (uint64_t)limbs[i - 1];
        limbs[i - 1] = (uint32_t)(current / divisor);
        remainder = current % divisor;
    }

    return (uint32_t)remainder;
}

static size_t effectiveLength(const uint32_t *limbs, size_t length)
{
    while (length > 1 && limbs[length - 1] == 0)
        length--;

    return length;
}

bool NaturalFromStr(const char *digits, size_t length, Natural *out)
{
    if (digits == NULL || length == 0 || out == NULL)
        return false;

    size_t start = 0;
    while (start + 1 < length && digits[start] == '0')
        start++;

    for (size_t i = start; i < length; i++)
        if (!isDigit(digits[i]))
            return false;

    if (length - start == 1 && digits[start] == '0')
        return false;

    Natural result;
    result.length = 1;

    if (!Allocate(sizeof(uint32_t), &result.limbs))
        return false;

    result.limbs[0] = 0;

    for (size_t i = start; i < length; i++)
    {
        uint32_t digitValue = (uint32_t)(digits[i] - '0');
        if (!mulSmallAddSmall(&result.limbs, &result.length, 10, digitValue))
        {
            Release(&result.limbs);
            return false;
        }
    }

    *out = result;
    return true;
}

void NaturalFree(Natural *natural)
{
    if (natural == NULL)
        return;

    Release(&natural->limbs);
    natural->length = 0;
}

void NaturalShow(Natural natural)
{
    if (natural.limbs == NULL || natural.length == 0)
    {
        printf("Natural => Invalid\n");
        return;
    }

    size_t length = natural.length;
    uint32_t *scratch;

    if (!Allocate(length * sizeof(uint32_t), &scratch))
    {
        printf("Natural => Too big\n");
        return;
    }

    memcpy(scratch, natural.limbs, length * sizeof(uint32_t));

    size_t capacity = length * 10 + 2;
    char *decimalDigits;

    if (!Allocate(capacity, &decimalDigits))
    {
        Release(&scratch);
        printf("Natural => Too big\n");
        return;
    }

    size_t count = 0;
    size_t remaining = length;
    do
    {
        uint32_t remainder = divSmallInPlace(scratch, remaining, 10);
        decimalDigits[count++] = (char)('0' + remainder);
        remaining = effectiveLength(scratch, remaining);
    } while (!(remaining == 1 && scratch[0] == 0));

    printf("Natural => ");
    for (size_t i = count; i > 0; i--)
        putchar(decimalDigits[i - 1]);

    putchar('\n');

    Release(&decimalDigits);
    Release(&scratch);
}

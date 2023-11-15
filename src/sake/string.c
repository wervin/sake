#include <stdlib.h>
#include <string.h>

#include "sake/string.h"
#include "sake/macro.h"

#define GET_SIZE(base)                      (*((uint32_t *) ((uint8_t *) (base))))
#define GET_CAPACITY(base)                  (*((uint32_t *) ((uint8_t *) (base) + sizeof(uint32_t))))
#define GET_DATA_PTR(base)                  ((sake_string) ((uint8_t *) (base) + 2 * sizeof(uint32_t)))
#define GET_BASE_PTR(string)                ((uint8_t *) (string) - 2 * sizeof(uint32_t))

#define SET_SIZE(base, size)                (GET_SIZE(base) = (size))
#define SET_CAPACITY(base, capacity)        (GET_CAPACITY(base) = (capacity))

#define STRING_META_SIZE                    (2 * sizeof(uint32_t))

#define AT(base, i) (((uint8_t *) (base)) + (i))

static uint32_t _next_pow2(uint32_t v);
static inline void _set(char *a, const char *b, uint32_t size);

sake_string sake_string_new(const char *string)
{
    uint32_t length, size, capacity;
    sake_string base;

    length = strlen(string);
    capacity = _next_pow2(length + 1);
    size = STRING_META_SIZE + capacity;
    base = malloc(size);
    SET_SIZE(base, length + 1);
    SET_CAPACITY(base, capacity);

    if (length)
        _set(GET_DATA_PTR(base), string, length);
    GET_DATA_PTR(base)[length] = '\0';

    return GET_DATA_PTR(base);
}

void sake_string_free(sake_string string)
{   
    void * base;

    base = GET_BASE_PTR(string);
    free(base);
}

static uint32_t _next_pow2(uint32_t v)
{
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;

    return v;
}

static inline void _set(char *a, const char *b, uint32_t size)
{
    do
    {
        *a++ = *b++;
    } while (--size > 0);
}
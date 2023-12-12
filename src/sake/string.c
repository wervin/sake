#include <stdlib.h>
#include <string.h>

#include "sake/string.h"
#include "sake/macro.h"
#include "sake/utils.h"

#define GET_SIZE(base)                      (*((uint32_t *) ((uint8_t *) (base))))
#define GET_CAPACITY(base)                  (*((uint32_t *) ((uint8_t *) (base) + sizeof(uint32_t))))
#define GET_DATA_PTR(base)                  ((sake_string) ((uint8_t *) (base) + 2 * sizeof(uint32_t)))
#define GET_BASE_PTR(string)                ((uint8_t *) (string) - 2 * sizeof(uint32_t))

#define SET_SIZE(base, size)                (GET_SIZE(base) = (size))
#define SET_CAPACITY(base, capacity)        (GET_CAPACITY(base) = (capacity))

#define STRING_META_SIZE                    (2 * sizeof(uint32_t))

#define AT(base, i) ((char *) ((uint8_t *) (base)) + (i))

static sake_string _grow(sake_string string, uint32_t size);
static uint32_t _raw_index(sake_string string, uint32_t index);

sake_string sake_string_new(const char *string)
{
    uint32_t length, size, capacity;
    sake_string base;

    length = strlen(string);
    capacity = sake_utils_next_pow2(length + 1);
    size = STRING_META_SIZE + capacity;
    base = malloc(size);
    if (!base)
        return NULL;
    SET_SIZE(base, length);
    SET_CAPACITY(base, capacity);

    memcpy(GET_DATA_PTR(base), string, length);
    GET_DATA_PTR(base)[length] = '\0';

    return GET_DATA_PTR(base);
}

sake_string sake_string_new_range(const char *begin, const char *end)
{
    uint32_t length, size, capacity;
    sake_string base;

    length = *end - *begin;
    capacity = sake_utils_next_pow2(length + 1);
    size = STRING_META_SIZE + capacity;
    base = malloc(size);
    if (!base)
        return NULL;
    SET_SIZE(base, length);
    SET_CAPACITY(base, capacity);

    memcpy(GET_DATA_PTR(base), begin, length);
    GET_DATA_PTR(base)[length] = '\0';

    return GET_DATA_PTR(base);
}

void sake_string_free(sake_string string)
{   
    void * base;

    base = GET_BASE_PTR(string);
    free(base);
}

uint32_t sake_string_raw_size(sake_string string)
{
    return GET_SIZE(GET_BASE_PTR(string));
}

uint32_t sake_string_utf8_size(sake_string string)
{
    uint32_t raw_size = GET_SIZE(GET_BASE_PTR(string));
    uint32_t utf8_size = 0;
    uint32_t i = 0;
    while (i < raw_size)
    {
        i += sake_utils_utf8_length(*AT(string, i));
        utf8_size++;
    }
    return utf8_size;
}

bool sake_string_empty(sake_string string)
{
    return GET_SIZE(GET_BASE_PTR(string)) == 0;
}

uint32_t sake_string_at(sake_string string, uint32_t index)
{
    uint32_t raw_size = GET_SIZE(GET_BASE_PTR(string));
    uint32_t position = 0;
    uint32_t i = 0;
    while (i < raw_size && position != index)
    {
        i += sake_utils_utf8_length(*AT(string, i));
        position++;
    }

    return sake_utils_utf8_from_char(AT(string, i));
}

sake_string sake_string_push_back(sake_string string, const char *data)
{
    uint32_t capacity, size, length;

    length = strlen(data);

    capacity = GET_CAPACITY(GET_BASE_PTR(string));
    size = GET_SIZE(GET_BASE_PTR(string));

    if (capacity < (size + length))
    {
        string = _grow(string, size + length);
        if (!string)
            return NULL;
    }

    /* concatenate */
    memcpy(AT(string, size), data, length);
    string[size + length] = '\0';
    SET_SIZE(GET_BASE_PTR(string), size + length);

    return string;
}

void sake_string_pop_back(sake_string string)
{
    uint32_t utf8_size, index;
    utf8_size = sake_string_utf8_size(string);
    index = _raw_index(string, utf8_size - 1);
    string[index] = '\0';
    SET_SIZE(GET_BASE_PTR(string), index);
}

void sake_string_erase(sake_string string, uint32_t index)
{
    uint32_t utf8_size, raw_index, raw_size;
    raw_size = GET_SIZE(GET_BASE_PTR(string));
    raw_index = _raw_index(string, index);
    utf8_size = sake_utils_utf8_length(string[raw_index]);
    memmove(AT(string, raw_index), AT(string, raw_index + utf8_size), raw_size - (raw_index + utf8_size));
    string[raw_size - utf8_size] = '\0';
    SET_SIZE(GET_BASE_PTR(string), raw_size - utf8_size);
}

void sake_string_erase_range(sake_string string, uint32_t from, uint32_t to)
{
    uint32_t range, from_raw_index, to_raw_index, raw_size;
    raw_size = GET_SIZE(GET_BASE_PTR(string));
    from_raw_index = _raw_index(string, from);
    to_raw_index = _raw_index(string, to);
    range = to_raw_index - from_raw_index;
    memmove(AT(string, from_raw_index), AT(string, to_raw_index), raw_size - to_raw_index);
    string[raw_size - range] = '\0';
    SET_SIZE(GET_BASE_PTR(string), raw_size - range);
}

sake_string sake_string_insert(sake_string string, uint32_t index, const char *data)
{
    uint32_t capacity, size, length, raw_index;

    length = strlen(data);

    capacity = GET_CAPACITY(GET_BASE_PTR(string));
    size = GET_SIZE(GET_BASE_PTR(string));

    if (capacity < (size + length))
    {
        string = _grow(string, size + length);
        if (!string)
            return NULL;
    }

    raw_index = _raw_index(string, index);

    memmove(AT(string, raw_index + length), AT(string, raw_index), size - raw_index);
    memcpy(AT(string, raw_index), data, length);
    string[size + length] = '\0';
    SET_SIZE(GET_BASE_PTR(string), size + length);
    
    return string;
}

static sake_string _grow(sake_string string, uint32_t size)
{
    uint32_t new_capacity, new_size;
    void *base;

    new_capacity = sake_utils_next_pow2(size);
    new_size = STRING_META_SIZE + new_capacity;

    base = GET_BASE_PTR(string);
    base = realloc(base, new_size);
    if (!base)
        return NULL;

    SET_CAPACITY(base, new_capacity);
    return GET_DATA_PTR(base);
}

static uint32_t _raw_index(sake_string string, uint32_t index)
{
    uint32_t raw_size = GET_SIZE(GET_BASE_PTR(string));
    uint32_t position = 0;
    uint32_t i = 0;
    while (i < raw_size && position != index)
    {
        i += sake_utils_utf8_length(*AT(string, i));
        position++;
    }

    return i;
}
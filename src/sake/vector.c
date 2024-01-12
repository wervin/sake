#include <stdlib.h>
#include <string.h>

#include "sake/vector.h"
#include "sake/array.h"
#include "sake/macro.h"
#include "sake/utils.h"

#define GET_DESTRUCTOR(base) (*((sake_vector_destructor *)((uint8_t *)(base))))
#define GET_ELT_SIZE(base) (*((uint32_t *)((uint8_t *)(base) + sizeof(sake_vector_destructor))))
#define GET_SIZE(base) (*((uint32_t *)((uint8_t *)(base) + sizeof(sake_vector_destructor) + sizeof(uint32_t))))
#define GET_CAPACITY(base) (*((uint32_t *)((uint8_t *)(base) + sizeof(sake_vector_destructor) + 2 * sizeof(uint32_t))))
#define GET_DATA_PTR(base) ((sake_vector *)((uint8_t *)(base) + sizeof(sake_vector_destructor) + 3 * sizeof(uint32_t)))
#define GET_BASE_PTR(vec) ((uint8_t *)(vec) - sizeof(sake_vector_destructor) - 3 * sizeof(uint32_t))

#define SET_DESTRUCTOR(base, destructor) (GET_DESTRUCTOR(base) = (destructor))
#define SET_ELT_SIZE(base, elt_size) (GET_ELT_SIZE(base) = (elt_size))
#define SET_SIZE(base, size) (GET_SIZE(base) = (size))
#define SET_CAPACITY(base, capacity) (GET_CAPACITY(base) = (capacity))

#define VECTOR_META_SIZE (sizeof(sake_vector_destructor) + 3 * sizeof(uint32_t))

#define AT(base, i, size) (((uint8_t *)(base)) + ((i) * (size)))

static sake_vector *_grow(sake_vector *vec, uint32_t size);

static inline void _set(void *a, void *b, uint32_t size);

sake_vector *sake_vector_new(uint32_t elt_size, sake_vector_destructor destructor)
{
    uint32_t size;
    void *base;

    size = VECTOR_META_SIZE + elt_size;
    base = malloc(size);
    if (!base)
        return NULL;

    SET_DESTRUCTOR(base, destructor);
    SET_ELT_SIZE(base, elt_size);
    SET_SIZE(base, 0);
    SET_CAPACITY(base, 1);

    return GET_DATA_PTR(base);
}

uint32_t sake_vector_size(sake_vector *vec)
{
    return GET_SIZE(GET_BASE_PTR(vec));
}

uint32_t sake_vector_capacity(sake_vector *vec)
{
    return GET_CAPACITY(GET_BASE_PTR(vec));
}

sake_vector *sake_vector_push_back(sake_vector *vec, void *elt)
{
    uint32_t capacity, size, elt_size;

    elt_size = GET_ELT_SIZE(GET_BASE_PTR(vec));
    capacity = GET_CAPACITY(GET_BASE_PTR(vec));
    size = GET_SIZE(GET_BASE_PTR(vec));

    if (capacity < (size + 1))
    {
        vec = _grow(vec, size + 1);
        if (!vec)
            return NULL;
    }

    /* vec[size] = elt */
    _set(AT(vec, size, elt_size), elt, elt_size);
    SET_SIZE(GET_BASE_PTR(vec), size + 1);

    return vec;
}

void sake_vector_free(sake_vector *vec)
{
    uint32_t elt_size;
    void *base;
    sake_vector_destructor destructor;

    base = GET_BASE_PTR(vec);
    destructor = GET_DESTRUCTOR(base);
    elt_size = GET_ELT_SIZE(base);

    if (destructor)
    {
        for (uint32_t i = 0; i < GET_SIZE(base); i++)
        {
            destructor(*(void **)AT(vec, i, elt_size));
        }
    }

    free(base);
}

bool sake_vector_empty(sake_vector *vec)
{
    return GET_SIZE(GET_BASE_PTR(vec)) == 0;
}

void sake_vector_erase(sake_vector *vec, uint32_t index)
{
    uint32_t size, elt_size;
    sake_vector_destructor destructor;

    elt_size = GET_ELT_SIZE(GET_BASE_PTR(vec));
    destructor = GET_DESTRUCTOR(GET_BASE_PTR(vec));

    if (destructor)
        destructor(*(void **)AT(vec, index, elt_size));

    size = GET_SIZE(GET_BASE_PTR(vec));
    memmove(AT(vec, index, elt_size), AT(vec, index + 1, elt_size), elt_size * (size - (index + 1)));
    SET_SIZE(GET_BASE_PTR(vec), size - 1);
}

void sake_vector_clear(sake_vector *vec)
{
    uint32_t elt_size, size;
    void *base;
    sake_vector_destructor destructor;

    base = GET_BASE_PTR(vec);
    destructor = GET_DESTRUCTOR(base);
    elt_size = GET_ELT_SIZE(base);
    size = GET_SIZE(GET_BASE_PTR(vec));

    if (destructor)
    {
        for (uint32_t i = 0; i < GET_SIZE(base); i++)
        {
            destructor(*(void **)AT(vec, i, elt_size));
        }
    }

    memset(vec, 0, elt_size * size);
    SET_SIZE(GET_BASE_PTR(vec), 0);
}

void sake_vector_erase_range(sake_vector *vec, uint32_t from, uint32_t to)
{
    uint32_t size, elt_size;
    sake_vector_destructor destructor;

    elt_size = GET_ELT_SIZE(GET_BASE_PTR(vec));
    destructor = GET_DESTRUCTOR(GET_BASE_PTR(vec));

    if (destructor)
        for (uint32_t i = from; i < to; i++)
            destructor(*(void **)AT(vec, i, elt_size));

    size = GET_SIZE(GET_BASE_PTR(vec));
    memmove(AT(vec, from, elt_size), AT(vec, to, elt_size), elt_size * (size - to));
    SET_SIZE(GET_BASE_PTR(vec), size - (to - from));
}

sake_vector *sake_vector_insert(sake_vector *vec, uint32_t index, void *elt)
{
    uint32_t capacity, size, elt_size;

    elt_size = GET_ELT_SIZE(GET_BASE_PTR(vec));
    capacity = GET_CAPACITY(GET_BASE_PTR(vec));
    size = GET_SIZE(GET_BASE_PTR(vec));

    if (capacity < (size + 1))
    {
        vec = _grow(vec, size + 1);
        if (!vec)
            return NULL;
    }

    memmove(AT(vec, index + 1, elt_size), AT(vec, index, elt_size), elt_size * (size - index));

    /* vec[index] = elt */
    _set(AT(vec, index, elt_size), elt, elt_size);
    SET_SIZE(GET_BASE_PTR(vec), size + 1);

    return vec;
}

sake_vector *sake_vector_insert_range(sake_vector *vec, uint32_t index, void *elts, uint32_t n)
{
    uint32_t capacity, size, elt_size;

    elt_size = GET_ELT_SIZE(GET_BASE_PTR(vec));
    capacity = GET_CAPACITY(GET_BASE_PTR(vec));
    size = GET_SIZE(GET_BASE_PTR(vec));

    if (capacity < (size + n))
    {
        vec = _grow(vec, size + n);
        if (!vec)
            return NULL;
    }

    memmove(AT(vec, index + n, elt_size), AT(vec, index, elt_size), elt_size * (size - index));
    memcpy(AT(vec, index, elt_size), elts, elt_size * n);
    SET_SIZE(GET_BASE_PTR(vec), size + n);
    
    return vec;
}

void sake_vector_pop_back(sake_vector *vec)
{
    uint32_t size, elt_size;
    sake_vector_destructor destructor;

    elt_size = GET_ELT_SIZE(GET_BASE_PTR(vec));
    destructor = GET_DESTRUCTOR(GET_BASE_PTR(vec));
    size = GET_SIZE(GET_BASE_PTR(vec));

    if (destructor)
        destructor(*(void**)AT(vec, size - 1, elt_size));

    SET_SIZE(GET_BASE_PTR(vec), size - 1);
}

sake_vector *sake_vector_copy(sake_vector *from, sake_vector *to)
{
    uint32_t size_from, capacity_to, elt_size;

    elt_size = GET_ELT_SIZE(GET_BASE_PTR(from));
    size_from = GET_SIZE(GET_BASE_PTR(from));
    capacity_to = GET_CAPACITY(GET_BASE_PTR(to));

    if (capacity_to < (size_from))
    {
        to = _grow(to, size_from);
        if (!to)
            return NULL;
    }

    memcpy(to, from, elt_size * size_from);

    SET_SIZE(GET_BASE_PTR(to), size_from);

    return to;
}

void sake_vector_heapsort(sake_vector *vec, int32_t (*comparator)(const void *, const void *))
{
    uint32_t elt_size;
    elt_size = GET_ELT_SIZE(GET_BASE_PTR(vec));
    sake_array_heapsort(vec, GET_SIZE(GET_BASE_PTR(vec)), elt_size, comparator);
}

void sake_vector_quicksort(sake_vector *vec, int32_t (*comparator)(const void *, const void *))
{
    uint32_t elt_size;
    elt_size = GET_ELT_SIZE(GET_BASE_PTR(vec));
    sake_array_quicksort(vec, GET_SIZE(GET_BASE_PTR(vec)), elt_size, comparator);
}

int32_t sake_vector_binarysearch(sake_vector *vec, int32_t (*comparator)(const void *, const void *), void *target)
{
    uint32_t elt_size;
    elt_size = GET_ELT_SIZE(GET_BASE_PTR(vec));
    return sake_array_binarysearch(vec, GET_SIZE(GET_BASE_PTR(vec)), elt_size, comparator, target);
}

int32_t sake_vector_find(sake_vector *vec, bool (*predicate)(const void *, const void *), void *target)
{
    uint32_t elt_size;
    elt_size = GET_ELT_SIZE(GET_BASE_PTR(vec));
    return sake_array_find(vec, GET_SIZE(GET_BASE_PTR(vec)), elt_size, predicate, target);
}

int32_t sake_vector_rfind(sake_vector *vec, bool (*predicate)(const void *, const void *), void *target)
{
    uint32_t elt_size;
    elt_size = GET_ELT_SIZE(GET_BASE_PTR(vec));
    return sake_array_rfind(vec, GET_SIZE(GET_BASE_PTR(vec)), elt_size, predicate, target);
}

static sake_vector *_grow(sake_vector *vec, uint32_t size)
{
    uint32_t new_capacity, new_size, elt_size;
    void *base;

    elt_size = GET_ELT_SIZE(GET_BASE_PTR(vec));

    new_capacity = sake_utils_next_pow2(size);
    new_size = VECTOR_META_SIZE + elt_size * new_capacity;

    base = GET_BASE_PTR(vec);
    base = realloc(base, new_size);
    if (!base)
        return NULL;

    SET_CAPACITY(base, new_capacity);
    return GET_DATA_PTR(base);
}

static inline void _set(void *a, void *b, uint32_t size)
{
    uint8_t *__a = (a), *__b = (b);
    do
    {
        *__a++ = *__b++;
    } while (--size > 0);
}
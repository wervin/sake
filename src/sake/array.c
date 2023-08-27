#include "sake/array.h"
#include "sake/macro.h"

#define SWAP(a, b, size)                \
    SAKE_MACRO_BEGIN                    \
        uint32_t __size = (size);       \
        uint8_t *__a = (a), *__b = (b); \
        do                              \
        {                               \
            uint8_t __tmp = *__a;       \
            *__a++ = *__b;              \
            *__b++ = __tmp;             \
        } while (--__size > 0);         \
    SAKE_MACRO_END

#define AT(base, index, size) (((uint8_t *) (base)) + ((index) * (size)))

static void _heapify(void * base, uint32_t n, uint32_t size, uint32_t root_idx, int32_t (*comparator)(const void *, const void *));

void sake_array_heapsort(void * base, uint32_t n, uint32_t size, int32_t (*comparator)(const void *, const void *))
{
    /* Build the binary max heap */
    for (int32_t i = n / 2; i >= 0; i--)
    {
        _heapify(base, n, size, i, comparator);
    }
    /* Sort the binary max heap */
    for (int32_t i = n - 1; i >= 0; i--)
    {
        /* Swap the root node and the last leaf node */
        SWAP(AT(base, 0, size), AT(base, i, size), size);
        /* Again heapify the max heap from the root */
        _heapify(base, i, size, 0, comparator);
    }
}

void sake_array_quicksort(void * base, uint32_t n, uint32_t size, int32_t (*comparator) (const void *, const void*))
{
    int32_t i, j, p, stack_idx, start, end;
    /* can sort up to 2^64 elements */
    int32_t start_stack[64];
    int32_t end_stack[64];

    start_stack[0] = 0;
    end_stack[0] = n;
    stack_idx = 1;
    while (stack_idx > 0)
    {
        stack_idx--;
        start = start_stack[stack_idx];
        end = end_stack[stack_idx];
        while (end - start > 2)
        {
            p = start;
            i = start + 1;
            j = end - 1;
            while (i < j)
            {
                while (i <= j && comparator(AT(base, i, size), AT(base, p, size) ) <= 0)
                    i++;
                
                if (i > j)
                {
                    /* all remaining elements lesseq than pivot */
                    SWAP(AT(base, j, size), AT(base, p, size), size);
                    i = j;
                }
                else
                {
                    while (i <= j && comparator(AT(base, j, size), AT(base, p, size) ) >=0)
                        j--;

                    if (i > j)
                    {
                        /* all remaining elements greater than pivot */
                        SWAP(AT(base, j, size), AT(base, p, size), size);
                        i = j;
                    }
                    else if (i < j)
                    {
                        SWAP(AT(base, i, size), AT(base, j, size), size);
                        if (i + 2 < j)
                        {
                            i++;
                            j--;
                        }
                        else if (i + 1 < j)
                            i++;
                    }
                }
            }
            /* O.K. i==j and pivot is on a[i] == a[j] */
            /* handle recursive calls without recursion */
            if (i - start > 1 && end - j > 1)
            {
                /* two recursive calls, use array-stack */
                if (i - start < end - j - 1)
                {
                    start_stack[stack_idx] = j + 1;
                    end_stack[stack_idx] = end;
                    stack_idx++;
                    end = i;
                }
                else
                {
                    start_stack[stack_idx] = start;
                    end_stack[stack_idx] = i;
                    stack_idx++;
                    start = j + 1;
                }
            }
            else
            {
                if (i - start > 1)
                {
                    end = i;
                }
                else
                {
                    start = j + 1;
                }
            }
        }
        if (end - start == 2)
        {
            if (comparator(AT(base, start, size), AT(base, end - 1, size)) > 0)
            {
                SWAP(AT(base, start, size), AT(base, end - 1, size), size);
            }
        }
    }
}

int32_t sake_array_binarysearch(void * base, uint32_t n, uint32_t size, int32_t (*comparator) (const void *, const void*), void * target)
{
    int32_t mid, match, i, j;
    i = 0;
    j = n;
    while (i <= j)
    {
        mid = (j + i) / 2;
        match = comparator(AT(base, mid, size), target);
        if (match == 0)
            return mid;
        else if (match < 0)
            i = mid + 1;
        else
            j = mid - 1;
    }

    return -1;
}

int32_t sake_array_find(void * base, uint32_t n, uint32_t size, bool (*predicate) (const void *, const void*), void * target)
{
    for (int32_t i = 0; i < n; i++)
    {
        if (predicate(AT(base, i, size), target))
            return i;
    }
    return -1;
}

int32_t sake_array_rfind(void * base, uint32_t n, uint32_t size, bool (*predicate) (const void *, const void*), void * target)
{
    for (int32_t i = n - 1; i >= 0; i--)
    {
        if (predicate(AT(base, i, size), target))
            return i;
    }
    return -1;
}

static void _heapify(void * base, uint32_t max, uint32_t size, uint32_t root_idx, int32_t (*comparator)(const void *, const void *))
{
    uint32_t left_idx, right_idx, largest_idx;
    
    /* Store the root index as the largest_idx element */
    largest_idx = root_idx;
    do
    {
        /* Update root, left, right indexes */
        root_idx = largest_idx;
        left_idx = 2 * root_idx + 1;
        right_idx = left_idx + 1;
        /* Check whether the left is larger than the current root */
        if ((left_idx < max) && (comparator(AT(base, largest_idx, size), AT(base, left_idx, size)) < 0))
        {
            largest_idx = left_idx;
        }
        /* Check whether the right is larger than the current root */
        if ((right_idx < max) && (comparator(AT(base, largest_idx, size), AT(base, right_idx, size)) < 0))
        {
            largest_idx = right_idx;
        }
        /* If the root is smaller than the children then swap it with the largest children's value */
        if (largest_idx != root_idx)
            SWAP(AT(base, root_idx, size), AT(base, largest_idx, size), size);
        /* Heapify until the root is no longer updated */
    } while (largest_idx != root_idx);
}
#ifndef SAKE_ARRAY_H
#define SAKE_ARRAY_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void sake_array_heapsort(void * base, uint32_t n, uint32_t size, int32_t (*comparator) (const void *, const void*));
void sake_array_quicksort(void * base, uint32_t n, uint32_t size, int32_t (*comparator) (const void *, const void*));
int32_t sake_array_binarysearch(void * base, uint32_t n, uint32_t size, int32_t (*comparator) (const void *, const void*), void * target);
int32_t sake_array_find(void * base, uint32_t n, uint32_t size, bool (*predicate) (const void *, const void*), void * target);
int32_t sake_array_rfind(void * base, uint32_t n, uint32_t size, bool (*predicate) (const void *, const void*), void * target);

#ifdef __cplusplus
}
#endif

#endif /* SAKE_ARRAY_H */
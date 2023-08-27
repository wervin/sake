#ifndef SAKE_VECTOR_H
#define SAKE_VECTOR_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  +-----------------+----------+------+----------+------+
 *  | elem_destructor | elt_size | size | capacity | data |
 *  +-----------------+----------+------+----------+------+
 */

typedef void sake_vector;

typedef void (*sake_vector_destructor)(void * elt);

sake_vector * sake_vector_new(uint32_t elt_size, sake_vector_destructor destructor);
uint32_t sake_vector_size(sake_vector * vec);
uint32_t sake_vector_capacity(sake_vector * vec);
sake_vector * sake_vector_push_back(sake_vector * vec, void * elt);
void sake_vector_free(sake_vector * vec);
bool sake_vector_empty(sake_vector * vec);
void sake_vector_erase(sake_vector * vec, uint32_t index);
sake_vector * sake_vector_insert(sake_vector * vec, uint32_t index, void * elt);
void sake_vector_pop_back(sake_vector * vec);
sake_vector * sake_vector_copy(sake_vector * from, sake_vector * to);
void sake_vector_heapsort(sake_vector * vec, int32_t (*comparator) (const void *, const void*));
void sake_vector_quicksort(sake_vector * vec, int32_t (*comparator) (const void *, const void*));
int32_t sake_vector_binarysearch(sake_vector * vec, int32_t (*comparator) (const void *, const void*), void * target);
int32_t sake_vector_find(sake_vector * vec, bool (*predicate) (const void *, const void*), void * target);
int32_t sake_vector_rfind(sake_vector * vec, bool (*predicate) (const void *, const void*), void * target);

#ifdef __cplusplus
}
#endif

#endif /* SAKE_VECTOR_H */
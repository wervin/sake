#ifndef SAKE_LIST_H
#define SAKE_LIST_H

#include <stdint.h>
#include <stdbool.h>

typedef void sake_list_node;

typedef void (*sake_list_destructor)(void * elt);

struct sake_list {
    sake_list_destructor destructor;
    uint32_t elt_size;
    uint32_t size;
    sake_list_node * head;
    sake_list_node * tail;
};

void sake_list_new(struct sake_list * l, uint32_t elt_size, sake_list_destructor destructor);
void sake_list_insert_head(struct sake_list * l, sake_list_node * new);
void sake_list_insert_prev(struct sake_list * l, sake_list_node * front, sake_list_node * new);
void sake_list_insert_tail(struct sake_list * l, sake_list_node * new);
void sake_list_insert_sorted(struct sake_list * l, sake_list_node * new, int32_t (*comparator) (const void *, const void*));
sake_list_node * sake_list_head(struct sake_list * l);
sake_list_node * sake_list_tail(struct sake_list * l);
sake_list_node * sake_list_next(sake_list_node * cur);
sake_list_node * sake_list_prev(sake_list_node * cur);
void sake_list_remove_head(struct sake_list * l);
void sake_list_remove(struct sake_list * l, sake_list_node * n);
void sake_list_remove_tail(struct sake_list * l);
void sake_list_free(struct sake_list * l);
bool sake_list_empty(struct sake_list * l);
uint32_t sake_list_size(struct sake_list * l);
void sake_list_mergesort(struct sake_list * l, int32_t (*comparator) (const void *, const void*));
sake_list_node * sake_list_find(struct sake_list * l, bool (*predicate) (const void *, const void*), void * target);
sake_list_node * sake_list_rfind(struct sake_list * l, bool (*predicate) (const void *, const void*), void * target);

#endif /* SAKE_LIST_H */
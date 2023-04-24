#include <stdlib.h>

#include "sake_list.h"
#include "sake_macro.h"

#define GET_NEXT(base)                      (* (sake_list_node **) (((uint8_t *) (base))))
#define GET_PREV(base)                      (* (sake_list_node **) (((uint8_t *) (base) + sizeof(sake_list_node *))))
#define GET_NODE_PTR(base)                  ((sake_list_node *) ((uint8_t *) (base) + 2 * sizeof(sake_list_node *)))
#define GET_BASE_PTR(node)                  ((uint8_t *) (node) - 2 * sizeof(sake_list_node *))

#define SET_NEXT(base, next)                (GET_NEXT(base) = (next))
#define SET_PREV(base, prev)                (GET_PREV(base) = (prev))

#define SAKE_LIST_NODE_META_SIZE            (2 * sizeof(sake_list_node *))

#define SET(a, b, size)                     \
    SAKE_MACRO_BEGIN                        \
        uint32_t __size = (size);           \
        uint8_t * __a = (a), * __b = (b);   \
        do                                  \
        {                                   \
            *__a++ = *__b++;                \
        } while (--__size > 0);             \
    SAKE_MACRO_END

void sake_list_init(struct sake_list * l, uint32_t elt_size, sake_list_destructor destructor)
{
    l->head = NULL;
    l->tail = NULL;
    l->destructor = destructor;
    l->elt_size = elt_size;
    l->size = 0;
}

void sake_list_insert_head(struct sake_list * l, sake_list_node * new)
{
    sake_list_node * base = malloc(l->elt_size + SAKE_LIST_NODE_META_SIZE);
    
    /* Copy element */
    SET(GET_NODE_PTR(base), new, l->elt_size);
    /* No prev before the new head */
    SET_PREV(base, NULL);
    /* After new comes the old head */
    SET_NEXT(base, l->head);
    /* If there is old head then before it goes the new */
    if (l->head != NULL)
        SET_PREV(GET_BASE_PTR(l->head), GET_NODE_PTR(base));

    /* Set the new head */
    l->head = GET_NODE_PTR(base); 
    /* If there is no tail set the tail too */
    if (l->tail == NULL)
        l->tail = GET_NODE_PTR(base);

    l->size++;
}

void sake_list_insert_prev(struct sake_list * l, sake_list_node * front, sake_list_node * new)
{
    sake_list_node * base, * base_prev;

    if (sake_list_head(l) ==  front)
    {
        sake_list_insert_head(l, new);
        return;
    }

    base = malloc(l->elt_size + SAKE_LIST_NODE_META_SIZE);
    
    /* Copy element */
    SET(GET_NODE_PTR(base), new, l->elt_size);

    base_prev = GET_BASE_PTR(GET_PREV(GET_BASE_PTR(front)));

    SET_NEXT(base_prev, GET_NODE_PTR(base));
    SET_PREV(base, GET_NODE_PTR(base_prev));
    SET_PREV(GET_BASE_PTR(front), GET_NODE_PTR(base));
    SET_NEXT(base, front);

    l->size++;
}

void sake_list_insert_tail(struct sake_list * l, sake_list_node * new)
{
    sake_list_node * base = malloc(l->elt_size + SAKE_LIST_NODE_META_SIZE);

    /* Copy element */
    SET(GET_NODE_PTR(base), new, l->elt_size);
    /* No next after the new tail */
    SET_NEXT(base, NULL);
    /* Before new comes the old tail */
    SET_PREV(base, l->tail);
    /* If there is old tail then after it goes the new */
    if (l->tail != NULL)
        SET_NEXT(GET_BASE_PTR(l->tail), GET_NODE_PTR(base));

    /* Set the new tail */
    l->tail = GET_NODE_PTR(base); 
    /* If there is no head set the head too */
    if (l->head == NULL)
        l->head = GET_NODE_PTR(base);
    
    l->size++;
}

void sake_list_insert_sorted(struct sake_list * l, sake_list_node * new, int32_t (*comparator) (const void *, const void*))
{
    sake_list_node * it;

    it = sake_list_head(l);
    while (it != NULL && comparator(it, new) < 0)
        it = sake_list_next(it);

    if (it != NULL)
        sake_list_insert_prev(l, it, new);
    else
        sake_list_insert_tail(l, new);
}

sake_list_node * sake_list_head(struct sake_list * l)
{
    return l->head;
}

sake_list_node * sake_list_tail(struct sake_list * l)
{
    return l->tail;
}

sake_list_node * sake_list_next(sake_list_node * cur)
{
    return GET_NEXT(GET_BASE_PTR(cur));
}

sake_list_node * sake_list_prev(sake_list_node * cur)
{
    return GET_PREV(GET_BASE_PTR(cur));
}

void sake_list_remove_head(struct sake_list * l)
{
    sake_list_node * base, * node_next;
    base = GET_BASE_PTR(l->head);
    node_next = GET_NEXT(base);

    if (node_next == NULL)
    {
        l->head = NULL;
        l->tail = NULL;
    }
    else {
        SET_PREV(GET_BASE_PTR(node_next), NULL);
        l->head = node_next;
    }

    if (l->destructor)
        l->destructor(* (sake_list_node **) GET_NODE_PTR(base));
    free(base);
    l->size--;
}

void sake_list_remove(struct sake_list * l, sake_list_node * n)
{
    if (sake_list_head(l) == n)
        sake_list_remove_head(l);
    else if (sake_list_tail(l) == n)
        sake_list_remove_tail(l);
    else
    { 
        sake_list_node * base = GET_BASE_PTR(n);
        sake_list_node * node_prev = GET_PREV(base);
        sake_list_node * node_next = GET_NEXT(base);

        SET_NEXT(GET_BASE_PTR(node_prev), node_next);
        SET_PREV(GET_BASE_PTR(node_next), node_prev);

        if (l->destructor)
            l->destructor(* (sake_list_node **) GET_NODE_PTR(base));
        free(base);
        l->size--;
    }
}

void sake_list_remove_tail(struct sake_list * l)
{
    sake_list_node * base, * node_prev;
    base = GET_BASE_PTR(l->tail);
    node_prev = GET_PREV(base);
    
    if (node_prev == NULL)
    {
        l->head = NULL;
        l->tail = NULL;
    }
    else {
        SET_NEXT(GET_BASE_PTR(node_prev), NULL);
        l->tail = node_prev;
    }

    if (l->destructor)
        l->destructor(* (sake_list_node **) GET_NODE_PTR(base));
    free(base);
    l->size--;
}

void sake_list_free(struct sake_list * l)
{
    sake_list_node * cur, * base;

    cur = sake_list_head(l);
    while (cur != NULL)
    {
        base = GET_BASE_PTR(cur);
        cur = sake_list_next(cur);
        if (l->destructor)
            l->destructor(* (sake_list_node **) GET_NODE_PTR(base));
        free(base);
    }
    l->size = 0;
}

bool sake_list_empty(struct sake_list * l)
{
    return l->size == 0;
}

uint32_t sake_list_size(struct sake_list * l)
{
    return l->size;
}

void sake_list_mergesort(struct sake_list * l, int32_t (*comparator) (const void *, const void*))
{
    bool done;
    uint32_t n, i;
    sake_list_node * it, * a, * b, * start, * head, * prev;
    sake_list_node ** tmp;

    head = l->head;
    done = false;
    n = 1;
    while (!done)
    {
        done = true;
        start = head;
        tmp = &head;
        while (start != NULL)
        {
            a = start;

            i = 1;
            it = a;
            while (i < n && it != NULL)
            {
                i++;
                it = sake_list_next(it);
            }

            if (it == NULL)
            {
                *tmp = a;
                break;
            }

            b = sake_list_next(it);
            SET_NEXT(GET_BASE_PTR(it), NULL);

            i = 1;
            it = b;
            while (i < n && it != NULL)
            {
                i++;
                it = sake_list_next(it);
            }

            if (it == NULL)
                start = NULL;
            else {
                start = sake_list_next(it);
                SET_NEXT(GET_BASE_PTR(it), NULL);
            }

            /* merge */
            while (a != NULL && b != NULL)
            {
                /* if b is larger than a*/
                if (comparator(a, b) < 0)
                {
                    *tmp = a;
                    tmp = &GET_NEXT(GET_BASE_PTR(a));
                    a = sake_list_next(a);
                }
                /* if a is larger than b*/
                else
                {
                    *tmp = b;
                    tmp = &GET_NEXT(GET_BASE_PTR(b));
                    b = sake_list_next(b);
                }
            }
            
            *tmp = a != NULL ? a : b;

            while (*tmp != NULL)
                tmp = &GET_NEXT(GET_BASE_PTR(*tmp));

            done = false;
        }

        n += n;
    }

    l->head = head;

    prev = NULL;
    it = sake_list_head(l);
    while (it != NULL)
    {
        SET_PREV(GET_BASE_PTR(it), prev);
        prev = it;
        it = sake_list_next(it);
    }
}

sake_list_node * sake_list_find(struct sake_list * l, bool (*predicate) (const void *, const void*), void * target)
{
    sake_list_node * cur;

    cur = sake_list_head(l);
    while (cur != NULL && !predicate(cur, target))
        cur = sake_list_next(cur);
    return cur;
}

sake_list_node * sake_list_rfind(struct sake_list * l, bool (*predicate) (const void *, const void*), void * target)
{
    sake_list_node * cur;

    cur = sake_list_tail(l);
    while (cur != NULL && !predicate(cur, target))
        cur = sake_list_prev(cur);
    return cur;
}
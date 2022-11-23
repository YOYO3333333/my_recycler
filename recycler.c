#include "recycler.h"

#include <stdlib.h>

struct recycler *recycler_create(size_t block_size, size_t total_size)
{
    if (block_size % sizeof(size_t) != 0)
        return NULL;
    else if (block_size == 0 || total_size == 0)
        return NULL;
    else if (total_size % block_size != 0)
        return NULL;
    void *new_chunk = malloc(total_size);
    if (new_chunk == NULL)
        return NULL;
    struct recycler *r = malloc(sizeof(struct recycler));
    if (!r)
        return NULL;
    r->chunk = new_chunk;
    r->block_size = block_size;
    r->capacity = total_size / block_size;
    r->free = new_chunk;
    struct free_list *tmp = r->free;
    for (size_t i = 0; i < r->capacity - 1; i++)
    {
        char *sara = (char *)tmp + block_size;
        void *s = sara;
        tmp->next = s;
    }
    return r;
}

void recycler_destroy(struct recycler *r)
{
    if (r != NULL)
    {
        free(r->chunk);
        free(r);
    }
    else
        return;
}

void *recycler_allocate(struct recycler *r)
{
    if (r == NULL)
        return NULL;
    if (r->free == NULL)
        return NULL;
    struct free_list *e = r->free;
    void *ch = r->free;
    r->free = e->next;
    return ch;
}

void recycler_free(struct recycler *r, void *block)
{
    if (r == NULL)
        return;
    r->free = block;
    struct free_list *e = r->free;
    e->next = block;
}

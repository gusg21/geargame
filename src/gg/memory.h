#if !defined(GG_MEMORY_H)
#define GG_MEMORY_H

#include <stdint.h>
#include <stdlib.h>

#define GG_MALLOC(size) _GGMemory_malloc(size, __LINE__, ##__FILE__)

typedef struct gg_mem_alloc gg_mem_alloc_t;
typedef struct gg_mem_alloc {
    void* address;
    size_t size;
    int32_t source_line;
    const char* source_name;

    gg_mem_alloc_t* next;
} gg_mem_alloc_t;

gg_mem_alloc_t* g_mem_alloc_list = NULL;

static void* _GGMemory_malloc(size_t size, int32_t source_line, const char* source_name) {
    gg_mem_alloc_t* tail = g_mem_alloc_list;
    gg_mem_alloc_t* new_alloc = NULL;
    if (tail != NULL) {
        while (tail->next != NULL) {
            tail = tail->next;
        }
        tail->next = (gg_mem_alloc_t*)malloc(sizeof(gg_mem_alloc_t));
        new_alloc = tail->next;
    } else {
        tail = (gg_mem_alloc_t*)malloc(sizeof(gg_mem_alloc_t));
        new_alloc = tail;
    }

    new_alloc->address = malloc(size);
    new_alloc->size = size;
    new_alloc->source_line = source_line;
    new_alloc->source_name = source_name;
}

#endif // GG_MEMORY_H

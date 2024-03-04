#include "memory.h"

#include "log.h"

gg_mem_alloc_t* g_mem_alloc_list = NULL;

void* _Memory_Malloc(size_t size, int32_t source_line, const char* source_name) {
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
    new_alloc->freed = false;

    return new_alloc->address;
}

void* _Memory_Calloc(size_t count, size_t size, int32_t source_line, const char* source_name) {
    gg_mem_alloc_t* tail = g_mem_alloc_list;
    gg_mem_alloc_t* new_alloc = NULL;
    if (tail != NULL) {
        while (tail->next != NULL) {
            tail = tail->next;
        }
        tail->next = (gg_mem_alloc_t*)malloc(sizeof(gg_mem_alloc_t));
        new_alloc = tail->next;
    } else {
        g_mem_alloc_list = (gg_mem_alloc_t*)malloc(sizeof(gg_mem_alloc_t));
        new_alloc = g_mem_alloc_list;
    }

    new_alloc->address = calloc(count, size);
    new_alloc->size = size;
    new_alloc->source_line = source_line;
    new_alloc->source_name = source_name;
    new_alloc->freed = false;

    return new_alloc->address;
}

void _Memory_Free(void* address, int32_t source_line, const char* source_name) {
    gg_mem_alloc_t* alloc = g_mem_alloc_list;
    while (alloc != NULL) {
        if (alloc->address == address) {
            if (alloc->freed) {
                Log_Warn(Log_TextFormat("Re-freeing already freed memory! (@0x%x)", address));
            } else {
                free(address);
                alloc->freed = true;
            }
            return;
        }

        alloc = alloc->next;
    }

}

void Memory_PrintAllocations() {
    gg_mem_alloc_t* alloc = g_mem_alloc_list;
    while (alloc != NULL) {
        if (!alloc->freed) {
            Log_Warn(Log_TextFormat("Unfreed memory @%x (source %s:%d)", alloc->address, alloc->source_name,
                                    alloc->source_line));
        }
        alloc = alloc->next;
    }
}

#if !defined(GG_MEMORY_H)
#define GG_MEMORY_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define GG_MALLOC(size) _Memory_Malloc(size, __LINE__, __FILE__)
#define GG_CALLOC(count, size) _Memory_Calloc(count, size, __LINE__, __FILE__)
#define GG_FREE(ptr) _Memory_Free(ptr, __LINE__, __FILE__)

typedef struct gg_mem_alloc gg_mem_alloc_t;
typedef struct gg_mem_alloc {
    void* address;
    size_t size;
    int32_t source_line;
    const char* source_name;
    bool freed;

    gg_mem_alloc_t* next;
} gg_mem_alloc_t;

void* _Memory_Malloc(size_t size, int32_t source_line, const char* source_name);
void* _Memory_Calloc(size_t count, size_t size, int32_t source_line, const char* source_name);
void _Memory_Free(void* address, int32_t source_line, const char* source_name);

void Memory_PrintAllocations();

#endif  // GG_MEMORY_H

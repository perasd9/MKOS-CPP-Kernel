#include "../h/syscall_c.hpp"
#include "../lib/mem.h"

void* operator new (const size_t size) {
    /*void* addr = mem_alloc(size);

    return addr;*/
    return __mem_alloc(size);
}

void* operator new[](const size_t size) {
    /*void* addr = mem_alloc(size);

    return addr;*/
    return __mem_alloc(size);
}

void operator delete(void* ptr) {
    // mem_free(ptr);
    __mem_free(ptr);
}

void operator delete[](void* ptr) {
    // mem_free(ptr);
    __mem_free(ptr);
}
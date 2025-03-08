#include "../h/syscall_c.hpp"
#include "../lib/hw.h"

void* operator new (const size_t size) {
    return mem_alloc(size);
}

void* operator new[](const size_t size) {
    return mem_alloc(size);
}

void operator delete(void* ptr) {
    mem_free(ptr);
}

void operator delete[](void* ptr) {
    mem_free(ptr);
}
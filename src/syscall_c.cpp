#include "../h/syscall_c.hpp"

#include "../h/memoryAllocator.hpp"

void* mem_alloc(size_t size) {

    return MemoryAllocator::getInstance()->mem_alloc(size);
}

int mem_free(void *ptr) {

    return MemoryAllocator::getInstance()->mem_free(ptr);
}
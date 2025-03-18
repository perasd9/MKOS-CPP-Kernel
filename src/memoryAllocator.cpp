#include "../h/MemoryAllocator.hpp"
#include "../lib/hw.h"

MemoryAllocator* MemoryAllocator::instance = nullptr;


MemoryAllocator* MemoryAllocator::getInstance() {
    if (instance == nullptr) {
        instance = (MemoryAllocator*)HEAP_START_ADDR;

        instance->freeList.head = (Node*)((size_t)HEAP_START_ADDR + sizeof(MemoryAllocator));
        instance->freeList.head->size = (size_t)HEAP_END_ADDR - ((size_t)HEAP_START_ADDR - sizeof(MemoryAllocator));
        instance->freeList.head->next = nullptr;
    }

    return instance;
}

void* MemoryAllocator::mem_alloc(size_t size) {
    instance = getInstance();

    for (const Node* curr = instance->freeList.head; curr != nullptr; curr = curr->next) {
        if (curr->size < size) continue;


    }
}

int MemoryAllocator::mem_free(void *address) {
    
}
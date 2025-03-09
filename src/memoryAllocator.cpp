#include "../h/memoryAllocator.hpp"

MemoryAllocator* MemoryAllocator::instance = nullptr;

void MemoryAllocator::allocateMemory() {
    
}


MemoryAllocator* MemoryAllocator::getInstance() {
    if (instance == nullptr) {
        allocateMemory();
    }

    return instance;
}
#include "../h/MemoryAllocator.hpp"

MemoryAllocator* MemoryAllocator::getInstance() {
    if (instance == nullptr) {
        allocateMemory();
    }

    return instance;
}


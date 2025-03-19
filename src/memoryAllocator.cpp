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

    return nullptr;
}

int MemoryAllocator::mem_free(void *address) {
    instance = getInstance();

    size_t foundSize = -1;
    Node* previous = nullptr;

    for (Node* curr = instance->PCBList.head; curr != nullptr; curr = curr->next) {
        if ((size_t)curr + sizeof(Node) == (size_t)address) {
            foundSize = curr->size;

            if (previous == nullptr)
                instance->PCBList.head = curr->next;
            else
                previous->next = curr->next;

            break;
        }
        previous = curr;
    }

    if (foundSize == (size_t)-1) return -1;

    Node* temp = nullptr;
    if (!instance->freeList.head || (size_t)address < (size_t)instance->freeList.head)
        temp = nullptr;
    else
        for (temp = instance->freeList.head; temp != nullptr && (size_t)address > (size_t)temp->next; temp = temp->next);

    Node* newSeg = (Node*) address;
    newSeg->size = foundSize;

    if (temp != nullptr) {
        newSeg->next = temp->next;
        temp->next = newSeg;
    } else {
        newSeg->next = instance->freeList.head;
        instance->freeList.head = newSeg;
    }

    //left to join segments to avoid fragmentations

    return 0;
}
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
    for (const Node* curr = freeList.head; curr != nullptr; curr = curr->next) {
        if (curr->size < size) continue;


    }

    return nullptr;
}

bool MemoryAllocator::tryJoin(Node* memBlock) {
    if (memBlock == nullptr) return false;

    if (memBlock->next != nullptr && (size_t)memBlock + memBlock->size == (size_t)memBlock->next) {
        memBlock->size += memBlock->next->size + sizeof(Node);
        memBlock->next = memBlock->next->next;

        return true;
    }

    return false;
}

int MemoryAllocator::mem_free(void *address) {
    size_t foundSize = -1;
    Node* previous = nullptr;

    for (Node* curr = PCBList.head; curr != nullptr; curr = curr->next) {
        if ((size_t)curr + sizeof(Node) == (size_t)address) {
            foundSize = curr->size;

            if (previous == nullptr)
                PCBList.head = curr->next;
            else
                previous->next = curr->next;

            break;
        }
        previous = curr;
    }

    if (foundSize == (size_t)-1) return -1;

    Node* temp = nullptr;
    if (!freeList.head || (size_t)address < (size_t)freeList.head)
        temp = nullptr;
    else
        for (temp = freeList.head; temp != nullptr && (size_t)address > (size_t)temp->next; temp = temp->next){}

    auto const newSeg = (Node*) address;
    newSeg->size = foundSize;

    if (temp != nullptr) {
        newSeg->next = temp->next;
        temp->next = newSeg;
    } else {
        newSeg->next = freeList.head;
        freeList.head = newSeg;
    }

    //join current temp segment actually previous and new segment newSeg
    tryJoin(temp);
    tryJoin(newSeg);

    return 0;
}
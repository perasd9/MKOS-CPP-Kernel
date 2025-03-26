#include "../h/memoryAllocator.hpp"
#include "../lib/hw.h"

memoryAllocator* memoryAllocator::instance = nullptr;

memoryAllocator* memoryAllocator::getInstance() {
    if (instance == nullptr) {
        instance = (memoryAllocator*)HEAP_START_ADDR;

        instance->freeList.head = (Node*)((size_t)HEAP_START_ADDR + sizeof(memoryAllocator));
        instance->freeList.head->size = (size_t)HEAP_END_ADDR - ((size_t)HEAP_START_ADDR - sizeof(memoryAllocator));
        instance->freeList.head->next = nullptr;


        instance->PCBList.head = nullptr;
    }

    return instance;
}

void* memoryAllocator::mem_alloc(size_t size) {
    for (Node* curr = freeList.head, *previous = nullptr; curr != nullptr; previous = curr, curr = curr->next) {
        if (curr->size < size) continue;

        if (curr->size - size <= sizeof(Node) + 1) {
            if (previous != nullptr) previous->next = curr->next;
            else freeList.head = curr->next;

            curr->size = size;
            curr->next = nullptr;

            Node* PCBBlock = curr;

            if (PCBList.head == nullptr)
                PCBList.head = PCBBlock;
            else {
                Node* temp = PCBList.head;
                for (; temp->next != nullptr; temp = temp->next){}

                temp->next = PCBBlock;
            }

            return (void*)((size_t)curr + sizeof(Node));
        } else {
            auto newSeg = (Node*)(curr + size + sizeof(Node));

            if (previous != nullptr) previous->next = newSeg;
            else freeList.head = newSeg;

            newSeg->size = curr->size - size;
            newSeg->next = curr->next;

            curr->size = size;
            curr->next = nullptr;

            Node* PCBBlock = curr;

            if (PCBList.head == nullptr)
                PCBList.head = PCBBlock;
            else {
                Node* temp = PCBList.head;
                for (; temp->next != nullptr; temp = temp->next){}

                temp->next = PCBBlock;
            }

            return (void*)((size_t)curr + sizeof(Node));
        }

    }

    return nullptr;
}

bool memoryAllocator::tryJoin(Node* memBlock) {
    if (memBlock == nullptr) return false;

    if (memBlock->next != nullptr && (size_t)memBlock + memBlock->size == (size_t)memBlock->next) {
        memBlock->size += memBlock->next->size + sizeof(Node);
        memBlock->next = memBlock->next->next;

        return true;
    }

    return false;
}

int memoryAllocator::mem_free(void *address) {
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
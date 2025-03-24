#ifndef DEQUE_HPP
#define DEQUE_HPP
#include "MemoryAllocator.hpp"

template<typename T>
class Deque {
private:
    struct Element {
        T* data;
        Element* next;

        Element(T* data, Element* next) : data(data), next(next) {}
    };

    Element *head, *tail;

public:
    Deque() : head(nullptr), tail(nullptr) {}

    void* operator new (const size_t size) {
        return MemoryAllocator::getInstance()->mem_alloc(size);
    }

    void operator delete(void* address) {
        MemoryAllocator::getInstance()->mem_free(address);
    }
};

#endif //DEQUE_HPP

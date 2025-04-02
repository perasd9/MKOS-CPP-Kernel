#ifndef DEQUE_HPP
#define DEQUE_HPP
#include "memoryAllocator.hpp"

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

    void addFirst(T *data) {
        auto elem = (Element*)MemoryAllocator::getInstance()->mem_alloc(sizeof(Element));
        elem->data = data;
        elem->next = head;

        head = elem;
        if (tail == nullptr) tail = head;
    }

    void addLast(T* data) {
        auto elem = (Element*)MemoryAllocator::getInstance()->mem_alloc(sizeof(Element));
        elem->data = data;
        elem->next = nullptr;

        if (tail != nullptr) {
            tail->next = elem;

            tail = elem;
        } else {
            tail = elem;
            head = tail;
        }
    }

    T* removeFirst() {
        if (head == nullptr) return nullptr;

        Element* elem = head;

        head = head->next;

        if (head == nullptr) tail = nullptr;

        T* ret = elem->data;
        MemoryAllocator::getInstance()->mem_free(elem);

        return ret;
    }

    T* peekFirst() {
        return head == nullptr ? nullptr : head->data;
    }

    T* removeLast() {
        if (head == nullptr) return nullptr;

        Element* previous = nullptr;

        for (Element* curr = head; curr != nullptr && curr != tail; curr = curr->next)
            previous = curr;

        Element* elem = tail;

        if (previous != nullptr) previous->next = nullptr;
        else head = nullptr;

        tail = previous;

        T* ret = elem->data;

        MemoryAllocator::getInstance()->mem_free(elem);

        return ret;
    }

    T* peekLast() {
        return tail == nullptr ? nullptr : tail->data;
    }
};

#endif //DEQUE_HPP

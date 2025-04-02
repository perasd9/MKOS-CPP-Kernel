#ifndef MEMORYALLOCATOR_HPP
#define MEMORYALLOCATOR_HPP
#include "linkedList.hpp"

class MemoryAllocator {
public:

    static MemoryAllocator* getInstance();

    void* mem_alloc(size_t size);
    int mem_free(void* address);

private:
    //self-referencing instance
    static MemoryAllocator* instance;

    //private constructor and destructor
    MemoryAllocator(){}
    ~MemoryAllocator()= default;

    static bool tryJoin(Node* memBlock);

    LinkedList freeList;
    LinkedList PCBList;
};

#endif //MEMORYALLOCATOR_HPP

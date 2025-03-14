#ifndef MEMORYALLOCATOR_HPP
#define MEMORYALLOCATOR_HPP

class MemoryAllocator {
public:
    static void allocateMemory();

    static MemoryAllocator* getInstance();

    static void* mem_alloc(size_t size);
    static int mem_free(void* address);


private:
    //self-referencing instance
    static MemoryAllocator* instance;

    //private constructor and destructor
    MemoryAllocator();
    ~MemoryAllocator();
};

#endif //MEMORYALLOCATOR_HPP

#ifndef BUFFER_HPP
#define BUFFER_HPP
#include "../h/syscall_c.hpp"


class Buffer {
private:
    int cap;
    int *buffer;
    int head, tail;

    sem_t spaceAvailable;
    sem_t itemAvailable;
    sem_t mutexHead;
    sem_t mutexTail;

public:
    explicit Buffer(int _cap);
    ~Buffer();

    void put(int val);
    int get();

    int getCnt();
};

#endif //BUFFER_HPP

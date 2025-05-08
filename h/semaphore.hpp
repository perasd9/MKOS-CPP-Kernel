#ifndef SEMAPHORE_HPP
#define SEMAPHORE_HPP

#include "deque.hpp"
#include "thread.hpp"
#include "../lib/hw.h"

class Semaphore {
public:

    explicit Semaphore(uint64 init = 1) : value(init) {}

    int wait();
    void signal();

    static int semOpen(sem_t* handle, unsigned init);
    static int semClose(sem_t handle);

    void block();
    void unblock();

    int getValue() const {
        return value;
    }

private:
    friend class RiscV;
    int value;
    Deque<Thread> blockedThreads;
};

#endif //SEMAPHORE_HPP

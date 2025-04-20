#ifndef SEMAPHORE_HPP
#define SEMAPHORE_HPP

#include "deque.hpp"
#include "thread.hpp"
#include "../lib/hw.h"

class Semaphore {
public:

    explicit Semaphore(uint64 init = 1) : value(init) {

    }

    void wait();
    void signal();

    void block();
    void unblock();

    int getValue() const {
        return value;
    }

private:
    int value;
    Deque<Thread> blockedThreads;
};

#endif //SEMAPHORE_HPP
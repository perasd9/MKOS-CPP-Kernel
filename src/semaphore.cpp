#include "../h/semaphore.hpp"


void Semaphore::block() {
    //set status blocked for running thread, also check is thread finished, if it is we can skip blocking
    //

    blockedThreads.addLast(Thread::running);
    Thread::yield();
}

void Semaphore::wait() {
    if (--value < 0) {
        block();
    }
}

void Semaphore::unblock() {
    //set status ready for unblocked thread

    Scheduler::getInstance()->put(blockedThreads.removeFirst());
}

void Semaphore::signal() {
    if (++value > 0) {
        unblock();
    }
}

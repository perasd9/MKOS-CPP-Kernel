#include "../h/semaphore.hpp"


int Semaphore::semOpen(sem_t* handle, unsigned init = 1) {
    *handle = new Semaphore(init);

    return handle != nullptr ? 0 : -1; //ofc this one always returns 0, for now, here needs to be more checkings
}

int Semaphore::semClose(const sem_t handle) {
    while (handle->blockedThreads.peekFirst() != nullptr) {
        Thread* t = handle->blockedThreads.removeFirst();
        t->setStatus(Thread::READY);
        handle->value++;
        Scheduler::getInstance()->put(t);
    }

    return 0;
}

void Semaphore::block() {
    //set status blocked for running thread, also check is thread finished, if it is we can skip blocking
    blockedThreads.addLast(Thread::running);

    Thread::running->setStatus(Thread::BLOCKED);
    Thread::yield();
}

void Semaphore::unblock() {
    //set status ready for unblocked thread
    if (blockedThreads.peekFirst() != nullptr) {
        Thread* t = blockedThreads.removeFirst();
        t->setStatus(Thread::READY);

        Scheduler::getInstance()->put(t);
    }
}

int Semaphore::wait() {
    if (--value < 0) {
        if (Thread::running->isFinished() == false) {
            block();
        }
    }

    return 0;
}

void Semaphore::signal() {
    if (++value <= 0) {
        unblock();
    }
}

#include "../h/scheduler.hpp"

Scheduler* Scheduler::instance = nullptr;
Deque<Thread> Scheduler::readyThreadQueue;

Scheduler *Scheduler::getInstance() {
    if (instance == nullptr) {
        instance = (Scheduler*)MemoryAllocator::getInstance()->mem_alloc(sizeof(Scheduler));
    }

    return instance;
}

Thread *Scheduler::get() {
    return readyThreadQueue.removeFirst();
}

void Scheduler::put(Thread *ccb) {
    readyThreadQueue.addLast(ccb);
}



#include "../h/scheduler.hpp"

#include "../h/thread.hpp"
#include "../utils/printUtils.hpp"

Scheduler* Scheduler::instance = nullptr;
Deque<Thread> Scheduler::readyThreadQueue = Deque<Thread>();

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



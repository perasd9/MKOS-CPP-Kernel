#include "../h/thread.hpp"
#include "../h/scheduler.hpp"

Thread* Thread::running = nullptr;

Thread *Thread::createThread(Body body) {

    return new Thread(body, nullptr, 10, nullptr);
}


void Thread::yield() {

}

//currently being executed thread need to be changed with new thread from queue of ready threads
void Thread::dispatch() {
    Thread* old = running;

    if (old->is_finished() == false) {
        Scheduler::getInstance()->put(old);
    }

    running = Scheduler::getInstance()->get();

    contextSwitch(&old->ctx, &running->ctx);
}


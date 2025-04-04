#include "../h/thread.hpp"
#include "../h/scheduler.hpp"

Thread* Thread::running = nullptr;
uint64 Thread::timeSliceCounter = 0;

Thread *Thread::createThread(Body body) {

    return new Thread(body, nullptr, DEFAULT_TIME_SLICE, nullptr);
}


void Thread::yield() {

    //need to be pushed registers on currently stack pointer of thread but i will omit that probably

    //actually context switching
    dispatch();

    //popped registers for taking back at the return of last thread or coroutine for now
    //coroutine is just independently control flow but that all will be thread
    //that's the case why i named this thread
}

//currently being executed thread need to be changed with new thread from queue of ready threads
void Thread::dispatch() {
    Thread* old = running;

    if (old->is_finished() == false) {
        Scheduler::getInstance()->put(old);
    }

    running = Scheduler::getInstance()->get();

    //actual context switching registers between 2 active threads
    contextSwitch(&old->ctx, &running->ctx);
}


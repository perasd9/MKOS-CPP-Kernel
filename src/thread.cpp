#include "../h/thread.hpp"
#include "../h/scheduler.hpp"

Thread* Thread::running = nullptr;
uint64 Thread::timeSliceCounter = 0;

Thread *Thread::createThread(Body body) {

    return new Thread(body, nullptr, DEFAULT_TIME_SLICE, nullptr);
}


void Thread::yield() {

    //this is won't be finally solution but for now it works if you have assembly defined method for pushing and popping regs
    //ecall will be called with specific code
  
    //push registers
    pushRegisters();

    //dispatching context
    dispatch();

   //popping registers
   popRegisters();
    
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

void Thread::threadWrapper() {
    running->body(running->arg);
    running->set_finished(true);
}
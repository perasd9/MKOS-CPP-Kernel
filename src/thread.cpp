#include "../h/thread.hpp"

#include "../h/riscV.hpp"
#include "../h/scheduler.hpp"
#include "../h/syscall_c.hpp"
#include "../utils/printUtils.hpp"

Thread *Thread::running = nullptr;
uint64 Thread::timeSliceCounter = 0;

int Thread::createThread(/* Thread** */ thread_t *handle, const Body body, void *arg, uint64 *stack) {
    *handle = new Thread(body, DEFAULT_TIME_SLICE, arg);

    (*handle)->stack = body != nullptr ? stack : nullptr;

    (*handle)->ctx.sp = (*handle)->stack != nullptr
                            ? reinterpret_cast<uint64>(&(*handle)->stack[DEFAULT_STACK_SIZE]) : 0;
    (*handle)->ctx.ra = reinterpret_cast<uint64>(&threadWrapper);

    if (*(*handle)->body != nullptr) {
        //stack need to be initialized by c or cpp api, only then ABI will call constructor for other fields
        // this->stack = new uint64[DEFAULT_STACK_SIZE];

        (*handle)->status = READY;
        Scheduler::getInstance()->put(*handle);
    }

    return *handle != nullptr ? 0 : -1;
}

void Thread::yield() {
    uint64 a0;

    __asm__ volatile("mv %0, a0" : "=r" (a0));

    uint64 interruptCode = 0x92;
    __asm__ volatile("mv a0, %0" : : "r" (interruptCode));
    __asm__ volatile("ecall");

    __asm__ volatile("mv a0, %0" : : "r" (a0));
}

//currently being executed thread need to be changed with new thread from queue of ready threads
void Thread::dispatch() {
    Thread *old = running;

    if (old->isFinished() == false && old->status != BLOCKED) {
        old->status = READY;
        Scheduler::getInstance()->put(old);
    }

    running = Scheduler::getInstance()->get();
    running->status = RUNNING;

    //actual context switching registers between 2 active threads
    contextSwitch(&old->ctx, &running->ctx);
}

void Thread::threadWrapper() {
    // RiscV::popSppSpie();
    running->body(running->arg);
    running->setFinished(true);
    yield();
}

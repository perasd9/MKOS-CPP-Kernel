#include "../h/thread.hpp"
#include "../h/scheduler.hpp"
#include "../utils/printUtils.hpp"

Thread* Thread::running = nullptr;
uint64 Thread::timeSliceCounter = 0;

Thread *Thread::createThread(const Body body, void* arg) { return new Thread(body, nullptr, DEFAULT_TIME_SLICE, arg); }

void Thread::yield() {
    uint64 a0;

    __asm__ volatile("mv %0, a0" : "=r" (a0));

    uint64 interruptCode = 0x09UL;
    __asm__ volatile("mv a0, %0" : : "r" (interruptCode));
    __asm__ volatile("ecall");

    __asm__ volatile("mv a0, %0" : : "r" (a0));
}

//currently being executed thread need to be changed with new thread from queue of ready threads
void Thread::dispatch() {
    Thread* old = running;

    if (old->isFinished() == false) {
        Scheduler::getInstance()->put(old);
    }

    running = Scheduler::getInstance()->get();

    char s[] = "HELO FROM DISPATCH \n";


    printString(s);

    printInt(old->ctx.ra);  // Ispisivanje RA iz starog konteksta
    printInt(old->ctx.sp);  // Ispisivanje SP iz starog konteksta

    printInt(running->ctx.ra);  // Ispisivanje RA iz novog konteksta
    printInt(running->ctx.sp);  // Ispisivanje SP iz novog konteksta

    //actual context switching registers between 2 active threads
    contextSwitch(&old->ctx, &running->ctx);
}

void Thread::threadWrapper() {
    char s[] = "HELO FROM WRAPPER \n"; printString(s);
    running->body(running->arg);
    running->setFinished(true);
    yield();
}
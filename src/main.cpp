#include "../h/riscV.hpp"
#include "../h/thread.hpp"
#include "../lib/console.h"
#include "../lib/hw.h"
#include "../utils/printUtils.hpp"

class Thread;
uint64 saved;

void f() {
    __putc('p');
    __putc('\n');

    __asm__ volatile("mv ra, %[retAddress]" : : [retAddress] "r" (saved));
    __asm__ volatile("sd ra, 8(sp)");
}

void dispatch() {
    __asm__ volatile("mv %[savedReg], ra" : [savedReg] "=r" (saved));
    __asm__ volatile("mv ra, %[fAddress]" : : [fAddress] "r" (&f));
}

void workerA(void * par) {
    char s[] = "WORKERAAAA";

    printString(s);
}


void workerB(void * par) {
    char s[] = "WORKERBBBB";

    printString(s);
}

void main() {
     //dispatch cannot be tested with thread execution
     //dispatch();
     char s[] = "\n";
     char num[] = "999";
     char buf[3];
     printString((char *)&HEAP_START_ADDR);
     printInt((int)(size_t)HEAP_START_ADDR);
     printString(s);
     printInt((int)(size_t)HEAP_END_ADDR);
     printInt(stringToInt(num));
    
     getString(buf, 3);
     printString(buf);

     RiscV::write_stvec((size_t)&RiscV::supervisorTrap);
     __asm__ volatile("ecall");

    //Thread* threads[3];

    //threads[0] = Thread::createThread(nullptr);

    //Thread::running = threads[0];

    //threads[1] = Thread::createThread(workerA);

    //threads[2] = Thread::createThread(workerB);

   // while (threads[1]->is_finished() == false && threads[2]->is_finished() == false)
       // Thread::yield();

    //for (auto &thread : threads)
        //delete thread;
}

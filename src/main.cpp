#include "../h/riscV.hpp"
#include "../h/syscall_c.hpp"
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

void workerBodyAA(void* arg) {
    char* s;

    for (uint64 i = 0; i < 10; i++) {
        s = (char*)"A: i=";
        printString(s); printInt(i); s = (char*)"\n"; printString(s);
        for (uint64 j = 0; j < 10000; j++) {
            for (uint64 k = 0; k < 30000; k++) { /* busy wait */ }
            thread_dispatch();
        }
    }
    s = (char*)"A finished\n";
    printString(s);
}

void workerBodyBB(void* arg) {
    char* s;
    for (uint64 i = 0; i < 16; i++) {
        s = (char*)"B: i=";
        printString(s); printInt(i); s = (char*)"\n"; printString(s);
        for (uint64 j = 0; j < 10000; j++) {
            for (uint64 k = 0; k < 30000; k++) { /* busy wait */ }
            thread_dispatch();
        }
    }
    s = (char*)"B finished!\n"; printString(s);
    thread_dispatch();
}

extern void userMain(void*);

void main() {
    RiscV::write_stvec((uint64)&RiscV::supervisorTrap);
    RiscV::mset_sstatus(RiscV::SIE);

    Thread* t;

    thread_create(&t, nullptr, nullptr);
    Thread::running = t;


    userMain(nullptr);
}

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

extern void userMain(void*);

void main() {
    RiscV::write_stvec((uint64)&RiscV::supervisorTrap);
    RiscV::mset_sstatus(RiscV::SIE);

    Thread* t;

    thread_create(&t, nullptr, nullptr);
    Thread::running = t;

    userMain(nullptr);
}
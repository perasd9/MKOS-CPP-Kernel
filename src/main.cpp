#include "../lib/console.h"
#include "../lib/hw.h"

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

void main() {
    dispatch();
}
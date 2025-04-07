#include "../h/riscV.hpp"

#include "../h/thread.hpp"
#include "../lib/console.h"
#include "../utils/printUtils.hpp"

void RiscV::popSppSpie() {

}

void RiscV::handleSupervisorTrap() {
    uint64 scause = read_scause();


    if (scause == 0x8000000000000001UL) { //interrupt, cause supervisor software interrupt

        Thread::timeSliceCounter++;

        if (Thread::timeSliceCounter >= Thread::running->get_time_slice()) {
            uint64 sepc = read_sepc();
            uint64 sstatus = read_sstatus();

            Thread::timeSliceCounter = 0;
            Thread::dispatch();

            write_sepc(sepc);
            write_sstatus(sstatus);
        }

        mclear_sip(SSIP);
    } else if (scause == 0x8000000000000009UL) { //interrupt, cause supervisor external interrupt(console)
        console_handler();
    } else { //unexpected trap cause
        printInt((int)scause);
        printInt((int)read_sepc());
        printInt((int)read_stval());
    }
}
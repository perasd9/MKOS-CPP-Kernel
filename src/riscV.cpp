#include "../h/riscV.hpp"

#include "../h/thread.hpp"
#include "../lib/console.h"
#include "../utils/printUtils.hpp"

void RiscV::popSppSpie() {
    __asm__ volatile("csrw sepc, ra");
    mclear_sstatus(SPP);
    __asm__ volatile("sret");
}

void RiscV::handleSupervisorTrap() {
    uint64 scause = read_scause();

    uint64 a0;

    __asm__ volatile("mv %0, a0" : "=r" (a0));

    if (scause == 0x8000000000000001UL) { //interrupt, cause supervisor software interrupt
        mclear_sip(SSIP);
    } else if (scause == 0x8000000000000009UL) { //interrupt, cause supervisor external interrupt(console)
        console_handler();
    } else if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL) {
        //not interrupt, cause is environment call from user mode of supervisor mode
        printInt(a0);

        switch(a0){
            case 0x09:

                uint64 sepc = read_sepc() + 4;
                uint64 sstatus = read_sstatus();

                Thread::timeSliceCounter = 0;
                Thread::dispatch();

                write_sepc(sepc);
                write_sstatus(sstatus);
                break;

            /*case 0x01:

                break;

            case 0x02:

                break;

            case 0x11:

                break;

            case 0x12:

                break;

            case 0x13:

                break;

            case 0x21:

                break;

            case 0x22:

                break;

            case 0x23:

                break;

            case 0x24:

                break;

            case 0x25:

                break;

            case 0x26:

                break;

            case 0x31:

                break;

            case 0x41:

                break;

            case 0x42:

                break;*/

        }
    } else { //unexpected trap cause
        printInt((int)scause);
        printInt((int)read_sepc());
        printInt((int)read_stval());
    }
}
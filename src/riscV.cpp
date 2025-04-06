#include "../h/riscV.hpp"

#include "../lib/console.h"
#include "../utils/printUtils.hpp"

void RiscV::handleSupervisorTrap() {
    uint64 scause = read_scause();


    if (scause == 0x8000000000000001UL) { //interrupt, cause supervisor software interrupt
        mclear_sip(SSIP);
    } else if (scause == 0x8000000000000009UL) { //interrupt, cause supervisor external interrupt(console)
        console_handler();
    } else { //unexpected trap cause
        printInt((int)scause);
        printInt((int)read_sepc());
        printInt((int)read_stval());
    }
}


#ifndef RISCV_HPP
#define RISCV_HPP

#include "../lib/hw.h"

class RiscV {
public:
    bool privilegeMode;

    //supervisor exception pc
    static uint64 read_sepc();
    static void write_sepc(uint64 sepc);

    //supervisor cause
    static uint64 read_scause();
    static void write_scause(uint64 scause);

    //supervisor trap vector
    static uint64 read_stvec();
    static void write_stvec(uint64 stvec);

    //supervisor trap value
    static uint64 read_stval();
    static void write_stval(uint64 stvec);

    //left bitmask for clearing and setting bits in sip and status

    //supervisor interrupt pending
    static uint64 read_sip();
    static void write_sip(uint64 sip);

    //supervisor status
    static uint64 read_sstatus();
    static void write_sstatus(uint64 sstatus);


private:

};

#endif //RISCV_HPP
#ifndef RISCV_HPP
#define RISCV_HPP

#include "../lib/hw.h"

class RiscV {
public:
    static RiscV* getInstance();
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

    //setting 1. 5. and 9. bit
    enum SipBitMask {
        SSIP = (1 << 1),
        STIP = (1 << 5),
        SEIP = (1 << 9)
    };

    //supervisor interrupt pending
    static uint64 read_sip();
    static void write_sip(uint64 sip);

    //setting 1. 5. and 8. bit
    enum StatusBitMask {
        SIE = (1 << 1),
        SPIE = (1 << 5),
        SPP = (1 << 8)
    };

    //supervisor status
    static uint64 read_sstatus();
    static void write_sstatus(uint64 sstatus);

private:
    static RiscV* instance;

    RiscV();
    ~RiscV();
};

#endif //RISCV_HPP
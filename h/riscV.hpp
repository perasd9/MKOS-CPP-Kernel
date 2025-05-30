#ifndef RISCV_HPP
#define RISCV_HPP

#include "../lib/hw.h"

class RiscV {
public:
    bool privilegeMode;


    //pop sstatus.spp and sstatus.spie bits
    static void popSppSpie();

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
    static void write_stval(uint64 stval);

    //setting 1. 5. and 9. bit
    enum SipBitMask {
        SSIP = (1 << 1),
        STIP = (1 << 5),
        SEIP = (1 << 9)
    };

    //supervisor interrupt pending
    static uint64 read_sip();
    static void write_sip(uint64 sip);

    static void mset_sip(uint64 mask);
    static void mclear_sip(uint64 mask);

    //setting 1. 5. and 8. bit
    enum StatusBitMask {
        SIE = (1 << 1),
        SPIE = (1 << 5),
        SPP = (1 << 8)
    };

    //supervisor status
    static uint64 read_sstatus();
    static void write_sstatus(uint64 sstatus);

    static void mset_sstatus(uint64 mask);
    static void mclear_sstatus(uint64 mask);


    //interrupt call in assembly
    //warning for not implemented in cpp file but this is implemented in assembly language in .S files
    static void supervisorTrap();
    static void consoleTrap();

private:
    static void handleSupervisorTrap();
    static void handleConsoleTrap();
};

//supervisor exception pc
inline uint64 RiscV::read_sepc() {
    uint64 sepc;

    __asm__ volatile("csrr %[sepcPar], sepc" : [sepcPar] "=r" (sepc));

    return sepc;
}

inline void RiscV::write_sepc(uint64 sepc) {
    __asm__ volatile("csrw sepc, %[sepcPar]" : : [sepcPar] "r" (sepc));
}

//supervisor cause
inline uint64 RiscV::read_scause() {
    uint64 scause;

    __asm__ volatile("csrr %[scausePar], scause" : [scausePar] "=r" (scause));

    return scause;
}

inline void RiscV::write_scause(uint64 scause) {
    __asm__ volatile("csrw scause, %[scausePar]" : : [scausePar] "r" (scause));
}

//supervisor trap vector
inline uint64 RiscV::read_stvec() {
    uint64 stvec;

    __asm__ volatile("csrr %[stvecPar], stvec" : [stvecPar] "=r" (stvec));

    return stvec;
}

inline void RiscV::write_stvec(uint64 stvec) {
    __asm__ volatile("csrw stvec, %[stvecPar]" : : [stvecPar] "r" (stvec));
}

//supervisor trap value
inline uint64 RiscV::read_stval() {
    uint64 stval;

    __asm__ volatile("csrr %[stvalPar], stval" : [stvalPar] "=r" (stval));

    return stval;
}

inline void RiscV::write_stval(uint64 stval) {
    __asm__ volatile("csrw stval, %[stvalPar]" : : [stvalPar] "r" (stval));
}

//supervisor interrupt pending
inline uint64 RiscV::read_sip() {
    uint64 sip;

    __asm__ volatile("csrr %[sipPar], sip" : [sipPar] "=r" (sip));

    return sip;
}

inline void RiscV::write_sip(uint64 sip) {
    __asm__ volatile("csrw sip, %[sipPar]" : : [sipPar] "r" (sip));
}

inline void RiscV::mset_sip(uint64 mask) {
    __asm__ volatile("csrs sip, %[maskPar]" : : [maskPar] "r" (mask));
}

inline void RiscV::mclear_sip(uint64 mask) {
    __asm__ volatile("csrc sip, %[maskPar]" : : [maskPar] "r" (mask));
}

//supervisor status
inline uint64 RiscV::read_sstatus() {
    uint64 sstatus;

    __asm__ volatile("csrr %[sstatusPar], sstatus" : [sstatusPar] "=r" (sstatus));

    return sstatus;
}

inline void RiscV::write_sstatus(uint64 sstatus) {
    __asm__ volatile("csrw sstatus, %[sstatusPar]" : : [sstatusPar] "r" (sstatus));
}

inline void RiscV::mset_sstatus(uint64 mask) {
    __asm__ volatile("csrs sstatus, %[maskPar]" : : [maskPar] "r" (mask));
}

inline void RiscV::mclear_sstatus(uint64 mask) {
    __asm__ volatile("csrc sstatus, %[maskPar]" : : [maskPar] "r" (mask));
}


#endif //RISCV_HPP

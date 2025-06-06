#include "../h/riscV.hpp"

#include "../h/semaphore.hpp"
#include "../h/thread.hpp"
#include "../lib/console.h"
#include "../utils/printUtils.hpp"

void RiscV::popSppSpie() {
    __asm__ volatile("csrw sepc, ra");
    mclear_sstatus(SPP);
    __asm__ volatile("sret");
}

void RiscV::handleSupervisorTrap() {
    uint64 a0;
    uint64 a1;

    __asm__ volatile("mv %0, a0" : "=r" (a0));
    uint64 scause = read_scause();


    if (scause == 0x8000000000000001UL) {
        //interrupt, cause supervisor software interrupt
        mclear_sip(SSIP);
    } else if (scause == 0x8000000000000009UL) {
        //interrupt, cause supervisor external interrupt(console)
        console_handler();
    } else if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL) {
        //not interrupt, cause is environment call from user mode of supervisor mode

        if (a0 == 0x92) {
            uint64 sepc = read_sepc() + 4;
            uint64 sstatus = read_sstatus();

            Thread::timeSliceCounter = 0;
            Thread::dispatch();

            write_sepc(sepc);
            write_sstatus(sstatus);
        }
        else if (a0 == 0x01) { //mem_alloc code sys call
            uint64 sepc = read_sepc() + 4;
            uint64 sstatus = read_sstatus();

            __asm__ volatile("mv %0, x11" : "=r" (a1));

            // __asm__ volatile("ld a0, 11*8(fp)");
            // __asm__ volatile("mv %0, a0" : "=r" (a1));

            uint64 addr;
            addr = (uint64) MemoryAllocator::getInstance()->mem_alloc(a1 * MEM_BLOCK_SIZE);

            // __asm__ volatile("mv a0, %0" : : "r" (addr));
            __asm__ volatile("sd %0, 10*8(fp)" : : "r" (addr));

            write_sepc(sepc);
            write_sstatus(sstatus);
        }
        else if (a0 == 0x02) { //mem_free code sys call
            uint64 sepc = read_sepc() + 4;
            uint64 sstatus = read_sstatus();

            __asm__ volatile("mv %0, x11" : "=r" (a1));
            int ret = MemoryAllocator::getInstance()->mem_free((void *) a1);

            __asm__ volatile("sd %0, 10*8(fp)" : : "r" (ret));

            write_sepc(sepc);
            write_sstatus(sstatus);
        }
        else if (a0 == 0x11) { //thread_create code sys call
            uint64 sepc = read_sepc() + 4;
            uint64 sstatus = read_sstatus();

            thread_t *arg1; //handle pointer (handle)
            Thread::Body arg2; //start routine (start_routine)
            uint64 arg3; //start routine arguments (arg)
            uint64 *arg4; //stack initialized by c or cpp api

            __asm__ volatile("mv %0, x11" : "=r"(arg1));
            __asm__ volatile("mv %0, x12" : "=r"(arg2));
            __asm__ volatile("mv %0, t5" : "=r"(arg3));
            __asm__ volatile("mv %0, t6" : "=r"(arg4));

            int statusCode = Thread::createThread(arg1, arg2, reinterpret_cast<void *>(arg3), arg4);

            __asm__ volatile("sd %0, 10*8(fp)" : : "r"(statusCode));

            write_sepc(sepc);
            write_sstatus(sstatus);
        }
        else if (a0 == 0x12) { //thread_exit code sys call
            uint64 sepc = read_sepc() + 4;
            uint64 sstatus = read_sstatus();

            Thread::running->setFinished(true);
            Thread::dispatch();

            write_sepc(sepc);
            write_sstatus(sstatus);
        }
        else if (a0 == 0x13) { //thread_dispatch code sys call
            uint64 sepc = read_sepc() + 4;
            uint64 sstatus = read_sstatus();

            Thread::dispatch();

            write_sepc(sepc);
            write_sstatus(sstatus);
        } else if (a0 == 0x21) { //semaphore open sys call
            uint64 sepc = read_sepc() + 4;
            uint64 sstatus = read_sstatus();

            sem_t *arg1; //handle pointer (handle)
            unsigned arg2; //semaphore value

            __asm__ volatile("mv %0, x11" : "=r"(arg1));
            __asm__ volatile("mv %0, x12" : "=r"(arg2));

            int statusCode = Semaphore::semOpen(arg1, arg2);

            __asm__ volatile("sd %0, 10*8(fp)" : : "r"(statusCode));

            write_sepc(sepc);
            write_sstatus(sstatus);
        } else if (a0 == 0x22) { //semaphore close sys call
            uint64 sepc = read_sepc() + 4;
            uint64 sstatus = read_sstatus();

            Semaphore* arg1;
            __asm__ volatile("mv %0, x11" : "=r"(arg1));

            //closing semaphore
            Semaphore::semClose(arg1);

            //free all memory allocated
            delete arg1;

            write_sepc(sepc);
            write_sstatus(sstatus);
        } else if (a0 == 0x23) { //semaphore wait sys call
            uint64 sepc = read_sepc() + 4;
            uint64 sstatus = read_sstatus();

            Semaphore* arg1;
            __asm__ volatile("mv %0, x11" : "=r"(arg1));

            int statusCode = arg1->wait();

            __asm__ volatile("sd %0, 10*8(fp)" : : "r"(statusCode));

            write_sepc(sepc);
            write_sstatus(sstatus);
        } else if (a0 == 0x24) { //semaphore signal sys call
            uint64 sepc = read_sepc() + 4;
            uint64 sstatus = read_sstatus();

            Semaphore* arg1;
            __asm__ volatile("mv %0, x11" : "=r"(arg1));

            arg1->signal();

            write_sepc(sepc);
            write_sstatus(sstatus);
        } else if (a0 == 0x41) { //get char sys call
            uint64 sepc = read_sepc() + 4;
            uint64 sstatus = read_sstatus();

            RiscV::write_stvec((uint64)&RiscV::consoleTrap);
            char c = __getc();
            RiscV::write_stvec((uint64)&RiscV::supervisorTrap);

            __asm__ volatile("sd %0, 10*8(fp)" : : "r" (c));

            write_sepc(sepc);
            write_sstatus(sstatus);
        } else if (a0 == 0x42) { //put char sys call
            uint64 sepc = read_sepc() + 4;
            uint64 sstatus = read_sstatus();

            __asm__ volatile("mv %0, x11" : "=r" (a1));

            RiscV::write_stvec((uint64)&RiscV::consoleTrap);
            __putc(a1);
            RiscV::write_stvec((uint64)&RiscV::supervisorTrap);

            write_sepc(sepc);
            write_sstatus(sstatus);
        } else {
            printInt(a0);
        }

    } else {
        //unexpected trap cause
        char s[] = "\n";
        char s1[] = "  ";
        printInt(scause);
        printString(s1);
        printInt(read_sepc());
        printString(s1);
        printInt(read_stval());
        printString(s);
    }

}

void RiscV::handleConsoleTrap() {
    uint64 scause = read_scause();
    if (scause == 0x8000000000000009UL) {
        console_handler();
        mclear_sip(SEIP);
    } else if (scause == 0x8000000000000001UL) {
        mclear_sip(SSIP);
    }

}

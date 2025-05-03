#include "../h/syscall_c.hpp"

#include "../h/memoryAllocator.hpp"
#include "../h/thread.hpp"
#include "../utils/printUtils.hpp"

//memory management system calls
void* mem_alloc(size_t size) {
    uint64 blocks = size % MEM_BLOCK_SIZE != 0 ? (size / MEM_BLOCK_SIZE) + 1 : (size / MEM_BLOCK_SIZE);
    uint64 code = 0x01;

    __asm__ volatile("mv a0, %0" : : "r" (code));
    __asm__ volatile("mv a1, %0" : : "r" (blocks));

    __asm__ volatile("ecall");

    uint64 retAddress;
    __asm__ volatile("mv %0, a0" : "=r" (retAddress));


    return (void*)retAddress;
}

int mem_free(void *ptr) {
    __asm__ volatile("mv a0, %0" : : "r" (0x02));
    __asm__ volatile("mv a1, %0" : : "r" (ptr));

    __asm__ volatile("ecall");

    int statusCode;

    __asm__ volatile("mv %0, a0" : "=r" (statusCode));

    return statusCode;
}


//thread management system calls
int thread_create(thread_t* handle, void(*start_routine)(void *), void *arg) {
    auto stack = new uint64[DEFAULT_STACK_SIZE];
    __asm__ volatile("mv a0, %0" : : "r"(stack));
    __asm__ volatile("mv t6, a0");
    __asm__ volatile("mv a0, %0" : : "r"(0x11));
    __asm__ volatile("mv a1, %0" : : "r"(handle));
    __asm__ volatile("mv a2, %0" : : "r"(start_routine));
    __asm__ volatile("mv a3, %0" : : "r"(arg));
    // __asm__ volatile("mv a4, %0" : : "r"(stack));

    __asm__ volatile("ecall");

    int statusCode;
    __asm__ volatile("mv %0, a0" : "=r"(statusCode));

    return statusCode;
}

int thread_exit() {
    __asm__ volatile("mv a0, %0" : : "r" (0x12));

    __asm__ volatile("ecall");

    int statusCode;

    __asm__ volatile("mv %0, a0" : "=r" (statusCode));

    return statusCode;
}

void thread_dispatch() {
    __asm__ volatile("mv a0, %0" : : "r"(0x13));

    __asm__ volatile("ecall");
}


//semaphore management system calls
int sem_open(sem_t*handle, unsigned init) {
    __asm__ volatile("mv a0, %0" : : "r"(0x21));
    __asm__ volatile("mv a1, %0" : : "r"(handle));
    __asm__ volatile("mv a2, %0" : : "r"(init));

    __asm__ volatile("ecall");

    int statusCode;

    __asm__ volatile("mv %0, a0" : "=r" (statusCode));

    return statusCode;
}

int sem_close(sem_t handle) {
    __asm__ volatile("mv a0, %0" : : "r"(0x22));
    __asm__ volatile("mv a1, %0" : : "r"(handle));

    __asm__ volatile("ecall");

    return 0;
}

int sem_wait(sem_t id) {
    __asm__ volatile("mv a0, %0" : : "r"(0x23));
    __asm__ volatile("mv a1, %0" : : "r"(id));

    __asm__ volatile("ecall");

    int statusCode;

    __asm__ volatile("mv %0, a0" : "=r" (statusCode));

    return statusCode;
}

int sem_signal(sem_t id) {
    __asm__ volatile("mv a0, %0" : : "r"(0x24));
    __asm__ volatile("mv a1, %0" : : "r"(id));

    __asm__ volatile("ecall");

    return 0;
}


//console management system calls
char getc() {
    __asm__ volatile("mv a0, %0" : : "r"(0x41));

    __asm__ volatile("ecall");

    char c;

    __asm__ volatile("mv %0, a0" : "=r" (c));

    return c;
}

void putc(char c) {
    __asm__ volatile("mv a0, %0" : : "r"(0x42));
    __asm__ volatile("mv a1, %0" : : "r"(c));

    __asm__ volatile("ecall");
}
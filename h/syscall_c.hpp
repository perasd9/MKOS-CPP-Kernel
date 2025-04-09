#include "../lib/hw.h"

#ifndef SYSCALL_C_HPP
#define SYSCALL_C_HPP

// memory management functions
void* mem_alloc(size_t size);

int mem_free(void* ptr);

//thread syscalls c
class Thread;
typedef Thread* thread_t;

int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg);

int thread_exit();

void thread_dispatch();

//semaphore syscalls c need to be filled later

//console syscalls c for getc and putc need to be filled later

#endif //SYSCALL_C_HPP
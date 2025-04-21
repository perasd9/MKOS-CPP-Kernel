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

//semaphore syscalls
class Semaphore;
typedef Semaphore* sem_t;

int sem_open(sem_t* handle, unsigned init);

int sem_close(sem_t handle);

int sem_wait(sem_t id);

int sem_signal(sem_t id);

//console syscalls c
const int EOF = -1;

char getc();

void putc(char);

#endif //SYSCALL_C_HPP

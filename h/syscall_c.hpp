#include "../lib/hw.h"

#ifndef SYSCALL_C_HPP
#define SYSCALL_C_HPP

// memory management functions
void mem_alloc(size_t size);
int mem_free(void* ptr);

#endif //SYSCALL_C_HPP

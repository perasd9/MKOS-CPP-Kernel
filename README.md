# MKOS-CPP-Kernel

This is an implementation of a small operating system kernel that supports threads (a multithreaded operating system) with time sharing. This project implements a memory allocator and thread management. The kernel should provide the concept of threads, semaphores, and support for time sharing, as well as asynchronous context switching and preemption triggered by timer and keyboard interrupts. The concurrent processes created within the application will actually be "lightweight" processes(threads) running within the same program. The kernel is implemented for the RISC-V processor architecture.

## Interface Of Kernel

The kernel code and the hardware access modules (software below the dark thick line) execute in the privileged (system) mode of the target processor, while the layers above the kernel execute in the unprivileged (user) mode. The kernel will initially be launched in system mode.

<div align = "center">
    <img src = "https://github.com/user-attachments/assets/7a9e7713-889c-4dfe-b80e-1a2112ebfc6f">
</div>

- **The ABI (Application Binary Interface)** is the binary interface for system calls, which are performed using software interrupts of the target processor. This layer provides the transfer of system call arguments via processor registers, the transition to privileged processor mode, and the jump to kernel code. System call, exception, and interrupt handling should be implemented using direct mode.

- **The C API** is a classic, procedural (non-object-oriented) programming interface for system calls, implemented as a set of functions. These functions may internally use one or more system calls from the ABI layer, or none at all, depending on their role. Therefore, these functions effectively serve as wrappers around the ABI interface.

- **The C++ API** is an object-oriented API that provides an object-based view of the concepts supported by the kernel. It is implemented as a simple object-oriented wrapper around the C API functions, written in C++.

### ABI

A system call in this layer is performed via a software interrupt (using the appropriate processor instruction). The parameters are passed to the system call through the processor registers.
All signatures of the listed system calls from the C API are fully mapped to the corresponding ABI calls according to the specified convention and share the similar semantics.

### C API

The functions of this interface are described in the following table, and their declarations are provided in the file `syscall_c.hpp`.

- **0x01 | void\* mem_alloc(size_t size);** Allocates a memory area of (at least) size bytes, rounded up and aligned to blocks of size `MEM_BLOCK_SIZE`. On success, it returns a pointer to a portion of the allocated space. On failure, it returns null. `MEM_BLOCK_SIZE` is an integer constant greater than or equal to 64 and less than or equal to 1024.
- **0x02 | int mem_free(void\*);** Frees the memory space previously allocated using `mem_alloc`. Returns 0 on success, or a negative value in case of an error (error code). The argument must be a value returned by `mem_alloc`.
- **0x11 | int thread_create(thread_t\* handle, void(\*start_routine)(void\*), void\* arg);** Starts a thread by invoking the function `start_routine`, passing arg as its argument. On success, it writes the handle of the newly created thread to \*handle and returns 0. On failure, it returns a negative value (error code).
- **0x12 | int thread_exit();** Terminates the current thread. On failure, it returns a negative value (error code).
- **0x13 | void thread_dispatch();** Potentially takes the processor away from the current thread and gives it to another (or the same) thread.
- **0x21 | int sem_open(sem_t\* handle, unsigned init);** Creates a semaphore with the initial value `init`. On success, it writes the handle of the newly created semaphore to \*handle and returns 0. On failure, it returns a negative value (error code).
- **0x22 | int sem_close(sem_t handle);** Releases the semaphore with the given handle. All threads that were waiting on the semaphore are unblocked, and their wait calls return an error. On success, it returns 0; on failure, it returns a negative value (error code).
- **0x23 | int sem_wait(sem_t id);** Performs a `wait` operation on the semaphore with the given handle. On success, it returns 0. On failure-including the case where the semaphore was deallocated while the calling thread was waiting on it, returns a negative value (error code).
- **0x24 | int sem_signal(sem_t id);** Performs a `signal` operation on the semaphore with the given handle. On success, it returns 0; on failure, it returns a negative value (error code).
- **0x41 | char getc();** Reads a single character from the buffer of characters read from the console. If the buffer is empty, it suspends the calling thread until a character becomes available. Returns the read character on success, or the constant EOF in case of an error.
- **0x42 | void putc(char);** Writes forwarded char on console.

### CPP API

The classes of this interface are defined in the file `syscall_cpp.hpp`.

## Key Abstractions

- **MemoryAllocator:** A singleton class that provides memory allocation and deallocation services using continuous allocation, as described in the requirements. Alghorithm used in this project is algorithm of continual allocation concrete `first-fit`.
- **Thread**: A class that abstracts a thread and stores its context along with other necessary attributes.
- **Scheduler:** A singleton class that implements the process scheduler, the scheduling algorithm. In this project concrete was used the simplest one alghorithm, FCFS(First Come First Serve, implemented by FIFO queue).
- **Semaphore:** A class that abstracts a semaphore and the operations performed on it.
- **Console:** A singleton class that implements the interface to the console.

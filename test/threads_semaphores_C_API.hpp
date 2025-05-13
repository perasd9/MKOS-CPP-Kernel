#ifndef THREADS_SEMAPHORES_C_API_HPP
#define THREADS_SEMAPHORES_C_API_HPP

#include "../h/syscall_c.hpp"

#include "../utils/printUtils.hpp"
#include"../h/semaphore.hpp"
#include "../h/thread.hpp"

bool finishedA = false;
bool finishedB = false;
bool finishedC = false;
bool finishedD = false;

sem_t mutex;

void createMutex(){
    char* s;

    int a = sem_open(&mutex, 0);
    if(!a){
        s = (char*)"Mutex created\n";
        printString(s);
    }
}

uint64 fibonacci(uint64 n) {
    if (n == 0 || n == 1) { return n; }
    if (n % 10 == 0) { thread_dispatch(); }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

void workerBodyA(void* arg) {
    char* s;

    for (uint64 i = 0; i < 10; i++) {
        s = (char*)"A: i=";
        printString(s); printInt(i); s = (char*)"\n"; printString(s);
        for (uint64 j = 0; j < 10000; j++) {
            for (uint64 k = 0; k < 30000; k++) { /* busy wait */ }
            thread_dispatch();
        }
    }

    s = (char*)"A finished\n";
    printString(s);
    finishedA = true;
}

void workerBodyB(void* arg) {
    char* s;

    for (uint64 i = 0; i < 16; i++) {
        s = (char*)"B: i=";
        printString(s); printInt(i); s = (char*)"\n"; printString(s);
        for (uint64 j = 0; j < 10000; j++) {
            for (uint64 k = 0; k < 30000; k++) { /* busy wait */ }
            thread_dispatch();
        }
    }

    s = (char*)"B finished!\n"; printString(s);
    finishedB = true;
    thread_dispatch();
}
#ifndef THREADS_CPP_API_HPP
#define THREADS_CPP_API_HPP

#include "../h/syscall_cpp.hpp"
#include "../utils/printUtils.hpp"

bool finishedA = false;
bool finishedB = false;
bool finishedC = false;
bool finishedD = false;

uint64 fibonacci(uint64 n) {
    if (n == 0 || n == 1) { return n; }
    if (n % 10 == 0) { thread_dispatch(); }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

class WorkerA: public Thread {
    void workerBodyA(void* arg);
public:
    WorkerA():Thread() {}

    void run() override {
        workerBodyA(nullptr);
    }
};

class WorkerB: public Thread {
    void workerBodyB(void* arg);
public:
    WorkerB():Thread() {}

    void run() override {
        workerBodyB(nullptr);
    }
};

class WorkerC: public Thread {
    void workerBodyC(void* arg);
public:
    WorkerC():Thread() {}

    void run() override {
        workerBodyC(nullptr);
    }
};

class WorkerD: public Thread {
    void workerBodyD(void* arg);
public:
    WorkerD():Thread() {}

    void run() override {
        workerBodyD(nullptr);
    }
};

void WorkerA::workerBodyA(void* arg) {
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

void WorkerB::workerBodyB(void* arg) {
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
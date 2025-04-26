#ifndef THREADS_C_API_HPP
#define THREADS_C_API_HPP

#include "../h/syscall_c.hpp"

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

void workerBodyA(void* arg) {
    char* s;

    for (uint64 i = 0; i < 10; i++) {
        s = (char*)"A: i = ";
        printString(s); printInt(i); s = "\n"; printString(s);
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
        s = (char*)"B: i = ";
        printString(s); printInt(i); s = "\n"; printString(s);
        for (uint64 j = 0; j < 10000; j++) {
            for (uint64 k = 0; k < 30000; k++) { /* busy wait */ }
            thread_dispatch();
        }
    }
    s = (char*)"B finished!\n"; printString(s);
    finishedB = true;
    thread_dispatch();
}

void workerBodyC(void* arg) {
    char* s;
    uint8 i = 0;
    for (; i < 3; i++) {
        s = (char*)"C: i="; printString(s); printInt(i); s = (char*)"\n"; printString(s);
    }

    s = (char*)"C: dispatch\n"; printString(s);
    __asm__ ("li t1, 7");
    thread_dispatch();

    uint64 t1 = 0;
    __asm__ ("mv %[t1], t1" : [t1] "=r"(t1));

    s = (char*)"C: t1="; printString(s); printInt(t1); s = (char*)"\n"; printString(s);

    uint64 result = fibonacci(12);
    s = (char*)"C: fibonaci="; printString(s); printInt(result); s = (char*)"\n"; printString(s);

    for (; i < 6; i++) {
        s = (char*)"C: i="; printString(s); printInt(i); s = (char*)"\n"; printString(s);
    }

    s = (char*)"A finished!\n"; printString(s);
    finishedC = true;
    thread_dispatch();
}

void workerBodyD(void* arg) {
    char* s;
    uint8 i = 10;
    for (; i < 13; i++) {
        s = (char*)"D: i="; printString(s); printInt(i); s = (char*)"\n"; printString(s);
    }

    s = (char*)"D: dispatch\n"; printString(s);
    __asm__ ("li t1, 5");
    thread_dispatch();

    uint64 result = fibonacci(16);
    s = (char*)"D: fibonaci="; printString(s); printInt(result); s = (char*)"\n"; printString(s);

    for (; i < 16; i++) {
        s = (char*)"D: i="; printString(s); printInt(i); s = (char*)"\n"; printString(s);
    }

    s = (char*)"D finished!\n"; printString(s);
    finishedD = true;
    thread_dispatch();
}


void threads_C_API_test() {
    char* s;
    thread_t threads[4];
    thread_create(&threads[0], workerBodyA, nullptr);
    s = (char*)"ThreadA created\n"; printString(s);

    thread_create(&threads[1], workerBodyB, nullptr);
    s = (char*)"ThreadB created\n"; printString(s);

    thread_create(&threads[2], workerBodyC, nullptr);
    s = (char*)"ThreadC created\n"; printString(s);

    thread_create(&threads[3], workerBodyD, nullptr);
    s = (char*)"ThreadD created\n"; printString(s);

    while (!(finishedA && finishedB && finishedC && finishedD)) {
        thread_dispatch();
    }

}

#endif //THREADS_C_API_HPP

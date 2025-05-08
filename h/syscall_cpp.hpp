#ifndef SYSCALL_CPP_HPP
#define SYSCALL_CPP_HPP

#include "syscall_c.hpp"

// void* ::operator new(size_t);

// void ::operator delete(void *);

class Thread {
public:
    Thread(void (*body)(void *), void *arg) {
        thread_create(&myHandle, body, arg);
    }

    Thread() {
        myHandle = nullptr;
    }

    virtual ~Thread() {
        myHandle = nullptr;
    }

    int start() {
        if (myHandle == nullptr) {
            return thread_create(&myHandle, &wrapper, this);
        }

        return -1;
    }

    static void dispatch() {
        thread_dispatch();
    }

    static int sleep(time_t);

protected:

    virtual void run() {}

private:
    thread_t myHandle;
    void (*body)(void *);
    void *arg;

    static void wrapper(void* arg) {
        const auto t = static_cast<Thread *>(arg);

        t->run();
    }
};

class Semaphore {
public:
    Semaphore (unsigned init = 1) {
        sem_open(&myHandle, init);
    }

    virtual ~Semaphore () {
        sem_close(myHandle);
    }

    int wait () {
        return sem_wait(myHandle);
    }

    int signal () {
        return sem_signal(myHandle);
    }

    int timedWait (time_t);

    int tryWait();

private:
    sem_t myHandle;
};

class PeriodicThread : public Thread {
public:
    void terminate ();

protected:
    PeriodicThread (time_t period);

    virtual void periodicActivation () {}

private:
    time_t period;
};

class Console {
public:
    static char getc () { return ::getc(); }
    static void putc (char c) { ::putc(c); }

};

#endif //SYSCALL_CPP_HPP
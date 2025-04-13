#ifndef SYSCALL_CPP_HPP
#define SYSCALL_CPP_HPP

#include "syscall_c.hpp"

void* ::operator new(size_t);

void ::operator delete(void *);

class Thread {
public:
    Thread(void (*body)(void *), void *arg);

    virtual ~Thread();

    int start();

    static void dispatch();

    static int sleep(time_t);

protected:
    Thread();

    virtual void run() {
    }

private:
    thread_t myHandle;

    void (*body)(void *);

    void *arg;
};

class Semaphore {
public:
    Semaphore (unsigned init = 1);

    virtual ~Semaphore ();

    int wait ();

    int signal ();

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

//console left

#endif //SYSCALL_CPP_HPP

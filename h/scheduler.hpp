#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP
#include "deque.hpp"

class Thread;

class Scheduler {
private:
    static Deque<Thread> readyThreadQueue;
    static Scheduler* instance;

    Scheduler(){}
    ~Scheduler()= default;

public:
    static Scheduler* getInstance();

    //ccb - coroutine control block
    Thread* get();
    void put(Thread* ccb);
};

#endif //SCHEDULER_HPP

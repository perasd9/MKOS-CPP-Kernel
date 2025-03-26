#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP
#include "deque.hpp"
#include "thread.hpp"

class Scheduler {
private:
    static Deque<Thread> readyThreadQueue;
    static Scheduler* instance;

    Scheduler(){}
    ~Scheduler()= default;

public:
    static Scheduler* getInstance();

    //ccb - coroutine control block
    static Thread* get();
    static void put(Thread* ccb);
};

#endif //SCHEDULER_HPP

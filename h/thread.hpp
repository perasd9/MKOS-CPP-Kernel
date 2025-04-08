#ifndef THREAD_HPP
#define THREAD_HPP

#include "scheduler.hpp"
#include "../lib/hw.h"

class Thread {
public:
    ~Thread() {
        delete[] stack;
    }

    //getter and setter for 'timeSlice' field which represent period of "periods" of executing thread
    uint64 get_time_slice() const {
        return timeSlice;
    }

    void set_time_slice(uint64 time_slice) {
        timeSlice = time_slice;
    }

    //reference to the function body which will be executed by thread
    using Body = void(*)(void *);

    //currently running thread with context
    static Thread *running;

    //getter and setter for 'finished' field which represents is thread finished
    bool is_finished() const {
        return finished;
    }

    void set_finished(const bool finished) {
        this->finished = finished;
    }

    static Thread *createThread(Body body);

    static void yield();

    //various status of thread unit
    enum Status {
        NEW,
        READY,
        FINISHED,
        BLOCKED,
        RUNNING
    };

private:
    friend class RiscV;

    //private constructor
    explicit Thread(Body body, uint64 *stack, uint64 timeSlice, void *arg)
        : body(body),
          timeSlice(timeSlice),
          stack(body == nullptr
                    ? nullptr
                    : new uint64[DEFAULT_STACK_SIZE]),
          ctx({
              (uint64) &threadWrapper,
              stack == nullptr
                  ? 0
                  : (uint64) &stack[DEFAULT_STACK_SIZE]
          }),
          finished(false) {
        if (this->body != nullptr) Scheduler::getInstance()->put(this);
    }

    struct Context {
        uint64 ra;
        uint64 sp;
    };

    Body body;
    uint64 timeSlice;
    uint64 *stack;
    Context ctx;
    bool finished;
    void *arg;

    //currently counter which represents on which timeSlice is stopped thread ex. counter is 1 and timeSlice is 2
    //it means one more period is left to the end of exeuction
    static uint64 timeSliceCounter;

    static void contextSwitch(Context *oldCtx, Context *newCtx);

    static void dispatch();

    static void threadWrapper();
};

#endif //THREAD_HPP
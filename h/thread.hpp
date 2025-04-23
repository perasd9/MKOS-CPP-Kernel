#ifndef THREAD_HPP
#define THREAD_HPP

#include "scheduler.hpp"
#include "../lib/hw.h"
#include "../utils/printUtils.hpp"

class Thread {
public:
    ~Thread() {
        delete[] stack;
    }

    //getter and setter for 'timeSlice' field which represent period of "periods" of executing thread
    uint64 getTimeSlice() const {
        return timeSlice;
    }

    void setTimeSlice(uint64 time_slice) {
        timeSlice = time_slice;
    }

    //reference to the function body which will be executed by thread
    using Body = void(*)(void *);

    //currently running thread with context
    static Thread *running;

    //getter and setter for 'finished' field which represents is thread finished
    bool isFinished() const {
        return finished;
    }

    void setFinished(const bool finished) {
        this->finished = finished;
    }

    static Thread *createThread(Body body, void* arg);

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
          stack(nullptr),
          ctx({0,0}),
          finished(false) {

        if (body != nullptr) {
            this->stack = new uint64[DEFAULT_STACK_SIZE];

            this->ctx.ra = (uint64)&threadWrapper;
            this->ctx.sp = (uint64)&this->stack[DEFAULT_STACK_SIZE];

            Scheduler::getInstance()->put(this);
        }
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
    //it means one more period is left to the end of execution
    static uint64 timeSliceCounter;

    ///defined in assembly file
    static void contextSwitch(Context *oldCtx, Context *newCtx);

    static void dispatch();

    static void threadWrapper();
};

#endif //THREAD_HPP
#ifndef THREAD_HPP
#define THREAD_HPP

#include "scheduler.hpp"
#include "syscall_c.hpp"
#include "../lib/hw.h"

class Thread {
public:
    ~Thread() {
        delete[] stack;
    }

    //various status of thread unit
    enum Status {
        NEW,
        READY,
        FINISHED,
        BLOCKED,
        RUNNING
    };

    //getter and setter for 'timeSlice' field which represent period of "periods" of executing thread
    uint64 getTimeSlice() const {
        return timeSlice;
    }

    void setTimeSlice(uint64 time_slice) {
        timeSlice = time_slice;
    }


    //getter and setter for 'finished' field which represents is thread finished
    bool isFinished() const {
        return finished;
    }

    void setFinished(const bool finished) {
        this->finished = finished;
    }


    //getter and setter for 'status' field
    Status getStatus() const {
        return status;
    }

    void setStatus(Status status) {
        this->status = status;
    }


    //reference to the function body which will be executed by thread
    using Body = void(*)(void *);

    //currently running thread with context
    static Thread *running;

    static int createThread(/* Thread** */ thread_t* handle, Body body, void* arg, uint64* stack);

    static void yield();


private:
    friend class RiscV;
    friend void thread_dispatch();

    //private constructor
    explicit Thread(const Body body, const uint64 timeSlice, void *arg)
        : body(body),
          timeSlice(timeSlice),
          stack(nullptr),
          ctx({0,0}),
          finished(false),
          arg(arg),
          status(NEW){

        /*if (body != nullptr) {
            //stack need to be initialized by c or cpp api, only then ABI will call constructor for other fields
            // this->stack = new uint64[DEFAULT_STACK_SIZE];

            this->ctx.ra = reinterpret_cast<uint64>(&threadWrapper);
            this->ctx.sp = reinterpret_cast<uint64>(&this->stack[DEFAULT_STACK_SIZE]);

            Scheduler::getInstance()->put(this);
        }*/
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

    Status status;

    //currently counter which represents on which timeSlice is stopped thread ex. counter is 1 and timeSlice is 2
    //it means one more period is left to the end of execution
    static uint64 timeSliceCounter;

    //implemented in assembly language in .S file, that's warning
    static void contextSwitch(Context *oldCtx, Context *newCtx);

    static void dispatch();

    static void threadWrapper();
};

#endif //THREAD_HPP

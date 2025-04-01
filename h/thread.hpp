#ifndef THREAD_HPP
#define THREAD_HPP

class Thread {
public:

    using Body = void(*)(void*);

    enum Status {
        NEW,
        READY,
        FINISHED,
        BLOCKED,
        RUNNING
    };

private:

    Thread(Body body, uint64* stack, uint64 timeSlice, void* arg);

    struct Context {
        uint64 ra;
        uint64 sp;
    };

    Body body;
    uint64 timeSlice;
    uint64* stack;
    Context ctx;
    bool finished;


    static void contextSwitch(Context* oldCtx, Context* newCtx);

};

#endif //THREAD_HPP
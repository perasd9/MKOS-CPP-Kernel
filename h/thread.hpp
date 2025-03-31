#ifndef THREAD_HPP
#define THREAD_HPP

class Thread {
public:

private:

    struct Context {
        uint64 ra;
        uint64 sp;
    };

    static void contextSwitch(Context* oldCtx, Context* newCtx);

};

#endif //THREAD_HPP
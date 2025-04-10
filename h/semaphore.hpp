#ifndef SEMAPHORE_HPP
#define SEMAPHORE_HPP

#include "../lib/hw.h"

class Semaphore {
public:

    explicit Semaphore(uint64 init = 1) : value(init) {

    }
private:

    int value;

};

#endif //SEMAPHORE_HPP

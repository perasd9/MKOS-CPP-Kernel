#include "buffer.hpp"
#include "../utils/printUtils.hpp"


void Buffer::put(int val) {
    sem_wait(spaceAvailable);

    sem_wait(mutexTail);
    buffer[tail] = val;
    tail = (tail + 1) % cap;
    sem_signal(mutexTail);

    sem_signal(itemAvailable);

}

int Buffer::get() {
    sem_wait(itemAvailable);

    sem_wait(mutexHead);

    int ret = buffer[head];
    head = (head + 1) % cap;
    sem_signal(mutexHead);

    sem_signal(spaceAvailable);

    return ret;
}

int Buffer::getCnt() {
    int ret;

    sem_wait(mutexHead);
    sem_wait(mutexTail);

    if (tail >= head) {
        ret = tail - head;
    } else {
        ret = cap - head + tail;
    }

    sem_signal(mutexTail);
    sem_signal(mutexHead);

    return ret;
}
#ifndef PRODUCER_CONSUMER_C_API_HPP
#define PRODUCER_CONSUMER_C_API_HPP

#include "buffer.hpp"
#include "../h/syscall_c.hpp"
#include "../utils/printUtils.hpp"

sem_t waitForAll;

struct thread_data {
    int id;
    Buffer *buffer;
    sem_t wait;
};

volatile int threadEnd = 0;

void producerKeyboard(void *arg) {
    struct thread_data *data = (struct thread_data *) arg;

    int key;
    int i = 0;
    while ((key = getc()) != 0x1b) {
        data->buffer->put(key);
        i++;

        if (i % (10 * data->id) == 0) {
            thread_dispatch();
        }
    }

    threadEnd = 1;
    data->buffer->put('!');

    sem_signal(data->wait);
}

void producer(void *arg) {
    struct thread_data *data = (struct thread_data *) arg;

    int i = 0;
    while (!threadEnd) {
        data->buffer->put(data->id + '0');
        i++;

        if (i % (10 * data->id) == 0) {
            thread_dispatch();
        }
    }

    sem_signal(data->wait);
}
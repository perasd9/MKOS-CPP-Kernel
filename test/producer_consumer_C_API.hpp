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

void consumer(void *arg) {
    struct thread_data *data = (struct thread_data *) arg;

    int i = 0;
    while (!threadEnd) {
        int key = data->buffer->get();
        i++;

        putc(key);

        if (i % (5 * data->id) == 0) {
            thread_dispatch();
        }

        if (i % 80 == 0) {
            putc('\n');
        }
    }

    while (data->buffer->getCnt() > 0) {
        int key = data->buffer->get();
        putc(key);
    }

    sem_signal(data->wait);
}

void producerConsumer_C_API() {
    char* s;
    char input[30];
    int n, threadNum;

    s = (char*)"Unesite broj proizvodjaca?\n"; printString(s);
    getString(input, 30);
    threadNum = stringToInt(input);

    s = (char*)"Unesite velicinu bafera?\n"; printString(s);
    getString(input, 30);
    n = stringToInt(input);

    s = (char*)"Broj proizvodjaca "; printString(s); printInt(threadNum);
    s = (char*)" i velicina bafera "; printString(s); printInt(n);
    s = (char*) ".\n"; printString(s);

    if(threadNum > n) {
        s = (char*) "Broj proizvodjaca ne sme biti manji od velicine bafera!\n"; printString(s);

        return;
    } else if (threadNum < 1) {
        s = (char*) "Broj proizvodjaca mora biti veci od nula!\n"; printString(s);

        return;
    }

    auto buffer = new Buffer(n);

    sem_open(&waitForAll, 0);

    thread_t threads[threadNum];
    thread_t consumerThread;

    struct thread_data data[threadNum + 1];

    data[threadNum].id = threadNum;
    data[threadNum].buffer = buffer;
    data[threadNum].wait = waitForAll;
    thread_create(&consumerThread, consumer, data + threadNum);

    for (int i = 0; i < threadNum; i++) {
        data[i].id = i;
        data[i].buffer = buffer;
        data[i].wait = waitForAll;

        thread_create(threads + i,
                      i > 0 ? producer : producerKeyboard,
                      data + i);
    }

    thread_dispatch();

    for (int i = 0; i <= threadNum; i++) {
        sem_wait(waitForAll);
    }

    sem_close(waitForAll);

    delete buffer;

}

#endif //PRODUCER_CONSUMER_C_API_HPP

#ifndef PRODUCER_CONSUMER_CPP_API_SYNC_HPP
#define PRODUCER_CONSUMER_CPP_API_SYNC_HPP

#include "buffer_CPP_API.hpp"
#include "../h/syscall_cpp.hpp"
#include "../utils/printUtils.hpp"

Semaphore* waitForAll;

struct thread_data {
    int id;
    BufferCPP *buffer;
    Semaphore* wait;
};

volatile int threadEnd = 0;

class ProducerKeyboard:public Thread {
    thread_data* td;
    void producerKeyboard(void *arg);
public:
    ProducerKeyboard(thread_data* _td):Thread(), td(_td) {}

    void run() override {
        producerKeyboard(td);
    }
};

void ProducerKeyboard::producerKeyboard(void *arg) {
    struct thread_data *data = (struct thread_data *) arg;

    int key;
    int i = 0;
    while ((key = getc()) != 0x1b) {
        data->buffer->put(key);
        i++;

        if (i % (10 * data->id) == 0) {
            Thread::dispatch();
        }
    }

    threadEnd = 1;
    td->buffer->put('!');

    data->wait->signal();
}

class Producer:public Thread {
    thread_data* td;
    void producer(void *arg);
public:
    Producer(thread_data* _td):Thread(), td(_td) {}

    void run() override {
        producer(td);
    }
};

void Producer::producer(void *arg) {
    struct thread_data *data = (struct thread_data *) arg;

    int i = 0;
    while (!threadEnd) {
        data->buffer->put(data->id + '0');
        i++;

        if (i % (10 * data->id) == 0) {
            Thread::dispatch();
        }
    }

    data->wait->signal();
}

class Consumer:public Thread {
    thread_data* td;
    void consumer(void *arg);
public:
    Consumer(thread_data* _td):Thread(), td(_td) {}

    void run() override {
        consumer(td);
    }
};

void Consumer::consumer(void *arg) {
    struct thread_data *data = (struct thread_data *) arg;

    int i = 0;
    while (!threadEnd) {
        int key = data->buffer->get();
        i++;

        putc(key);

        if (i % (5 * data->id) == 0) {
            Thread::dispatch();
        }

        if (i % 80 == 0) {
            putc('\n');
        }
    }


    while (td->buffer->getCnt() > 0) {
        int key = td->buffer->get();
        Console::putc(key);
    }

    data->wait->signal();
}

void producerConsumer_CPP_Sync_API() {
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

    BufferCPP *buffer = new BufferCPP(n);

    waitForAll = new Semaphore(0);

    Thread* threads[threadNum];
    Thread* consumerThread;

    struct thread_data data[threadNum + 1];

    data[threadNum].id = threadNum;
    data[threadNum].buffer = buffer;
    data[threadNum].wait = waitForAll;
    consumerThread = new Consumer(data+threadNum);
    consumerThread->start();

    for (int i = 0; i < threadNum; i++) {
        data[i].id = i;
        data[i].buffer = buffer;
        data[i].wait = waitForAll;

        if(i>0) {
            threads[i] = new Producer(data+i);
        } else {
            threads[i] = new ProducerKeyboard(data+i);
        }

        threads[i]->start();
    }

    Thread::dispatch();

    for (int i = 0; i <= threadNum; i++) {
        waitForAll->wait();
    }

    for (int i = 0; i < threadNum; i++) {
        delete threads[i];
    }
    delete consumerThread;
    delete waitForAll;
    delete buffer;

}

#endif //PRODUCER_CONSUMER_CPP_API_SYNC_HPP

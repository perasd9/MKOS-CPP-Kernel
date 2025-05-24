#ifndef PRODUCER_CONSUMER_CPP_API_HPP
#define PRODUCER_CONSUMER_CPP_API_HPP

#include "buffer_CPP_API.hpp"
#include "../h/syscall_cpp.hpp"
#include "../utils/printUtils.hpp"

namespace ConsumerProducerCPP {

    Semaphore *waitForAll;

    struct thread_data {
        int id;
        BufferCPP *buffer;
        Semaphore *sem;
    };

    volatile int threadEnd = 0;

    class ProducerKeyborad : public Thread {
        thread_data *td;
    public:
        explicit ProducerKeyborad(thread_data *_td) : Thread(), td(_td) {}

        void run() override {
            int key;
            int i = 0;
            while ((key = getc()) != 0x1b) {
                td->buffer->put(key);
                i++;
            }

            threadEnd = 1;
            td->buffer->put('!');

            td->sem->signal();
        }
    };
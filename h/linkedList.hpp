#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include "../lib/hw.h"

struct Node{
    size_t size;
    Node* next;
    Node* previous;
};

class LinkedList {
public:
    Node* head;
    Node* tail;
    LinkedList() : head(nullptr), tail(nullptr) {}

private:

};

#endif //LINKEDLIST_HPP

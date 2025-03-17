#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include "../lib/hw.h"

struct Node{
    size_t size;
    Node* next;
};

class LinkedList {
public:
    Node* head;
    LinkedList() : head(nullptr) {}

private:

};

#endif //LINKEDLIST_HPP
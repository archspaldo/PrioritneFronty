#include "BinaryQueue.h"
#include "PairingHeap.h"
#include "BinomialHeap.h"
#include "FibonacciHeap.h"
#include "PairingHeapBinaryTree.h"
#include "heap_monitor.h"
#include <iostream>

class a
{
    int data_;
    int priority_;
public:
    a(int data, int priority) :
        data_(data), priority_(priority) {};
    int data() { return data_; };
    int priority() { return priority_; };
};

class b : public a
{
    a* ptr_;
public:
    b(int data, int priority) :
        a(data, priority), ptr_(nullptr) {};
    a* ptr() { return ptr_; };
    void ptr(a* ptr) { ptr_ = ptr; };
};



int main() {
    initHeapMonitor();
    PriorityQueue<int, int>* h = new BinomialHeap<int, int>();
    h->push(3, 3);
    h->push(15, 15);
    h->push(10, 10);
    h->push(7, 7);
    h->push(0, 0);
    h->push(1, 1);
    h->push(3, 3);
    h->push(4, 4);
    h->push(2, 2);
    
    std::cout << h->pop() << "\n";

    for (int i = 0; i < 8; i++)
    {
        std::cout << h->pop() << "\n";
    }

    delete h;

   /* PQDoubleLinkedItem<int, int>* i0 = new PQDoubleLinkedItem<int, int>(0, 0);
    PQDoubleLinkedItem<int, int>* i1 = new PQDoubleLinkedItem<int, int>(1, 1);
    PQDoubleLinkedItem<int, int>* i2 = new PQDoubleLinkedItem<int, int>(2, 2);
    PQDoubleLinkedItem<int, int>* i3 = new PQDoubleLinkedItem<int, int>(3, 3);
    PQDoubleLinkedItem<int, int>* i4 = new PQDoubleLinkedItem<int, int>(4, 4);
    PQDoubleLinkedItem<int, int>* i5 = new PQDoubleLinkedItem<int, int>(5, 5);
    PQDoubleLinkedItem<int, int>* i6 = new PQDoubleLinkedItem<int, int>(6, 6);
    PQDoubleLinkedItem<int, int>* i7 = new PQDoubleLinkedItem<int, int>(7, 7);
    PQDoubleLinkedItem<int, int>* i8 = new PQDoubleLinkedItem<int, int>(8, 8);

    i6->link_on_right(i1);
    i1->link_on_right(i2);
    i2->link_on_right(i3);
    i6->merge_with_right_sibling();
    i6->link_on_right(i0);
    i0->heapify();



    delete i0;*/

    /*a* a1 = new b(1, 1);
    a* a2 = new b(2, 2);
    ((b*)a1)->ptr(a2);

    std::cout << ((b*)a1)->ptr()->data();*/

    return 0;
}
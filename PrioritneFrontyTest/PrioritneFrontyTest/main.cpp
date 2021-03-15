/*#include "FibonacciHeap.h"
#include "BinomialHeap.h"
#include "heap_monitor.h"*/
#include <iostream>

class a {
public:
    int data_;
    a* next_;
    a* prev_;
    a(int data) : data_(data), next_(nullptr), prev_(nullptr) {};
    virtual void next(a* next) { this->next_ = next; next->prev_ = this; };
};

class b : public a {
public:
    b(int data) : a(data) {};
    void next(a* next) override { this->next_ = next; };
};

int main()
{
    a* a1 = new a(1);
    a* a2 = new b(2);

    a2->a::next(a1);
    return 0;
    /*initHeapMonitor();
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

    delete h;*/
}
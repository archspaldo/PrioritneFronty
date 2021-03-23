#include "FibonacciHeap.h"
#include "BinomialHeap.h"
#include "PairingHeap.h"
#include "heap_monitor.h"
#include "RankPairingHeap.h"
#include <iostream>


int main()
{
   
    initHeapMonitor();
    PriorityQueue<int, int>* h = new BinomialHeapMultiPass<int, int>();
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
    return 0;
}
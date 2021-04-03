#include "Test.h"
#include "heap_monitor.h"
#include <iostream>


int main()
{
    srand(69);
    //srand(time(NULL));
    initHeapMonitor();
    /*h->push(3, 3);
    h->push(15, 15);
    h->push(10, 10);
    h->push(7, 7);
    h->push(0, 0);
    h->push(1, 1);
    h->push(3, 3);
    h->push(4, 4);
    h->push(2, 2);*/

    /*std::cout << h->pop() << "\n";

    for (int i = 0; i < 8; i++)
    {
        std::cout << h->pop() << "\n";
    }*/
    PriorityQueueList<int, int>* list = new PriorityQueueList<int, int>();
    RandomTest().execute(list);
    delete list;

    /*PriorityQueueItem<int, int>* a = new FibonacciHeapItem<int, int>(1, 1, 1), * b = nullptr;

    std::chrono::system_clock::time_point thetime = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point time1 = std::chrono::system_clock::now();
    for (int i = 0; i < 1000000; i++)
    {
        b = a;
    }
    thetime = std::chrono::system_clock::now();

    std::cout << (thetime - time1).count() << '\n' << thetime.time_since_epoch().count() << '\n' << time1.time_since_epoch().count() ;*/

    return 0;
}
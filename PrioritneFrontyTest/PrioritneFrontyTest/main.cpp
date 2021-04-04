#include "Test.h"
#include "heap_monitor.h"
#include <iostream>


int main()
{
    //srand(0);
    srand(time(NULL));
    initHeapMonitor();
    PriorityQueueList<int, int>* list = new PriorityQueueList<int, int>();
    RandomTest().execute(list);
    delete list;
    system("pause");
    return 0;
}
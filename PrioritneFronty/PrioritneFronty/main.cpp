#include "BinaryQueue.h"
#include "LinkedList.h"
#include "PairingHeap.h"
#include "BinomialHeap.h"
#include <iostream> 

int main() {
    BinomialHeap<int, int>* h = new BinomialHeap<int, int>();
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

    for (int i = 0; i < 9; i++)
    {
        std::cout << h->pop() << "\n";
    }

    delete h;

    return 0;
    /*LinkedList<int> list;

    for (int i = 0; i < 10 || i < 11;i++)
    {
        list.add(i);
    }

    for (int i : list)
    {
        std::cout << i << '\n';
    }
    */
}
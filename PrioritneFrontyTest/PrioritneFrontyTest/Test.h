#pragma once
#include <list>
#include "BinaryHeap.h"
#include "BinomialHeap.h"
#include "FibonacciHeap.h"
#include "PairingHeap.h"
#include "RankPairingHeap.h"

class Test
{
private:
	std::list<PriorityQueue<int, int>*>* pqlist_;
protected:
	void execute();
public:
	Test();
};


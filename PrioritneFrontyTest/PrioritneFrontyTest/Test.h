#pragma once
#include <algorithm>
#include <random>
#include <iostream>
#include <list>
#include <unordered_map>
#include <chrono>
#include "Wrapper.h"

template  <typename Priority, typename Data>
class Test
{
public:
	virtual void execute(PriorityQueueList<Priority, Data>* = nullptr) = 0;
};

class PushTest : public Test<int, int>
{
public:
	void execute(PriorityQueueList<int, int>* = nullptr);
};

class RandomTest : public Test<int, int>
{
public:
	void execute(PriorityQueueList<int, int>* = nullptr);
};

inline void RandomTest::execute(PriorityQueueList<int, int>* pq_list)
{
	for (int j = 0; j < 10; j++)
	{
		for (int i = 0; i < 100000; i++)
		{
			int number = rand() % 100;
			if (number < 30)
			{
				pq_list->push(i, rand() % UINT32_MAX, rand() % UINT32_MAX);
			}
			else if (number < 50 && pq_list->size() > 0)
			{
				pq_list->pop();
			}
			else if (number < 60 && pq_list->size() > 0)
			{
				pq_list->find_min();
			}
			else if (number < 80 && pq_list->size() > 0)
			{
				pq_list->change_priority(pq_list->get_random_identifier(), rand() % UINT32_MAX);
			}
		}
		pq_list->reset();
	}
}

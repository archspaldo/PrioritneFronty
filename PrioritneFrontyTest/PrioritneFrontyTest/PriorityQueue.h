#pragma once
#include "PriorityQueueItems.h"
#include <stdexcept>
#include <vector>
#include <iostream>
#include <fstream>

template <typename Priority, typename Data>
class PriorityQueue
{
protected:
	PriorityQueue();
	virtual void priority_was_increased(PriorityQueueItem<Priority, Data>* node) = 0;
	virtual void priority_was_decreased(PriorityQueueItem<Priority, Data>* node) = 0;
public:
	virtual ~PriorityQueue();
	virtual void clear() = 0;
	virtual size_t size() const = 0;
	virtual void push(const int identifier, const Priority& priority, const Data& data, PriorityQueueItem<Priority, Data>*& node) = 0;
	virtual Data pop(int& identifier) = 0;
	virtual Data& find_min() = 0;
	virtual void merge(PriorityQueue<Priority, Data>* other_heap) = 0;
	virtual void change_priority(PriorityQueueItem<Priority, Data>* node, const Priority& priority);
};

template<typename Priority, typename Data>
inline PriorityQueue<Priority, Data>::PriorityQueue()
{
}

template<typename Priority, typename Data>
inline PriorityQueue<Priority, Data>::~PriorityQueue()
{
}

template<typename Priority, typename Data>
inline void PriorityQueue<Priority, Data>::change_priority(PriorityQueueItem<Priority, Data>* node, const Priority& priority)
{
	Priority old_priority = node->priority();
	node->priority() = priority;
	if (priority < old_priority)
	{
		this->priority_was_increased(node);
	}
	else if (priority > old_priority)
	{
		this->priority_was_decreased(node);
	}
}

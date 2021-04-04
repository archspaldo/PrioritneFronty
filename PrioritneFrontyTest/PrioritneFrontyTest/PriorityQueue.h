#pragma once
#include "PriorityQueueItems.h"
#include <stdexcept>
#include <vector>

template <typename K, typename T>
class PriorityQueue
{
protected:
	PriorityQueue();
	virtual void priority_was_increased(PriorityQueueItem<K, T>* node) = 0;
	virtual void priority_was_decreased(PriorityQueueItem<K, T>* node) = 0;
public:
	virtual ~PriorityQueue();
	virtual void clear() = 0;
	virtual size_t size() const = 0;
	virtual void push(const int identifier, const K& priority, const T& data, PriorityQueueItem<K, T>*& node) = 0;
	virtual T pop(int& identifier) = 0;
	virtual T& find_min() = 0;
	virtual void merge(PriorityQueue<K, T>* other_heap) = 0;
	void change_priority(PriorityQueueItem<K, T>* node, const K& priority);
};


template<typename K, typename T>
inline PriorityQueue<K, T>::PriorityQueue()
{
}

template<typename K, typename T>
inline PriorityQueue<K, T>::~PriorityQueue()
{
}

template<typename K, typename T>
inline void PriorityQueue<K, T>::change_priority(PriorityQueueItem<K, T>* node, const K& priority)
{
	K old_priority = node->priority();
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


#pragma once
#include <stdexcept>
#include "PriorityQueueItems.h"

template <typename K, typename T>
class PriorityQueue
{
public:
	virtual ~PriorityQueue() {};
	virtual size_t size() const = 0;
	virtual PriorityQueueItem<K, T>* push(const K& key, const T& data) = 0;
	virtual T pop() = 0;
	virtual T& peek() = 0;
	virtual const T peek() const = 0;
	virtual K peekPriority() = 0;
	virtual void merge(PriorityQueue<K, T>* other_heap) = 0;
	virtual void change_priority(PriorityQueueItem<K, T>* node, const K& priority) = 0;
};

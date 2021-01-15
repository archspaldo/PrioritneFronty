#pragma once
#include "Routines.h"
#include <stdexcept>

template <typename K, typename T>
class PriorityQueueItem
{
public:
	PriorityQueueItem(const K& priority, const T& data);
	virtual ~PriorityQueueItem() {};
	K& priority();
	T& data();
private:
	K priority_;
	T data_;
};


template <typename K, typename T>
class PriorityQueue
{
public:
	virtual PriorityQueue<K, T>& operator=(const PriorityQueue<K, T>& other) = 0;
	virtual PriorityQueue<K, T>& operator=(PriorityQueue<K, T>&& other) = 0;
	virtual void clear() = 0;
	virtual size_t size() const = 0;
	virtual void push(const K& key, const T& data) = 0;
	virtual T pop() = 0;
	virtual T& peek() = 0;
	virtual const T peek() const = 0;
	virtual K peekPriority() = 0;
};

template<typename K, typename T>
inline PriorityQueueItem<K, T>::PriorityQueueItem(const K& priority, const T& data) :
	priority_(priority), data_(data)
{
}

template<typename K, typename T>
inline K& PriorityQueueItem<K, T>::priority()
{
	return priority_;
}

template<typename K, typename T>
inline T& PriorityQueueItem<K, T>::data()
{
	return data_;
}

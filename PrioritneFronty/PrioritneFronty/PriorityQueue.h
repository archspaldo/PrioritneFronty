#pragma once
#include <stdexcept>

template <typename K, typename T>
class PriorityQueue
{
public:
	virtual void clear() = 0;
	virtual size_t size() const = 0;
	virtual void push(const K& key, const T& data) = 0;
	virtual T pop() = 0;
	virtual T& peek() = 0;
	virtual const T peek() const = 0;
	virtual K peekPriority() = 0;
protected:
	class PriorityQueueItem;
};

template <typename K, typename T>
class PriorityQueue<K, T>::PriorityQueueItem
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

template<typename K, typename T>
inline PriorityQueue<K, T>::PriorityQueueItem::PriorityQueueItem(const K& priority, const T& data) :
	priority_(priority), data_(data)
{
}

template<typename K, typename T>
inline K& PriorityQueue<K, T>::PriorityQueueItem::priority()
{
	return priority_;
}

template<typename K, typename T>
inline T& PriorityQueue<K, T>::PriorityQueueItem::data()
{
	return data_;
}
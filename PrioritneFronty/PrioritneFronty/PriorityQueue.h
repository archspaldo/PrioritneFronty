#pragma once

template <typename K, typename T>
class PriorityQueueItem
{
public:
	PriorityQueueItem(K priority, T data);
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
	virtual void clear() = 0;
	virtual size_t size() = 0;
	virtual void push(K& key, T& data) = 0;
	virtual T pop() = 0;
	virtual T& peek() = 0;
	virtual const T peek() const = 0;
	virtual K peekPriority() = 0;
};

template<typename K, typename T>
inline PriorityQueueItem<K, T>::PriorityQueueItem(K priority, T data) :
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
